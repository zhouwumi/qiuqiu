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

void BBHitManager::GetCanEatFood(BBPlayerNode* playerNode, std::vector<int>& vec)
{
	if (!gameManager)
	{
		return;
	}
	if (gameManager->frameOutManager.IsNodeHasBeEat(playerNode->idx))
	{
		return;
	}
	TempVec.clear();
	//gameManager->FoodTree.GetAllHitCircleNodeIdxs(playerNode->treeIndex, TempVec);
	int gripRadius = 0;
	if (playerNode->protect > 0)
	{
		gripRadius = BBConst::GripRadius;
	}
	BBRect TempPlayerRect = playerNode->rect.expand(gripRadius, gripRadius);
	gameManager->foodSpikyManager.FoodGridMap.GetAllHitIdxs(TempPlayerRect, TempVec);
	
	std::sort(TempVec.begin(), TempVec.end());
	for (int i = 0; i < TempVec.size(); i++)
	{
		int idx = TempVec[i];
		auto iter = gameManager->foodSpikyManager.mapFoodIdxs.find(idx);
		if (iter != gameManager->foodSpikyManager.mapFoodIdxs.end())
		{
			Food* food = iter->second;

			int deltaX = playerNode->location.x - food->location.x;
			int deltaY = playerNode->location.y - food->location.y;
			if (pow(deltaX, 2) + pow(deltaY, 2) <= pow(playerNode->GetRadius() + gripRadius, 2))
			{
				vec.emplace_back(idx);
			}
		}
	}
}

void BBHitManager::GetCanEatSpiky(BBPlayerNode* playerNode, std::vector<int>& vec)
{
	if (gameManager->frameOutManager.IsNodeHasBeEat(playerNode->idx))
	{
		return;
	}
	TempVec.clear();
	BBRect TempPlayerRect = playerNode->rect;

	gameManager->foodSpikyManager.SpikyGridMap.GetAllHitIdxs(TempPlayerRect, TempVec);
	std::sort(TempVec.begin(), TempVec.end());
	double delta = (double)BBConst::Delta / BBConst::DeltaBase;
	for (int i = 0; i < TempVec.size(); i++)
	{
		int idx = TempVec[i];
		if (gameManager->foodSpikyManager.mapSpikyBalls.find(idx) != gameManager->foodSpikyManager.mapSpikyBalls.end())
		{
			SpikyBall* ball = gameManager->foodSpikyManager.mapSpikyBalls[idx];
			int deltaX = playerNode->location.x - ball->location.x;
			int deltaY = playerNode->location.y - ball->location.y;
			if (ball->GetRadius() * (1 + delta) < playerNode->GetRadius() && pow(deltaX, 2) + pow(deltaY, 2) <= pow(playerNode->GetRadius(), 2))
			{
				vec.emplace_back(idx);
				vec.emplace_back(ball->GetBallMass());
			}
		}
	}
}

void BBHitManager::GetCanEatFood(BBPlayer* player, std::vector<int>& vec)
{
	if (!player)
	{
		return;
	}
	for (BBPlayerNode* node : player->vecPlayerNodes)
	{
		GetCanEatFood(node, vec);
	}
}

void BBHitManager::GetCanEatSpiky(BBPlayer* player, std::vector<int>& vec)
{
	if (!player)
	{
		return;
	}
	for (BBPlayerNode* node : player->vecPlayerNodes)
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

void BBHitManager::GetCanEatNodeOrSpore(BBPlayerNode* playerNode, std::vector<int>& nodeIdxs, std::vector<int>& sporeIdxs)
{
	if (gameManager->frameOutManager.IsNodeHasBeEat(playerNode->idx))
	{
		return;
	}
	TempVec.clear();
	BBRect TempPlayerRect = playerNode->rect;

	gameManager->NodeTree.GetAllHitCircleNodeIdxs(TempPlayerRect, TempVec);
	std::sort(TempVec.begin(), TempVec.end());

	double delta = (double)BBConst::Delta / BBConst::DeltaBase;
	for (int i = 0; i < TempVec.size(); i++)
	{
		int idx = TempVec[i];
		if (gameManager->playerManager.mapPlayNodes.find(idx) != gameManager->playerManager.mapPlayNodes.end())
		{
			if (playerNode->protect > 0)
			{
				continue;
			}
			BBPlayerNode* targetNode = gameManager->playerManager.mapPlayNodes[idx];
			_tryEatNode(playerNode, targetNode, nodeIdxs);
		}
		else if (gameManager->sporeManager.mapSpores.find(idx) != gameManager->sporeManager.mapSpores.end())
		{
			Spore* spore = gameManager->sporeManager.mapSpores[idx];
			_tryEatSpore(playerNode, spore, sporeIdxs);
		}
		
	}
}

void BBHitManager::_tryEatSpore(BBPlayerNode* playerNode, Spore* targetSpore, std::vector<int>& nodeIdxs)
{
	if (!gameManager->GetCanEatSpore())
	{
		return;
	}
	if (targetSpore->fromId == playerNode->idx && targetSpore->cd > 0)
	{
		return;
	}

	int deltaX = playerNode->location.x - targetSpore->location.x;
	int deltaY = playerNode->location.y - targetSpore->location.y;
	if (pow(deltaX, 2) + pow(deltaY, 2) <= pow(playerNode->GetRadius(), 2))
	{
		nodeIdxs.emplace_back(targetSpore->idx);
		nodeIdxs.emplace_back(targetSpore->GetBallMass());
	}
}

void BBHitManager::_tryEatNode(BBPlayerNode* playerNode, BBPlayerNode* targetNode, std::vector<int>& nodeIdxs)
{
	if (!gameManager->GetCanEatNode())
	{
		return;
	}

	if (playerNode->protect > 0)
	{
		return;
	}
	if (targetNode->protect > 0)
	{
		return;
	}
	if (playerNode->idx == targetNode->idx)
	{
		return;
	}
	if (gameManager->frameOutManager.IsNodeHasBeEat(targetNode->idx))
	{
		return;
	}
	
	if (playerNode->uid == targetNode->uid)
	{
		if (playerNode->cd > 0)
		{
			return;
		}
		if (targetNode->cd > 0)
		{
			return;
		}
	}

	double delta = (double)BBConst::Delta / BBConst::DeltaBase;

	int deltaX = playerNode->location.x - targetNode->location.x;
	int deltaY = playerNode->location.y - targetNode->location.y;
	if (targetNode->GetRadius() * (1 + delta) < playerNode->GetRadius() && pow(deltaX, 2) + pow(deltaY, 2) <= pow(playerNode->GetRadius(), 2))
	{
		nodeIdxs.emplace_back(targetNode->player->uid);
		nodeIdxs.emplace_back(targetNode->idx);
		nodeIdxs.emplace_back(targetNode->GetBallMass());
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
	if (gameManager->foodSpikyManager.mapFoodIdxs.find(posKey) == gameManager->foodSpikyManager.mapFoodIdxs.end())
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
		double fixedX, fixedY;
		BBMathUtils::FixCircle(gameManager->mapRect, x, y, radius, fixedX, fixedY);

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
			returnX = fixedX;
			returnY = fixedY;
			break;
		}
	}
}