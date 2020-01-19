#include "BBGameManager.h"
#include "BBConst.h"
#include "BBMathUtils.h"
#include <sstream>

#if defined(_WIN32) && defined(_WINDOWS)
#include "cocos2d.h"
#else
#include<stdarg.h>
#include<sys/time.h>
#endif

BBGameManager* BBGameManager::Create()
{
	return new BBGameManager();
}

void BBGameManager::Destory(BBGameManager* manager)
{
	if (manager)
	{
		delete manager;
	}
}

BBGameManager::BBGameManager():
	gameFrame(0),
	currentGetServerKeyFrame(0),
	isServer(true),
	timeMove(0),
	timeEat(0)
{
	BBMathUtils::init_crc_table();
	hitManager.SetGameManager(this);
	playerManager.SetGameManager(this);
	foodSpikyManager.SetGameManager(this);
	sporeManager.SetGameManager(this);
	
}


BBGameManager::~BBGameManager()
{
}


void BBGameManager::InitRoom()
{
	NodeTree.SetTreeWH(BBConst::MaxWidth, BBConst::MaxHeight);
	NodeTree.OnBuildFinish(NULL);

	spikyRect.setRect(500, 500, BBConst::MaxWidth - 500, BBConst::MaxHeight - 500);
	mapRect.setRect(0, 0, BBConst::MaxWidth, BBConst::MaxHeight);

	eatResults.reserve(100);
}


void BBGameManager::InitFoodSpiky()
{
	if (!isServer)
	{
		return;
	}
	foodSpikyManager.ServerInitFoodSpiky();
}

std::vector<int> BBGameManager::ClientGenerateFood(int num)
{
	return foodSpikyManager.ClientGenerateFood(num);
}

std::vector<int> BBGameManager::ClientGenerateSpiky(int num)
{
	return foodSpikyManager.ClientGenerateSpiky(num);
}

//*************食物部分****************//

void BB_gettimeofday(timeval& tm)
{
#if defined(_WIN32) && defined(_WINDOWS)
	cocos2d::gettimeofday(&tm, NULL);
#else
	gettimeofday(&tm, NULL);
#endif
}

int BB_getCost(timeval time1, timeval time2)
{
	return (time1.tv_sec - time2.tv_sec) * 1000000 + (time1.tv_usec - time2.tv_usec);
}

//tick 更新
void BBGameManager::OnFrameUpdate()
{
	gameFrame++;
	eatResults.clear();
	timeval tm1 = { 0 };
	BB_gettimeofday(tm1);
	playerManager.MovePlayers();
	sporeManager.MoveSpores();
	timeval tm2 = { 0 };
	BB_gettimeofday(tm2);
	playerManager.DoEat(eatResults);
	timeval tm3 = { 0 };
	BB_gettimeofday(tm3);
	timeMove = BB_getCost(tm2, tm1);
	timeEat = BB_getCost(tm3, tm2);
	timeHit = playerManager.timeHit;
	timeUpdateCircle = playerManager.timeUpdateCirle;
	timeMovePlayer = playerManager.timeMovePlayer;
}

void BBGameManager::ClientKeyFrameUpdateBefore()
{
	playerManager.RemoveMass();
	playerManager.FinishEat();
	playerManager.DoSpikySplit();
}

void BBGameManager::OnKeyFrameUpdate()
{
	BBMathUtils::BBLOG("current frame is: %d", gameFrame);
	playerManager.RemoveMass();
	playerManager.FinishEat();
	playerManager.DoSpikySplit();

	if (isServer)
	{
		playerManager.AjustVector();
		playerManager.KeyFrameUpdatePlayer();
		playerManager.DoShoot();
		playerManager.DoPlayerSplit();

		playerManager.DoJoin();
		foodSpikyManager.ServerCheckRegenFood();
		AddNodesCd();
	}
}

void BBGameManager::DoPlayerSplit()
{
	playerManager.DoPlayerSplit();
}

