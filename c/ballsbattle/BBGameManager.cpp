#include "BBGameManager.h"
#include "BBConst.h"
#include "BBMathUtils.h"
#include<assert.h>

unsigned int _start_idx = 1;
unsigned int _generate_next_idx()
{
	return _start_idx++;
}

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
BBGameManager::BBGameManager():
	_curObjIdx(1)
{
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

	for each (auto iter in mapFoods)
	{
		delete iter.second;
		iter.second = NULL;
	}
	mapFoods.clear();

	for each (auto iter in mapSpikyBalls)
	{
		delete iter.second;
		iter.second = NULL;
	}
	mapSpikyBalls.clear();
}

unsigned int BBGameManager::_getNextObjIdx()
{
	return _curObjIdx++;
}

void BBGameManager::InitRoom()
{
	ObjectTree.SetTreeWH(BBConst::MaxWidth, BBConst::MaxHeight);
	ObjectTree.OnBuildFinish(NULL);
	FoodTree.SetTreeWH(BBConst::MaxWidth, BBConst::MaxHeight);
	FoodTree.OnBuildFinish(NULL);
	NodeTree.SetTreeWH(BBConst::MaxWidth, BBConst::MaxHeight);
	NodeTree.OnBuildFinish(NULL);

	spikyRect.setRect(500, 500, BBConst::MaxWidth - 500, BBConst::MaxHeight - 500);

	mapRect.setRect(0, 0, BBConst::MaxWidth, BBConst::MaxHeight);
}


void BBGameManager::InitFood()
{
	BBRect TempRect;
	std::vector<int> TempVec;

	//刺球
	for (int i = 0; i < BBConst::SpikyNum; i++)
	{
		int mass = BBMathUtils::GetRandom(BBConst::MinSpikyMass, BBConst::MaxSpikyMass);
		int x = BBMathUtils::GetRandom(spikyRect.minX, spikyRect.maxX);
		int y = BBMathUtils::GetRandom(spikyRect.minY, spikyRect.maxY);
		int radius = BBMathUtils::Mass2Radius(mass);
		int fixedX, fixedY;
		bool isFixedX, isFixedY;
		BBMathUtils::FixCircle(spikyRect, x, y, radius, fixedX, fixedY, isFixedX, isFixedY);
		TempRect.setRect(fixedX - radius, fixedY - radius, fixedX + radius, fixedY + radius);
		TempVec.clear();
		ObjectTree.GetAllHitCircleNodeIdxs(TempRect, TempVec);

		bool hasHit = false;
		if (TempVec.size() > 0) {
			
			for (int j = 0; j < TempVec.size(); j++)
			{
				int idx = TempVec[j];
				if (mapSpikyBalls.find(idx) == mapSpikyBalls.end()) {
					assert("when generate spiky has spiky in tree, but not in map %d", idx);
				}
				else {
					SpikyBall* checkBall = mapSpikyBalls[idx];
					if (BBMathUtils::CheckCircleHit(*checkBall, TempRect.centerX, TempRect.centerY, radius))
					{
						hasHit = true;
						i--;
						break;
					}
				}
				
			}
		}
		if (!hasHit)
		{
			SpikyBall* newSpikyBall = new SpikyBall();
			newSpikyBall->SetMass(mass);

			newSpikyBall->ChangePosition(fixedX, fixedY, newSpikyBall->radius);
			newSpikyBall->Idx = _getNextObjIdx();

			mapSpikyBalls.emplace(newSpikyBall->Idx, newSpikyBall);
			ObjectTree.AddCircleNode(newSpikyBall);
		}
	}

	//食物
	for (int i = 0; i < BBConst::FoodNum; i++)
	{
		int x = BBMathUtils::GetRandom(1, BBConst::MaxWidth - 1);
		int y = BBMathUtils::GetRandom(1, BBConst::MaxHeight - 1);
		int key = y * BBConst::MaxWidth + x;
		if (mapFoods.find(key) == mapFoods.end())
		{
			AddFoodByKey(key, x, y);
		}
		else {
			assert("generate food same position %d_%d", x, y);
		}
	}
}

PlayerNode*  BBGameManager::SpawnPlayerNode()
{
	PlayerNode* newPlayerNode = new PlayerNode();
	newPlayerNode->Idx = _generate_next_idx();
	mapPlayNodes.emplace(newPlayerNode->Idx, newPlayerNode);
	return newPlayerNode;
}

