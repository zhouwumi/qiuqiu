#include "BBHitManager.h"
#include "BBGameManager.h"
#include "BBMathUtils.h"
#include "BBConst.h"
#include<algorithm>

BBHitManager::BBHitManager()
{
}


BBHitManager::~BBHitManager()
{
}

void BBHitManager::GetCanEatFood(PlayerNode* playerNode, std::vector<int>& vec)
{
	if (!gameManager)
	{
		return;
	}
	if (gameManager->frameCacheManager.RemovedNodes.find(playerNode->Idx) != gameManager->frameCacheManager.RemovedNodes.end())
	{
		return;
	}
	TempVec.clear();
	//gameManager->FoodTree.GetAllHitCircleNodeIdxs(playerNode->treeIndex, TempVec);
	int gripRadius = 0;
	if (playerNode->Protect > 0)
	{
		gripRadius = BBConst::GripRadius;
	}
	BBRect TempPlayerRect;
	TempPlayerRect.setRect(playerNode->minX - gripRadius, playerNode->minY - gripRadius, playerNode->maxX + gripRadius, playerNode->maxY + gripRadius);
	gameManager->foodSpikyManager.FoodGridMap.GetAllHitIdxs(TempPlayerRect, TempVec);
	
	std::sort(TempVec.begin(), TempVec.end());
	for (int i = 0; i < TempVec.size(); i++)
	{
		int idx = TempVec[i];
		if (gameManager->foodSpikyManager.mapFoodIdxs.find(idx) != gameManager->foodSpikyManager.mapFoodIdxs.end())
		{
			Food* food = gameManager->foodSpikyManager.mapFoodIdxs[idx];
			int foodX = food->positionX;
			int foodY = food->positionY;

			int deltaX = playerNode->positionX - foodX;
			int deltaY = playerNode->positionY - foodY;
			if (pow(deltaX, 2) + pow(deltaY, 2) <= pow(playerNode->radius + gripRadius, 2))
			{
				vec.emplace_back(playerNode->Idx);
				vec.emplace_back(idx);
			}
		}
	}
}

void BBHitManager::GetCanEatSpiky(PlayerNode* playerNode, std::vector<int>& vec)
{
	if (gameManager->frameCacheManager.RemovedNodes.find(playerNode->Idx) != gameManager->frameCacheManager.RemovedNodes.end())
	{
		return;
	}
	TempVec.clear();
	BBRect TempPlayerRect;
	TempPlayerRect.setRect(playerNode->minX, playerNode->minY, playerNode->maxX, playerNode->maxY);

	gameManager->foodSpikyManager.SpikyGridMap.GetAllHitIdxs(TempPlayerRect, TempVec);
	std::sort(TempVec.begin(), TempVec.end());
	float delta = (float)BBConst::Delta / BBConst::DeltaBase;
	for (int i = 0; i < TempVec.size(); i++)
	{
		int idx = TempVec[i];
		if (gameManager->foodSpikyManager.mapSpikyBalls.find(idx) != gameManager->foodSpikyManager.mapSpikyBalls.end())
		{
			SpikyBall* ball = gameManager->foodSpikyManager.mapSpikyBalls[idx];
			int foodX = ball->positionX;
			int foodY = ball->positionY;

			int deltaX = playerNode->positionX - foodX;
			int deltaY = playerNode->positionY - foodY;
			if (ball->radius * (1 + delta) < playerNode->radius && pow(deltaX, 2) + pow(deltaY, 2) <= pow(playerNode->radius, 2))
			{
				vec.emplace_back(playerNode->Idx);
				vec.emplace_back(idx);
				vec.emplace_back(ball->mass);
			}
		}
	}
}

void BBHitManager::GetCanEatFood(Player* player, std::vector<int>& vec)
{
	if (!player)
	{
		return;
	}
	for (PlayerNode* node : player->vecPlayerNodes)
	{
		GetCanEatFood(node, vec);
	}
}

void BBHitManager::GetCanEatSpiky(Player* player, std::vector<int>& vec)
{
	if (!player)
	{
		return;
	}
	for (PlayerNode* node : player->vecPlayerNodes)
	{
		GetCanEatSpiky(node, vec);
	}
}

void BBHitManager::GetCanEatFood(int playerId, std::vector<int>& vec)
{
	auto iter = gameManager->playerManager.mapPlayers.find(playerId);
	if (iter == gameManager->playerManager.mapPlayers.end())
	{
		return;
	}
	GetCanEatFood((*iter).second, vec);
}

void BBHitManager::GetCanEatSpiky(int playerId, std::vector<int>& vec)
{
	auto iter = gameManager->playerManager.mapPlayers.find(playerId);
	if (iter == gameManager->playerManager.mapPlayers.end())
	{
		return;
	}
	GetCanEatSpiky((*iter).second, vec);
}

