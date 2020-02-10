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

//*************食物部分****************//
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
	}
	playerManager.ServerDoJoinPlayer();
	foodSpikyManager.ServerCheckRegenFood();
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

