#include "BBHitManager.h"
#include "BBGameManager.h"
#include "BBMathUtils.h"
#include "BBConst.h"
#include<assert.h>

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
	TempVec.clear();
	gameManager->FoodTree.GetAllHitCircleNodeIdxs(playerNode->treeIndex, TempVec);
	for (int i = 0; i < TempVec.size(); i++)
	{
		int idx = TempVec[i];
		if (gameManager->mapFoodIdxs.find(idx) != gameManager->mapFoodIdxs.end())
		{
			Food* food = gameManager->mapFoodIdxs[idx];
			int foodX = food->positionX;
			int foodY = food->positionY;

			int deltaX = playerNode->positionX - foodX;
			int deltaY = playerNode->positionY - foodY;
			if (pow(deltaX, 2) + pow(deltaY, 2) <= pow(playerNode->radius, 2))
			{
				vec.emplace_back(playerNode->Idx);
				vec.emplace_back(idx);
			}
		}
	}
}

void BBHitManager::GetCanEatSpiky(PlayerNode* playerNode, std::vector<int>& vec)
{
	TempVec.clear();
	gameManager->ObjectTree.GetAllHitCircleNodeIdxs(playerNode->treeIndex, TempVec);
	for (int i = 0; i < TempVec.size(); i++)
	{
		int idx = TempVec[i];
		if (gameManager->mapSpikyBalls.find(idx) != gameManager->mapSpikyBalls.end())
		{
			SpikyBall* ball = gameManager->mapSpikyBalls[idx];
			int foodX = ball->positionX;
			int foodY = ball->positionY;

			int deltaX = playerNode->positionX - foodX;
			int deltaY = playerNode->positionY - foodY;
			if (pow(deltaX, 2) + pow(deltaY, 2) <= pow(playerNode->radius - ball->radius, 2))
			{
				vec.emplace_back(playerNode->Idx);
				vec.emplace_back(idx);
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
	for each (PlayerNode* node in player->vecPlayerNodes)
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
	for each (PlayerNode* node in player->vecPlayerNodes)
	{
		GetCanEatSpiky(node, vec);
	}
}

void BBHitManager::GetCanEatFood(int playerId, std::vector<int>& vec)
{
	auto iter = gameManager->mapPlayers.find(playerId);
	if (iter == gameManager->mapPlayers.end())
	{
		return;
	}
	GetCanEatFood((*iter).second, vec);
}

void BBHitManager::GetCanEatSpiky(int playerId, std::vector<int>& vec)
{
	auto iter = gameManager->mapPlayers.find(playerId);
	if (iter == gameManager->mapPlayers.end())
	{
		return;
	}
	GetCanEatSpiky((*iter).second, vec);
}

void BBHitManager::FindFreeSpikyPos(int& returnMass, int& returnX, int& returnY)
{
	BBRect TempRect;
	int mass = BBMathUtils::GetRandom(BBConst::MinSpikyMass, BBConst::MaxSpikyMass);
	int radius = BBMathUtils::Mass2Radius(mass);
	bool hasFind = false;
	while (!hasFind)
	{
		int x = BBMathUtils::GetRandom(gameManager->spikyRect.minX, gameManager->spikyRect.maxX);
		int y = BBMathUtils::GetRandom(gameManager->spikyRect.minY, gameManager->spikyRect.maxY);

		int fixedX, fixedY;
		bool isFixedX, isFixedY;
		BBMathUtils::FixCircle(gameManager->spikyRect, x, y, radius, fixedX, fixedY, isFixedX, isFixedY);
		TempRect.setRect(fixedX - radius, fixedY - radius, fixedX + radius, fixedY + radius);
		TempVec.clear();
		gameManager->ObjectTree.GetAllHitCircleNodeIdxs(TempRect, TempVec);
		bool hasHit = false;
		if (TempVec.size() > 0) {
			
			for (int j = 0; j < TempVec.size(); j++)
			{
				int idx = TempVec[j];
				if (gameManager->mapSpikyBalls.find(idx) == gameManager->mapSpikyBalls.end()) {
					assert("when generate spiky has spiky in tree, but not in map %d", idx);
				}
				else {
					SpikyBall* checkBall = gameManager->mapSpikyBalls[idx];
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
			returnX = fixedX;
			returnY = fixedY;
			hasFind = true;
		}
	}
}

//测试代码
void BBHitManager::CheckHitSpiky(SpikyBall* ball1, SpikyBall* ball2)
{
	BBRect TempRect;
	TempRect.setRect(ball1->minX, ball1->minY, ball1->maxX, ball1->maxY);
	TempVec.clear();
	gameManager->ObjectTree.GetAllHitCircleNodeIdxs(TempRect, TempVec);
	if (TempVec.size() > 0) {
		for (int j = 0; j < TempVec.size(); j++)
		{
			int idx = TempVec[j];
			if (gameManager->mapSpikyBalls.find(idx) == gameManager->mapSpikyBalls.end()) {
				assert("when generate spiky has spiky in tree, but not in map %d", idx);
			}
			else {
				SpikyBall* checkBall = gameManager->mapSpikyBalls[idx];
				if (checkBall == ball2) {
					int temp = 1;
				}
				if (BBMathUtils::CheckCircleHit(*checkBall, TempRect.centerX, TempRect.centerY, ball1->radius))
				{
					int temp2 = 1;
				}
			}
		}
	}
}

//好像这个不一定需要一直去寻找
bool BBHitManager::FindFreeFoodPos(int& returnX, int& returnY)
{
	int x = BBMathUtils::GetRandom(1, BBConst::MaxWidth - 1);
	int y = BBMathUtils::GetRandom(1, BBConst::MaxHeight - 1);
	int posKey = y * BBConst::MaxWidth + x;
	if (gameManager->mapFoodPos.find(posKey) == gameManager->mapFoodPos.end())
	{
		returnX = x;
		returnY = y;
		return true;
	}
	else {
		assert("generate food same position %d_%d", x, y);
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
			x = 200;
			y = 200;
			isFirstRandom = false;
		}
		int fixedX, fixedY;
		bool isFixedX, isFixedY;
		BBMathUtils::FixCircle(gameManager->mapRect, x, y, radius, fixedX, fixedY, isFixedX, isFixedY);

		TempRect.setRect(fixedX - radius, fixedY - radius, fixedX + radius, fixedY + radius);
		TempVec.clear();
		bool hasHit = false;

		//检测和刺球,孢子 的碰撞
		gameManager->ObjectTree.GetAllHitCircleNodeIdxs(TempRect, TempVec);
		if (TempVec.size() > 0) {
			for (int j = 0; j < TempVec.size(); j++)
			{
				int idx = TempVec[j];
				if (gameManager->mapSpikyBalls.find(idx) == gameManager->mapSpikyBalls.end() && gameManager->mapSpores.find(idx) == gameManager->mapSpores.end()) {
					assert("add player idx in tree but not in spikyballs or spores %d", idx);
				}
				else {
					BaseCircleNode* ball = gameManager->mapSpikyBalls[idx];
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
					if (gameManager->mapPlayNodes.find(idx) == gameManager->mapPlayNodes.end()) {
						assert("add player idx in tree but not in mapPlayNodes %d", idx);
					}
					else {
						BaseCircleNode* ball = gameManager->mapPlayNodes[idx];
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