void BBHitManager::GetCanEatNodeOrSpore(PlayerNode* playerNode, std::vector<int>& nodeIdxs, std::vector<int>& sporeIdxs)
{
	if (gameManager->frameCacheManager.RemovedNodes.find(playerNode->Idx) != gameManager->frameCacheManager.RemovedNodes.end())
	{
		return;
	}
	TempVec.clear();
	BBRect TempPlayerRect;
	TempPlayerRect.setRect(playerNode->minX, playerNode->minY, playerNode->maxX, playerNode->maxY);

	gameManager->NodeTree.GetAllHitCircleNodeIdxs(TempPlayerRect, TempVec);
	std::sort(TempVec.begin(), TempVec.end());

	float delta = (float)BBConst::Delta / BBConst::DeltaBase;
	for (int i = 0; i < TempVec.size(); i++)
	{
		int idx = TempVec[i];
		if (gameManager->playerManager.mapPlayNodes.find(idx) != gameManager->playerManager.mapPlayNodes.end())
		{
			if (playerNode->Protect > 0)
			{
				continue;
			}
			PlayerNode* targetNode = gameManager->playerManager.mapPlayNodes[idx];
			_tryEatNode(playerNode, targetNode, nodeIdxs);
		}
		else if (gameManager->sporeManager.mapSpores.find(idx) != gameManager->sporeManager.mapSpores.end())
		{
			Spore* spore = gameManager->sporeManager.mapSpores[idx];
			_tryEatSpore(playerNode, spore, sporeIdxs);
		}
		
	}
}

void BBHitManager::_tryEatSpore(PlayerNode* playerNode, Spore* targetSpore, std::vector<int>& nodeIdxs)
{
	if (targetSpore->FromId == playerNode->Idx && targetSpore->Cd > 0)
	{
		return;
	}

	int foodX = targetSpore->positionX;
	int foodY = targetSpore->positionY;

	int deltaX = playerNode->positionX - foodX;
	int deltaY = playerNode->positionY - foodY;
	if (pow(deltaX, 2) + pow(deltaY, 2) <= pow(playerNode->radius, 2))
	{
		nodeIdxs.emplace_back(playerNode->Idx);
		nodeIdxs.emplace_back(targetSpore->Idx);
		nodeIdxs.emplace_back(targetSpore->mass);

	}
}

void BBHitManager::_tryEatNode(PlayerNode* playerNode, PlayerNode* targetNode, std::vector<int>& nodeIdxs)
{
	if (playerNode->Protect > 0)
	{
		return;
	}
	if (targetNode->Protect > 0)
	{
		return;
	}
	if (playerNode->Idx == targetNode->Idx)
	{
		return;
	}
	auto& removeNodes = gameManager->frameCacheManager.RemovedNodes;
	if (removeNodes.find(targetNode->Idx) != removeNodes.end())
	{
		return;
	}
	if (playerNode->Uid == targetNode->Uid)
	{
		if (playerNode->Cd > 0)
		{
			return;
		}
		if (targetNode->Cd > 0)
		{
			return;
		}
	}

	float delta = (float)BBConst::Delta / BBConst::DeltaBase;
	int foodX = targetNode->positionX;
	int foodY = targetNode->positionY;

	int deltaX = playerNode->positionX - foodX;
	int deltaY = playerNode->positionY - foodY;
	if (targetNode->radius * (1 + delta) < playerNode->radius && pow(deltaX, 2) + pow(deltaY, 2) <= pow(playerNode->radius, 2))
	{
		nodeIdxs.emplace_back(playerNode->Idx);
		nodeIdxs.emplace_back(targetNode->Idx);
		nodeIdxs.emplace_back(targetNode->mass);

		gameManager->frameCacheManager.SetRemoved(targetNode->Idx, playerNode->Idx);
		if (playerNode->Uid == targetNode->Uid)
		{
			gameManager->frameCacheManager.AddCd(playerNode->Idx, playerNode->Uid);
		}
		gameManager->frameCacheManager.AddKiller(targetNode->Uid, playerNode->Uid);
		gameManager->frameCacheManager.AddEatenNode(targetNode->Idx, playerNode->Idx);
	}
}