/**食物部分**/
void BBGameManager::AddFoodByKey(int key, int x, int y)
{
	Food* newFood = new Food();
	newFood->Idx = key;
	newFood->radius = 0;
	newFood->ChangePosition(x, y, 0);
	mapFoods.emplace(key, newFood);
	FoodTree.AddCircleNode(newFood);
}

/**玩家部分**/
void BBGameManager::AddPlayer(int uid)
{
	if (mapPlayers.find(uid) == mapPlayers.end())
	{
		Player* player = new Player(uid);
		mapPlayers.emplace(uid, player);
		AddPlayerNode(uid);
		PlayerNode* node = player->vecPlayerNodes[0];
		player->ResetPoint(node->Location.x, node->Location.y);
	}
	else
	{
		Player* player = mapPlayers[uid];
		if (player->vecPlayerNodes.size() == 0)
		{
			AddPlayerNode(uid);
			PlayerNode* node = player->vecPlayerNodes[0];
			player->ResetPoint(node->Location.x, node->Location.y);
		}
	}
}

void BBGameManager::AddPlayerNode(int uid)
{
	Player* player = mapPlayers[uid];
	if (!player)
	{
		assert("add player node but player is not in map");
		return;
	}
	PlayerNode* newPlayerNode = new PlayerNode();
	newPlayerNode->Uid = uid;
	newPlayerNode->SetMass(BBConst::InitMass);
	newPlayerNode->Idx = _getNextObjIdx();
	newPlayerNode->FromLocation.SetPoint(0, 0);
	newPlayerNode->Current.SetPoint(0, 0);
	newPlayerNode->Delta.SetPoint(0, 0);
	newPlayerNode->Final.SetPoint(0, 0);
	newPlayerNode->Direction.SetPoint(0, 0);
	newPlayerNode->Cd = 0;
	player->vecPlayerNodes.emplace_back(newPlayerNode);
	mapPlayNodes.emplace(newPlayerNode->Idx, newPlayerNode);
	
	int radius = newPlayerNode->radius;
	BBRect TempRect;
	std::vector<int> TempVec;

	bool isFirstRandom = true;
	while (true)
	{
		int x = BBMathUtils::GetRandom(1, BBConst::MaxWidth);
		int y = BBMathUtils::GetRandom(1, BBConst::MaxHeight);
		if (isFirstRandom)
		{
			x = 200;
			y = 200;
			isFirstRandom = false;
		}
		int fixedX, fixedY;
		bool isFixedX, isFixedY;
		BBMathUtils::FixCircle(mapRect, x, y, radius, fixedX, fixedY, isFixedX, isFixedY);

		TempRect.setRect(fixedX - radius, fixedY - radius, fixedX + radius, fixedY + radius);
		TempVec.clear();
		ObjectTree.GetAllHitCircleNodeIdxs(TempRect, TempVec);

		bool hasHit = false;
		if (TempVec.size() > 0) {
			for (int j = 0; j < TempVec.size(); j++)
			{
				int idx = TempVec[j];
				if (mapSpikyBalls.find(idx) == mapSpikyBalls.end() && mapSpores.find(idx) == mapSpores.end()) {
					assert("add player idx in tree but not in spikyballs or spores %d", idx);
				}
				else {
					BaseCircleNode* ball = mapSpikyBalls[idx];
					if (!ball) {
						ball = mapSpores[idx];
					}
					if (ball && BBMathUtils::CheckCircleHit(*ball, TempRect.centerX, TempRect.centerY, radius))
					{
						hasHit = true;
						break;
					}
				}
			}
		}
		if (!hasHit)
		{
			TempVec.clear();
			NodeTree.GetAllHitCircleNodeIdxs(TempRect, TempVec);

			if (TempVec.size() > 0) {
				for (int j = 0; j < TempVec.size(); j++)
				{
					int idx = TempVec[j];
					if (mapPlayNodes.find(idx) == mapPlayNodes.end()) {
						assert("add player idx in tree but not in mapPlayNodes %d", idx);
					}
					else {
						BaseCircleNode* ball = mapPlayNodes[idx];
						if (ball && BBMathUtils::CheckCircleHit(*ball, TempRect.centerX, TempRect.centerY, radius))
						{
							hasHit = true;
							break;
						}
					}
				}
			}
		}
		if (!hasHit)
		{
			newPlayerNode->ChangePosition(x, y, radius);
			newPlayerNode->FromLocation = newPlayerNode->Location;
			break;
		}
	}
	NodeTree.AddCircleNode(newPlayerNode);
	_updatePlayerCenter(uid);
}

void BBGameManager::_updatePlayerCenter(int uid)
{

}