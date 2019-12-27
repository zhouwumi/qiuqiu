#include "BBGameManager.h"
#include "BBConst.h"
#include "BBMathUtils.h"
#include<assert.h>
#include <sstream>
#include<time.h>

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
	gameFrame(0)
{
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
	/*ObjectTree.SetTreeWH(BBConst::MaxWidth, BBConst::MaxHeight);
	ObjectTree.OnBuildFinish(NULL);
	ObjectTree.PreBuildAllSubTrees();*/
	
	/*FoodTree.SetTreeWH(BBConst::MaxWidth, BBConst::MaxHeight);
	FoodTree.OnBuildFinish(NULL);
	FoodTree.PreBuildAllSubTrees();*/
	

	NodeTree.SetTreeWH(BBConst::MaxWidth, BBConst::MaxHeight);
	NodeTree.OnBuildFinish(NULL);
	//NodeTree.PreBuildAllSubTrees();

	spikyRect.setRect(500, 500, BBConst::MaxWidth - 500, BBConst::MaxHeight - 500);
	//spikyRect.setRect(0, 0, BBConst::MaxWidth * 2, BBConst::MaxHeight * 2);

	mapRect.setRect(0, 0, BBConst::MaxWidth, BBConst::MaxHeight);

	eatResults.reserve(100);
}


void BBGameManager::InitFoodSpiky()
{
	foodSpikyManager.InitFoodSpiky();
}

std::vector<int> BBGameManager::ClientGenerateFood(int num)
{
	return foodSpikyManager.ClientGenerateFood(num);
}

std::vector<int> BBGameManager::ClientGenerateSpiky(int num)
{
	return foodSpikyManager.ClientGenerateSpiky(num);
}

bool BBGameManager::ClientFinishGenerateFoodAndSpiky()
{
	return foodSpikyManager.ClientFinishGenerateFoodAndSpiky();
}

//*************食物部分****************//

//int get_cost(timeval time1, timeval time2)
//{
//	return (time1.tv_sec - time2.tv_sec) * 1000000 + (time1.tv_usec - time2.tv_usec);
//}

bool BBGameManager::isKeyFrame()
{
	return gameFrame % BBConst::SyncFPS == 0;
}

//tick 更新
void BBGameManager::OnUpdate()
{
	gameFrame++;
	bool doPush = gameFrame % BBConst::SyncFPS == 0;
	bool doFix = doPush;
	bool isKeyFrame = doPush;

	if (isKeyFrame)
	{
		playerManager.AjustVector();
	}
	eatResults.clear();
	//frameCacheManager.BeginUpdate();
	if (isKeyFrame)
	{
		playerManager.RemoveMass();
	}
	playerManager.MovePlayers(isKeyFrame);
	sporeManager.MoveSpores();
	playerManager.DoEat(eatResults);
	
	if (isKeyFrame)
	{
		playerManager.FinishEat();
		playerManager.DoShoot();
		playerManager.DoSplit();
		playerManager.DoJoin();
		foodSpikyManager.CheckRegenFood();
		_AddNodesCd();
	}
}

void BBGameManager::_AddNodesCd()
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
	if (gameFrame % BBConst::SyncFPS == 0)
	{
		frameCacheManager.OnKeyFrameUpdate();
	}
}

//**********************以下是供测试用的**********************//
std::vector<int> BBGameManager::GetAllFoodInfos()
{
	return foodSpikyManager.GetAllFoodInfos();
}

std::vector<int> BBGameManager::GetAllSpikyInfos()
{
	return foodSpikyManager.GetAllSpikyInfos();
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


std::vector<int> BBGameManager::GetFrameNewPlayerNodeIdxs()
{
	std::vector<int> ret;
	if (frameCacheManager.newPlayerNodes.size() > 0)
	{
		for (auto iter : frameCacheManager.newPlayerNodes)
		{
			ret.emplace_back(iter.first);
			
			std::vector<int> vecs = iter.second;
			for (int i = 0; i < vecs.size(); i++)
			{
				ret.emplace_back(vecs[i]);
			}
			ret.emplace_back(-1);
		}
	}
	return ret;
}

std::vector<int> BBGameManager::GetFrameNewPlayer()
{
	return frameCacheManager.joinPlayerIds;
}

std::vector<int> BBGameManager::GetFrameNewFood()
{
	return frameCacheManager.newFoodIdxs;
}

std::vector<int> BBGameManager::GetFrameNewSpiky()
{
	return frameCacheManager.newSpikyIdxs;
}

std::vector<int> BBGameManager::GetFrameRemovedPlayerNodeIdxs()
{
	return frameCacheManager.frameRemovePlayerNodeIdxs;
}

std::vector<int> BBGameManager::GetFrameNewSpore()
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

void BBGameManager::AddOperateNewFood(int idx, int posKey)
{
	frameCacheManager.AddOperateNewFood(idx, posKey);
}

void BBGameManager::AddOperateNewSpiky(int idx, int posKey, int mass)
{
	frameCacheManager.AddOperateNewSpiky(idx, posKey, mass);
}

void BBGameManager::AddOperatePlayerShoot(int uid)
{
	frameCacheManager.AddOperatePlayerShoot(uid);
}