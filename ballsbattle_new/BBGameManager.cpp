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

void BBGameManager::Destory(BBGameManager* manager)
{
	if (manager)
	{
		delete manager;
	}
}

BBGameManager::BBGameManager():
	currentGetServerKeyFrame(0),
	isServer(true),
	canEatFoodSpiky(false),
	canEatNode(false),
	canEatSpore(false),
	userId(0)
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

bool BBGameManager::IsNeedSyncState(unsigned int uid)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		return player->IsNeedSyncState();
	}
	return false;
}

bool BBGameManager::CanSkipSyncState(unsigned int uid)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		return player->CanSkipSyncState();
	}
	return false;
}

void BBGameManager::FrameUpdate()
{
	mGameFrame++;
	frameOutManager.FrameClear();
	playerManager.HandleFrameInputCommand();
	sporeManager.MoveSpores();
	playerManager.UpdatePlayer();
	if (this->IsServer())
	{
		playerManager.PlayerEat();
		playerManager.FinishEat();
		playerManager.DoSpikySplit();
		playerManager.ServerDoJoinPlayer();
		foodSpikyManager.ServerCheckRegenFood();
	}
	frameInManager.FrameClear();
	if (this->IsServer())
	{
		AddNodesCd();
	}
	
	if (!this->IsServer())
	{
		BBMoveManager::ResetFrame();
	}
	
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

SpikyBall* BBGameManager::GetSpikyInfo(int idx)
{
	return foodSpikyManager.GetSpikyInfo(idx);
}

Spore* BBGameManager::GetSpore(int idx)
{
	return sporeManager.GetSpore(idx);
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

BBPlayerNode* BBGameManager::GetPlayerNode(int idx)
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


void BBGameManager::AddPlayerCommand(int uid, int angle, int pressure, bool isSplit, bool isShoot, int id, unsigned int checkSum)
{
	frameInManager.AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, id, checkSum);
}

void BBGameManager::RemovePlayerCommand(unsigned int uid)
{
	auto& playerCommands = frameInManager.playerCommands;
	if (playerCommands.find(uid) != playerCommands.end())
	{
		auto& commands = playerCommands[uid];
		if (commands.size() > 0)
		{
			BBMoveCommand& lastCommand = commands[commands.size() - 1];
			BBPlayer* player = GetPlayer(uid);
			if (player)
			{
				player->predictionData.lastAngle = lastCommand.angle;
				player->predictionData.lastPressure = lastCommand.pressure;
			}
		}
	}
	frameInManager.playerCommands.erase(uid);
}

void BBGameManager::SetNeedUpdatePlayers(std::vector<int> playerIds)
{
	this->needUpdatePlayers.clear();
	for (auto playerId : playerIds)
	{
		this->needUpdatePlayers.emplace(playerId, true);
	}
}

std::vector<int> BBGameManager::GetAllPlayerIdxInRect(int minX, int maxX, int minY, int maxY)
{
	std::vector<int> ret;
	for (auto playerId : playerManager.playerIds)
	{
		BBPlayer* player = playerManager.GetPlayer(playerId);
		if (player)
		{
			BBRect rect = player->GetGroupRect();
			if (rect.intersctsRect(minX, maxX, minY, maxY))
			{
				ret.emplace_back(playerId);
			}
		}
	}
	return ret;
}

int BBGameManager::GetCrcCnt(unsigned int uid)
{
	std::vector<int> ret;
	if (frameOutManager.uid2crcs.find(uid) != frameOutManager.uid2crcs.end())
	{
		ret = frameOutManager.uid2crcs[uid];
	}
	return ret.size();
}

unsigned int BBGameManager::GetCrcIdx(unsigned int uid, int idx)
{
	std::vector<int> ret;
	if (frameOutManager.uid2crcs.find(uid) != frameOutManager.uid2crcs.end())
	{
		ret = frameOutManager.uid2crcs[uid];
	}
	if (ret.size() <= 0)
	{
		return 0;
	}
	if (ret.size() <= idx)
	{
		return ret[ret.size() - 1];
	}
	return ret[idx];
}