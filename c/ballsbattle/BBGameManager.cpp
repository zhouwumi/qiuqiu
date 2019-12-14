#include "BBGameManager.h"
#include "BBConst.h"
#include "BBMathUtils.h"
#include<assert.h>

BBGameManager* BBGameManager::s_sharedGameManager = nullptr;


BBGameManager* BBGameManager::getInstance()
{
	if (s_sharedGameManager == nullptr)
	{
		s_sharedGameManager = new BBGameManager();
	}
	return s_sharedGameManager;
}

void BBGameManager::destroyInstance()
{
	if (s_sharedGameManager)
	{
		delete s_sharedGameManager;
		s_sharedGameManager = nullptr;
	}
}
BBGameManager::BBGameManager()
{
	hitManager.SetGameManager(this);
}

BBGameManager::~BBGameManager()
{
	for each (auto iter in mapPlayNodes)
	{
		delete iter.second;
		iter.second = NULL;
	}
	mapPlayNodes.clear();

	for each (auto iter in mapPlayers)
	{
		delete iter.second;
		iter.second = NULL;
	}
	mapPlayers.clear();

	for each (auto iter in mapSpores)
	{
		delete iter.second;
		iter.second = NULL;
	}
	mapSpores.clear();

	for each (auto iter in mapFoodIdxs)
	{
		delete iter.second;
		iter.second = NULL;
	}
	mapFoodIdxs.clear();
	mapFoodPos.clear();

	for each (auto iter in mapSpikyBalls)
	{
		delete iter.second;
		iter.second = NULL;
	}
	mapSpikyBalls.clear();
}

void BBGameManager::InitRoom()
{
	ObjectTree.SetTreeWH(BBConst::MaxWidth, BBConst::MaxHeight);
	ObjectTree.OnBuildFinish(NULL);
	ObjectTree.PreBuildAllSubTrees();
	FoodTree.SetTreeWH(BBConst::MaxWidth, BBConst::MaxHeight);
	FoodTree.OnBuildFinish(NULL);
	FoodTree.PreBuildAllSubTrees();
	NodeTree.SetTreeWH(BBConst::MaxWidth, BBConst::MaxHeight);
	NodeTree.OnBuildFinish(NULL);
	NodeTree.PreBuildAllSubTrees();

	spikyRect.setRect(500, 500, BBConst::MaxWidth - 500, BBConst::MaxHeight - 500);

	mapRect.setRect(0, 0, BBConst::MaxWidth, BBConst::MaxHeight);
}


void BBGameManager::InitFood()
{
	//刺球
	for (int i = 0; i < BBConst::SpikyNum; i++)
	{
		CreateSpiky();
	}

	//食物
	for (int i = 0; i < BBConst::FoodNum; i++)
	{
		CreateFood();
	}
}

PlayerNode*  BBGameManager::SpawnPlayerNode()
{
	PlayerNode* newPlayerNode = new PlayerNode();
	newPlayerNode->Idx = objectManager._getNextObjIdx();
	mapPlayNodes.emplace(newPlayerNode->Idx, newPlayerNode);
	return newPlayerNode;
}

//**************刺球部分***************//
void BBGameManager::CreateSpiky()
{
	int x, y;
	int mass;
	hitManager.FindFreeSpikyPos(mass, x, y);

	SpikyBall* newSpikyBall = objectManager.CreateSpikyBall(x, y, mass);
	mapSpikyBalls.emplace(newSpikyBall->Idx, newSpikyBall);
	ObjectTree.AddCircleNode(newSpikyBall);
}

//*************食物部分****************//
void BBGameManager::CreateFood()
{	
	int x, y;
	if (hitManager.FindFreeFoodPos(x, y))
	{
		Food* food = objectManager.CreateFood(x, y);
		mapFoodPos.emplace(food->posKey, food);
		mapFoodIdxs.emplace(food->Idx, food);
		FoodTree.AddCircleNode(food);
	}
}

void BBGameManager::RemoveFoodByIdx(int idx)
{
	if (mapFoodIdxs.find(idx) != mapFoodIdxs.end())
	{
		Food* food = mapFoodIdxs[idx];
		int posKey = food->posKey;
		mapFoodIdxs.erase(idx);
		mapFoodPos.erase(posKey);
		FoodTree.RemoveCircleNode(food);
		delete food;
	}
}

void BBGameManager::RemoveFoodByPos(int posKey)
{
	if (mapFoodPos.find(posKey) != mapFoodPos.end())
	{
		Food* food = mapFoodPos[posKey];
		int idx = food->Idx;
		mapFoodIdxs.erase(idx);
		mapFoodPos.erase(posKey);
		FoodTree.RemoveCircleNode(food);
		delete food;
	}
}