void BBGameManager::ClientKeyFrameUpdate()
{
	BBMathUtils::BBLOG("current frame is: %d", gameFrame);
	playerManager.AjustVector();
	playerManager.KeyFrameUpdatePlayer();
	playerManager.DoShoot();
	//playerManager.DoJoin();
	//foodSpikyManager.ServerCheckRegenFood();
	//AddNodesCd();
}

//由Lua启动,没收到服务器数据那么就开始模拟
void BBGameManager::StartSimulate(int serverKeyFrame)
{
	playerManager.KeyFrameUpdatePlayer();
}

void BBGameManager::OnSimulate(bool isSimulateKeyFrame)
{
	playerManager.MovePlayers();
	sporeManager.MoveSpores();
	if (isSimulateKeyFrame)
	{
		playerManager.KeyFrameUpdatePlayer();
	}
}

//停止模拟之前需要由lua来传入serverKeyFrame + 1的帧数据
void BBGameManager::StopSimulate(int serverKeyFrame)
{
}

//开始同步服务器第serverKeyFrame帧的操作数据
void BBGameManager::ClientBeginSyncServerOperate(int serverKeyFrame)
{
	currentGetServerKeyFrame = serverKeyFrame;
}

void BBGameManager::AddNodesCd()
{
	auto& InCdNodes = frameCacheManager.InCdNodes;
	for (auto iter : InCdNodes)
	{
		int nodeIdx = iter.first;
		int playerId = iter.second;

		if (playerManager.mapPlayNodes.find(nodeIdx) != playerManager.mapPlayNodes.end())
		{
			PlayerNode* playerNode = playerManager.mapPlayNodes[nodeIdx];
			playerNode->ChangeCd(BBConst::MergeCD);
		}
	}
}

void BBGameManager::EndUpdate()
{
	frameCacheManager.OnKeyFrameUpdate();
}

std::vector<int> BBGameManager::GetAllFoodInfos()
{
	return foodSpikyManager.GetAllFoodInfos();
}

std::vector<int> BBGameManager::GetAllSpikyInfos()
{
	return foodSpikyManager.GetAllSpikyInfos();
}

std::vector<int>& BBGameManager::GetAllSporeInfos()
{
	return sporeManager.vecSporeIds;
}

std::vector<int>& BBGameManager::GetAllPlayerIdxs()
{
	return playerManager.playerIds;
}

std::vector<int> BBGameManager::GetAllPlayerNodeIdxs(int playerId)
{
	std::vector<int> ret;
	Player* player = playerManager.GetPlayer(playerId);
	if (player)
	{
		for (auto iter = player->vecPlayerNodes.begin(); iter < player->vecPlayerNodes.end(); iter++)
		{
			ret.emplace_back((*iter)->Idx);
		}
	}
	return ret;
}

void BBGameManager::ClearServerLog()
{
	BBMathUtils::ClearLog();
}

std::vector < std::string> BBGameManager::GetServerLog()
{
	return BBMathUtils::serverLog;
}

int BBGameManager::GetFoodIdxByPos(int pos)
{
	return foodSpikyManager.GetFoodIdxByPos(pos);
}

int BBGameManager::GetFoodPosByIdx(int idx)
{
	return foodSpikyManager.GetFoodPosByIdx(idx);
}

SpikyBall* BBGameManager::GetSpikyInfo(int idx)
{
	return foodSpikyManager.GetSpikyInfo(idx);
}

Spore* BBGameManager::GetSporeInfo(int idx)
{
	return sporeManager.GetSporeInfo(idx);
}

std::vector<int> BBGameManager::GetPlayerNodeIdx(int uid)
{
	std::vector<int> ret;
	if (playerManager.mapPlayers.find(uid) != playerManager.mapPlayers.end())
	{
		Player* player = playerManager.mapPlayers[uid];
		for (PlayerNode* node : player->vecPlayerNodes)
		{
			ret.emplace_back(node->Idx);
		}
	}
	return ret;
}

PlayerNode* BBGameManager::GetPlayerNodeInfo(int idx)
{
	if (playerManager.mapPlayNodes.find(idx) == playerManager.mapPlayNodes.end())
	{
		return NULL;
	}
	PlayerNode* node = playerManager.mapPlayNodes[idx];
	return node;
}