void BBHitManager::FindFreeSpikyPos(int& returnMass, int& returnX, int& returnY)
{
	BBRect TempRect;
	int mass = BBMathUtils::GetRandom(BBConst::MinSpikyMass, BBConst::MaxSpikyMass);
	int radius = BBMathUtils::Mass2Radius(mass);
	bool hasFind = false;
	while (!hasFind)
	{
		int gridIdx = BBMathUtils::GetRandom(0, gameManager->foodSpikyManager.SpikyGridMap.gridNodes.size() - 1);
		GridNode& gridNode = gameManager->foodSpikyManager.SpikyGridMap.gridNodes[gridIdx];
		if (gridNode.objectIdxs.size() < 2)
		{
			int x = BBMathUtils::GetRandom(gridNode.minX + radius, gridNode.maxX - radius);
			int y = BBMathUtils::GetRandom(gridNode.minY + radius, gridNode.maxY - radius);

			TempRect.setRect(x - radius, y - radius, x + radius, y + radius);
			TempVec.clear();
			gameManager->foodSpikyManager.SpikyGridMap.GetAllHitIdxs(TempRect, TempVec);

			bool hasHit = false;
			if (TempVec.size() > 0) {

				for (int j = 0; j < TempVec.size(); j++)
				{
					int idx = TempVec[j];
					if (gameManager->foodSpikyManager.mapSpikyBalls.find(idx) == gameManager->foodSpikyManager.mapSpikyBalls.end()) {
						//assert("when generate spiky has spiky : tree, but not : map %d", idx);
					}
					else {
						SpikyBall* checkBall = gameManager->foodSpikyManager.mapSpikyBalls[idx];
						if (BBMathUtils::CheckCircleHit(*checkBall, TempRect.centerX, TempRect.centerY, radius))
						{
							hasHit = true;
							break;
						}
					}
				}
			}

			if (!hasHit)
			{
				returnMass = mass;
				returnX = x;
				returnY = y;
				hasFind = true;
			}
		}
	}
}

//好像这个不一定需要一直去寻找
bool BBHitManager::FindFreeFoodPos(int& returnX, int& returnY)
{
	//int gridIdx = BBMathUtils::GetRandom(0, gameManager->FoodGridMap.gridNodes.size() - 1);

	int x = BBMathUtils::GetRandom(1, BBConst::MaxWidth - 1);
	int y = BBMathUtils::GetRandom(1, BBConst::MaxHeight - 1);
	//int posKey = y * BBConst::MaxWidth + x;
	int posKey = BBMathUtils::xy_to_location(x, y);
	if (gameManager->foodSpikyManager.mapFoodPos.find(posKey) == gameManager->foodSpikyManager.mapFoodPos.end())
	{
		returnX = x;
		returnY = y;
		return true;
	}
	else {
		//assert("generate food same position %d_%d", x, y);
		return false;
	}
}


void BBHitManager::FindFreePlayerNodePos(int radius, int& returnX, int& returnY)
{
	BBRect TempRect;
	while (true)
	{
		bool isFirstRandom = true;
		int x = BBMathUtils::GetRandom(1, BBConst::MaxWidth);
		int y = BBMathUtils::GetRandom(1, BBConst::MaxHeight);
		if (isFirstRandom)
		{
			isFirstRandom = false;
		}
		int fixedX, fixedY;
		bool isFixedX, isFixedY;
		BBMathUtils::FixCircle(gameManager->mapRect, x, y, radius, fixedX, fixedY, isFixedX, isFixedY);

		TempRect.setRect(fixedX - radius, fixedY - radius, fixedX + radius, fixedY + radius);
		TempVec.clear();
		bool hasHit = false;

		//检测和刺球,孢子 的碰撞
		//gameManager->ObjectTree.GetAllHitCircleNodeIdxs(TempRect, TempVec);

		gameManager->foodSpikyManager.SpikyGridMap.GetAllHitIdxs(TempRect, TempVec);

		if (TempVec.size() > 0) {
			for (int j = 0; j < TempVec.size(); j++)
			{
				int idx = TempVec[j];
				if (gameManager->foodSpikyManager.mapSpikyBalls.find(idx) == gameManager->foodSpikyManager.mapSpikyBalls.end() && gameManager->mapSpores.find(idx) == gameManager->mapSpores.end()) {
					//assert("add player idx : tree but not : spikyballs or spores %d", idx);
				}
				else {
					BaseCircleNode* ball = gameManager->foodSpikyManager.mapSpikyBalls[idx];
					if (!ball) {
						ball = gameManager->mapSpores[idx];
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
			//检测和其他角色的球的碰撞
			TempVec.clear();
			gameManager->NodeTree.GetAllHitCircleNodeIdxs(TempRect, TempVec);

			if (TempVec.size() > 0) {
				for (int j = 0; j < TempVec.size(); j++)
				{
					int idx = TempVec[j];
					if (gameManager->playerManager.mapPlayers.find(idx) == gameManager->playerManager.mapPlayers.end()) {
						//assert("add player idx : tree but not : mapPlayNodes %d", idx);
					}
					else {
						BaseCircleNode* ball = gameManager->playerManager.mapPlayNodes[idx];
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
			returnX = x;
			returnY = y;
			break;
		}
	}
}