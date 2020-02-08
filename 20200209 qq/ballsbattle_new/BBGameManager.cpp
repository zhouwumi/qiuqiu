#include "BBGameManager.h"
#include "BBConst.h"
#include "BBMathUtils.h"
#include <sstream>
#include "BBMoveManager.h"

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
	canEatFoodSpiky(false),
	canEatNode(false),
	canEatSpore(false)
{
	BBMathUtils::init_crc_table();
	hitManager.SetGameManager(this);
	playerManager.SetGameManager(this);
	foodSpikyManager.SetGameManager(this);
	sporeManager.SetGameManager(this);
	objectManager.SetGameManager(this);
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
void BBGameManager::FrameUpdate()
{
	frameOutManager.FrameClear();
	playerManager.HandleFrameInputCommand();
	playerManager.UpdatePlayer();
	if (this->IsServer())
	{
		playerManager.PlayerEat();
		playerManager.FinishEat();
	}
	playerManager.ServerDoJoinPlayer();
	frameInManager.FrameClear();
	
	BBMoveManager::ResetFrame();
	/*playerManager.PlayerEat();*/
	//playerManager.FinishEat();
	//playerManager.HandlePlayerQuit();
	//AddNodesCd();
	/*gameFrame++;
	eatResults.clear();
	sporeManager.MoveSpores();
	playerManager.MovePlayers();
	playerManager.DoEat(eatResults);
	playerManager.RemoveMass();
	playerManager.FinishEat();
	playerManager.DoSpikySplit();
	playerManager.AjustVector();
	playerManager.KeyFrameUpdatePlayer();
	playerManager.DoShoot();
	playerManager.DoPlayerSplit();

	playerManager.DoJoin();
	foodSpikyManager.ServerCheckRegenFood();
	AddNodesCd();*/

}


void BBGameManager::AddNodesCd()
{
	auto InCdNodes = frameOutManager.markAddNodeCds;
	for (auto iter : InCdNodes)
	{
		int nodeIdx = iter.first;
		int playerId = iter.second;

		if (playerManager.mapPlayNodes.find(nodeIdx) != playerManager.mapPlayNodes.end())
		{
			BBPlayerNode* playerNode = playerManager.mapPlayNodes[nodeIdx];
			playerNode->ChangeCd(BBConst::MergeCD);
		}
	}
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
	BBPlayer* player = playerManager.GetPlayer(playerId);
	if (player)
	{
		for (auto iter = player->vecPlayerNodes.begin(); iter < player->vecPlayerNodes.end(); iter++)
		{
			ret.emplace_back((*iter)->idx);
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
		BBPlayer* player = playerManager.mapPlayers[uid];
		for (BBPlayerNode* node : player->vecPlayerNodes)
		{
			ret.emplace_back(node->idx);
		}
	}
	return ret;
}

BBPlayerNode* BBGameManager::GetPlayerNodeInfo(int idx)
{
	if (playerManager.mapPlayNodes.find(idx) == playerManager.mapPlayNodes.end())
	{
		return NULL;
	}
	BBPlayerNode* node = playerManager.mapPlayNodes[idx];
	return node;
}

BBPlayer* BBGameManager::GetPlayer(int uid)
{
	return playerManager.GetPlayer(uid);
}

std::vector<int> BBGameManager::GetFrameNewPlayerNodeIdxs()
{
	std::vector<int> ret;
	if (frameOutManager.newPlayerNodes.size() > 0)
	{
		for (auto iter : frameOutManager.newPlayerNodes)
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
	return frameOutManager.joinPlayerIds;
}

std::vector<int>& BBGameManager::GetFrameNewFood()
{
	return frameOutManager.newFoodIdxs;
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
	frameInManager.AddPlayerJoin(uid);
}

void BBGameManager::AddPlayerQuit(int uid)
{
	frameInManager.AddPlayerQuit(uid);
}

void BBGameManager::AddPlayerCommand(int uid, int angle, int pressure, bool isSplit, bool isShoot, int id, unsigned int checkSum)
{
	frameInManager.AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, id, checkSum);
}

bool BBGameManager::CanAddPlayerCommand(int uid)
{
	if (IsServer())
	{
		return true;
	}
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		return !player->mCommandList.isBacklogged();
	}
	return false;
}

void BBGameManager::AddNewFoodFromServer(int posKey)
{
	foodSpikyManager.ClientAddNewFoodFromServer(posKey);
}

void BBGameManager::AddNewSpikyFromServer(unsigned int idx, int posKey, int mass)
{
	foodSpikyManager.ClientAddNewSpikyFromServer(idx, posKey, mass);
}

void BBGameManager::AddNewSporeFromServer(unsigned int idx, unsigned int fromId, unsigned int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY,
	int curSpeed, int deltaSpeed, int initStopFrame, int cd)
{
	sporeManager.AddNewSporeFromServer(idx, fromId, uid, mass,x, y, directionX, directionY, currentX, currentY, curSpeed, deltaSpeed, initStopFrame, cd);
}

void BBGameManager::CreatePlayerNodeFromServer(unsigned int uid, unsigned int idx, unsigned int fromId, int x, int y, int mass, int cd, int protect, int initStopFrame, int initSpeed, int initDeltaSpeed)
{
	playerManager.CreatePlayerNodeFromServer(uid, idx, fromId, x, y, mass, cd, protect,initStopFrame, initSpeed, initDeltaSpeed);
}

void BBGameManager::RemovePlayerNodeFromServer(unsigned int uid, unsigned int idx)
{
	playerManager.RemovePlayerNode(uid, idx);
}

void BBGameManager::CreatePlayerFromServer(unsigned int uid, unsigned int idx, int directionX, int directionY, int NMass, int nextNodeIdx)
{
	playerManager.CreatePlayerFromServer(uid, idx, directionX, directionY, NMass, nextNodeIdx);
}

bool BBGameManager::IsPlayerCommandMatch(unsigned int uid)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		return !player->mCommandList.mControlMismatch;
	}
	return false;
}

unsigned int BBGameManager::GetAckCommand(unsigned int uid)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		return player->mCommandList.mLastCommandAck;
	}
	return 0;
}

void BBGameManager::SyncServerAckCommand(unsigned int uid,int ackCommand)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		player->mCommandList.clientSyncServerCommandAck(ackCommand);
	}
}

void BBGameManager::ClientSyncServerPlayerInfo(unsigned int uid, int nextNodeIdx, int nMass, int directionX, int directionY, int ackCommand)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		player->ClientSyncServerPlayerInfo(nextNodeIdx, nMass, directionX, directionY, ackCommand);
	}
}

void BBGameManager::CalcBallDelta(unsigned int uid)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		player->CalcBallDelta();
	}
}

void BBGameManager::ClientCatchup(unsigned int uid)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		player->clientCatchup();
	}
}

void BBGameManager::ClientSyncServerPlayerNode(unsigned int uid, int idx, int fromId, int x, int y, int mass, int cd, int protect, int initStopFrame, int initSpeed, int initDeltaSpeed)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		BBPlayerNode* node = player->GetPlayerNode(idx);
		if (node)
		{
			node->ClientSyncServerPlayerNode(fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed);
		}
	}
}