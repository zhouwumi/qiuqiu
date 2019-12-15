#include "BBGameManager.h"
#include "BBConst.h"
#include "BBMathUtils.h"
#include<assert.h>
#include <sstream>
#include "cocos2d.h"

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
	//spikyRect.setRect(100, 100, 1000, 700);

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

void BBGameManager::RemoveSpikyByIdx(int idx)
{
	if (mapSpikyBalls.find(idx) != mapSpikyBalls.end())
	{
		SpikyBall* ball = mapSpikyBalls[idx];
		mapSpikyBalls.erase(idx);
		ObjectTree.RemoveCircleNode(ball);
		delete ball;
	}
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
		MovePlayer(iter.second);
		vec.clear();
		hitManager.GetCanEatFood(iter.first, vec);
		if (vec.size() > 0)
		{
			for (int i = 0; i < vec.size(); i += 2)
			{
				RemoveFoodByIdx(vec[i + 1]);
				if (eatCallback)
				{
					eatCallback(Type_Food, vec[i + 1], Type_PlayerNode, vec[i]);
				}
			}
		}

		vec.clear();
		hitManager.GetCanEatSpiky(iter.first, vec);
		if (vec.size() > 0)
		{
			for (int i = 0; i < vec.size(); i+=2)
			{
				RemoveSpikyByIdx(vec[i + 1]);
				if (eatCallback)
				{
					eatCallback(Type_Spiky, vec[i + 1], Type_PlayerNode, vec[i]);
				}
			}
		}
	}
}


//玩家移动
void BBGameManager::MovePlayer(Player* player)
{
	std::vector<PlayerNode*>& allPlayerNodes = player->vecPlayerNodes;
	if (allPlayerNodes.size() == 0)
	{
		player->Direction = BBVector::ZERO;
		player->Stopped = true;
		return;
	}
	player->UpdateFinalPoint(player->FinalPoint.x, player->FinalPoint.y);
	BBPoint finalPoint = player->FinalPoint;

	for (int i = 0; i < allPlayerNodes.size(); i++)
	{
		PlayerNode* node = allPlayerNodes[i];
		BBPoint locVec = node->Location;
		if (node->Init > 0)
		{
			BBVector moveVec = node->InitMove();
			locVec.x += moveVec.x;
			locVec.y += moveVec.y;
		}
		else
		{
			if (node->Current != BBVector::ZERO)
			{
				locVec.x += node->Current.x;
				locVec.y += node->Current.y;
				if (node->FromLocation.x <= finalPoint.x && locVec.x >= finalPoint.x)
				{
					locVec.x = finalPoint.x;
				}
				if (node->FromLocation.x >= finalPoint.x && locVec.x <= finalPoint.x)
				{
					locVec.x = finalPoint.x;
				}

				if (node->FromLocation.y <= finalPoint.y && locVec.y >= finalPoint.y)
				{
					locVec.y = finalPoint.y;
				}
				if (node->FromLocation.y >= finalPoint.y && locVec.y <= finalPoint.y)
				{
					locVec.y = finalPoint.y;
				}
			}
		}

		bool isFixedX, isFixedY;
		int fixedX, fixedY;
		BBMathUtils::FixCircle(mapRect, locVec.x, locVec.y, node->radius, fixedX, fixedY, isFixedX, isFixedY);
		node->ChangePosition(fixedX, fixedY);
		NodeTree.UpdateCircleNode(node);
	}

	for (int i = 0; i < player->vecPlayerNodes.size(); i++)
	{
		PlayerNode* node = player->vecPlayerNodes[i];
		if (node->Init > 0)
		{
			node->Init -= 1;
			if (node->Init == 0)
			{
				node->SetSpeedVec(player->FinalPoint.x - node->Location.x, player->FinalPoint.y - node->Location.y);
				node->Delta = BBVector::ZERO;
				node->Final = node->Current;
			}
		}
		if (node->Cd > 0)
		{
			node->Cd -= 1;
		}
		if (node->Protect > 0)
		{
			node->Protect -= 1;
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

std::vector<int> BBGameManager::GetAllSpikyInfos()
{
	std::vector<int> ret;
	for each (auto iter in mapSpikyBalls)
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

SpikyBall* BBGameManager::GetSpikyInfo(int idx)
{
	auto iter = mapSpikyBalls.find(idx);
	if (iter == mapSpikyBalls.end())
	{
		return NULL;
	}
	return (*iter).second;
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

//测试代码
void BBGameManager::CheckSpikyHit(int idx1, int idx2)
{
	SpikyBall* ball_1 = mapSpikyBalls[idx1];
	SpikyBall* ball_2 = mapSpikyBalls[idx2];
	hitManager.CheckHitSpiky(ball_1, ball_2);
}

void BBGameManager::DebugPrintLog()
{
	std::vector<const QuadTree*> trees;
	trees.emplace_back(&ObjectTree);

	std::vector<const QuadTree*> copyTrees;
	while (trees.size() > 0)
	{
		for (int i = 0; i < trees.size(); i++)
		{
			const QuadTree* tree = trees[i];

			for each (const QuadTree& subTree in tree->subTrees)
			{
				copyTrees.emplace_back(&subTree);
			}

			std::ostringstream stream1;
			for each (auto node in tree->rootNode.subNodes)
			{
				stream1 << node->Idx << ",";
			}

			std::ostringstream stream2;
			stream2 << tree->treeIdx;

			const QuadTree* tempTree = tree;
			while (!tempTree->isRoot)
			{
				stream2 << " <-- " << tempTree->parentTree->treeIdx;
				tempTree = tempTree->parentTree;
			}
			
			std::string str1 = stream1.str();
			std::string str2 = stream2.str();
			
			cocos2d::log("nodes = %s , treeId = %s", str1.c_str(),  str2.c_str());
		}

		trees.clear();
		trees = std::move(copyTrees);
		copyTrees.clear();
	}
}

void BBGameManager::StartMovePlayer(int uid)
{
	if (mapPlayers.find(uid) == mapPlayers.end())
	{
		return;
	}
	Player* player = mapPlayers[uid];
	for (int i = 0; i < player->vecPlayerNodes.size(); i++)
	{
		player->vecPlayerNodes[i]->Current.SetPoint(5, 5);
	}
}