Player* BBGameManager::GetPlayer(int uid)
{
	return playerManager.GetPlayer(uid);
}

std::vector<int> BBGameManager::GetFrameNewPlayerNodeIdxs()
{
	std::vector<int> ret;
	if (frameCacheManager.newPlayerNodes.size() > 0)
	{
		for (auto iter : frameCacheManager.newPlayerNodes)
		{
			std::vector<int>& vecs = iter.second;
			for (int i = 0; i < vecs.size(); i++)
			{
				ret.emplace_back(vecs[i]);
			}
		}
	}
	return ret;
}

std::vector<int>& BBGameManager::GetFrameNewPlayer()
{
	return frameCacheManager.joinPlayerIds;
}

std::vector<int>& BBGameManager::GetFrameNewFood()
{
	return frameCacheManager.newFoodIdxs;
}

std::vector<int>& BBGameManager::GetFrameNewSpiky()
{
	return frameCacheManager.newSpikyIdxs;
}

std::vector<int>& BBGameManager::GetFrameRemovedPlayerNodeIdxs()
{
	return frameCacheManager.frameRemovePlayerNodeIdxs;
}

std::vector<int>& BBGameManager::GetFrameNewSpore()
{
	return frameCacheManager.newSporeIdxs;
}

void BBGameManager::AddOperatePlayerJoin(int uid)
{
	frameCacheManager.AddOperatePlayerJoin(uid);
}

void BBGameManager::AddOperatePlayerSplit(int uid)
{
	frameCacheManager.AddOperatePlayerSplit(uid);
}

void BBGameManager::AddOperateMove(int uid, int angle, int percent)
{
	frameCacheManager.AddOperateMove(uid, angle, percent);
}

void BBGameManager::AddOperatePlayerQuit(int uid)
{
	frameCacheManager.AddOperatePlayerQuit(uid);
}

void BBGameManager::AddOperatePlayerShoot(int uid)
{
	frameCacheManager.AddOperatePlayerShoot(uid);
}

void BBGameManager::AddNewFoodFromServer(int posKey)
{
	foodSpikyManager.ClientAddNewFoodFromServer(posKey, posKey);
}

void BBGameManager::AddNewSpikyFromServer(int idx, int posKey, int mass)
{
	foodSpikyManager.ClientAddNewSpikyFromServer(idx, posKey, mass);
}

void BBGameManager::AddNewSporeFromServer(int idx, int fromId, int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY,
	int curSpeed, int deltaSpeed, int Init, int Cd)
{
	sporeManager.AddNewSporeFromServer(idx, fromId, uid, mass,x, y, directionX, directionY, currentX, currentY, curSpeed, deltaSpeed, Init, Cd);
}

void BBGameManager::AddNewBallFromServer(int idx, int fromId, int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY, int curSpeed, int deltaSpeed, int Init, int Cd, int Protect)
{
	playerManager.AddNewBallFromServer(idx, fromId, uid, mass, x, y, directionX, directionY, currentX, currentY, curSpeed, deltaSpeed, Init, Cd, Protect);
}


void BBGameManager::CreatePlayerFromServer(int uid, int directionX, int directionY, int finalX, int finalY, bool isStopped, int NMass)
{
	playerManager.CreatePlayerFromServer(uid, directionX, directionY, finalX, finalY, isStopped, NMass);
}

int BBGameManager::GetCrc(int playerId)
{
	Player* player = playerManager.GetPlayer(playerId);
	if (player)
	{
		return player->GetCrc();
	}
	return -1;
}

unsigned int BBGameManager::GetAllPlayerCrc()
{
	return playerManager.GetAllPlayerCrc();
}

unsigned int BBGameManager::GetAllFoodCrc()
{
	return foodSpikyManager.GetAllFoodCrc();
}

unsigned int BBGameManager::GetAllSpikyCrc()
{
	return foodSpikyManager.GetAllSpikyCrc();
}

unsigned int BBGameManager::GetAllSporeCrc()
{
	return sporeManager.GetAllSporeCrc();
}