/**玩家部分**/
void BBGameManager::CreatePlayer(int uid)
{
	if (mapPlayers.find(uid) == mapPlayers.end())
	{
		Player* player = objectManager.CreatePlayer(uid);
		mapPlayers.emplace(uid, player);
		CreatePlayerNode(uid);
		PlayerNode* node = player->vecPlayerNodes[0];
		player->ResetPoint(node->Location.x, node->Location.y);
	}
	else
	{
		Player* player = mapPlayers[uid];
		if (player->vecPlayerNodes.size() == 0)
		{
			CreatePlayerNode(uid);
			PlayerNode* node = player->vecPlayerNodes[0];
			player->ResetPoint(node->Location.x, node->Location.y);
		}
	}
}


void BBGameManager::CreatePlayerNode(int uid)
{
	Player* player = mapPlayers[uid];
	if (!player)
	{
		assert("add player node but player is not in map");
		return;
	}
	PlayerNode* newPlayerNode = objectManager.CreatePlayerNode(uid, BBConst::InitMass);
	int radius = newPlayerNode->radius;
	int x, y;
	hitManager.FindFreePlayerNodePos(radius, x, y);
	newPlayerNode->ChangePosition(x, y);
	newPlayerNode->FromLocation = newPlayerNode->Location;

	player->vecPlayerNodes.emplace_back(newPlayerNode);
	mapPlayNodes.emplace(newPlayerNode->Idx, newPlayerNode);
	NodeTree.AddCircleNode(newPlayerNode);
	_updatePlayerCenter(uid);
}

void BBGameManager::RemovePlayer(int uid)
{
	Player* player = mapPlayers[uid];
	if (!player)
	{
		return;
	}
	
	for (int i = player->vecPlayerNodes.size() - 1; i >= 0; i--)
	{
		RemovePlayerNode(uid, player->vecPlayerNodes[i]->Idx);
	}
	
	player->vecPlayerNodes.clear();
	mapPlayers.erase(uid);
	delete player;
}

void BBGameManager::RemovePlayerNode(int uid, int nodeIdx, bool removeFromVec)
{
	if (mapPlayNodes.find(nodeIdx) == mapPlayNodes.end())
	{
		assert("remove player node but not in map");
		return;
	}
	PlayerNode* node = mapPlayNodes[nodeIdx];
	mapPlayNodes.erase(nodeIdx);
	NodeTree.RemoveCircleNode(node);

	if (removeFromVec)
	{
		Player* player = mapPlayers[uid];
		if (!player)
		{
			return;
		}
		for (int i = player->vecPlayerNodes.size() - 1; i >= 0; i--)
		{
			if (player->vecPlayerNodes[i] == node)
			{
				player->vecPlayerNodes.erase(player->vecPlayerNodes.begin() + i);
				break;
			}
		}
	}
	delete node;
}

void BBGameManager::_updatePlayerCenter(int uid)
{

}

//tick 更新
void BBGameManager::OnUpdate()
{
	std::vector<int> vec;
	for each (auto iter in mapPlayers)
	{
		hitManager.GetCanEatFood(iter.first, vec);
		if (vec.size() > 0)
		{
			for (int i = 0; i < vec.size(); i++)
			{
				RemoveFoodByIdx(vec[i]);
				if (eatFoodCallback)
				{
					eatFoodCallback(vec[i]);
				}
			}
		}
	}
}

//**********************以下是供测试用的**********************//
std::vector<int> BBGameManager::GetAllFoodInfos()
{
	std::vector<int> ret;
	for each (auto iter in mapFoodPos)
	{
		ret.emplace_back(iter.first);
	}
	return ret;
}

int BBGameManager::GetFoodIdxByPos(int pos)
{
	return mapFoodPos[pos]->Idx;
}

int BBGameManager::GetFoodPosByIdx(int idx)
{
	return mapFoodIdxs[idx]->posKey;
}

std::vector<int> BBGameManager::GetPlayerNodeIdx(int uid)
{
	std::vector<int> ret;
	
	if (mapPlayers.find(uid) != mapPlayers.end())
	{
		Player* player = mapPlayers[uid];
		for each (PlayerNode* node in player->vecPlayerNodes)
		{
			ret.emplace_back(node->Idx);
		}
	}
	return ret;
}

PlayerNode* BBGameManager::GetPlayerNodeInfo(int idx)
{
	if (mapPlayNodes.find(idx) == mapPlayNodes.end())
	{
		return NULL;
	}
	PlayerNode* node = mapPlayNodes[idx];
	return node;
}

std::vector<int> BBGameManager::CheckPlayerHit(int uid)
{
	std::vector<int> allHitNodes;
	hitManager.GetCanEatFood(uid, allHitNodes);
	return allHitNodes;
}

void BBGameManager::ChangePlayerNodePos(int idx, int x, int y)
{
	if (mapPlayNodes.find(idx) == mapPlayNodes.end())
	{
		return;
	}
	PlayerNode* node = mapPlayNodes[idx];
	node->ChangePosition(x, y, node->radius);
	NodeTree.UpdateCircleNode(node);
}
