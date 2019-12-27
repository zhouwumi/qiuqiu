#include "BBFoodSpikyManager.h"
#include "BBGameManager.h"
#include "BBConst.h"

BBFoodSpikyManager::BBFoodSpikyManager()
{
	FoodGridMap.SetMap(BBConst::MaxWidth, BBConst::MaxHeight, 5, BBConst::FoodNum);
	SpikyGridMap.SetMap(BBConst::MaxWidth, BBConst::MaxHeight, 4, BBConst::FoodNum);
}


BBFoodSpikyManager::~BBFoodSpikyManager()
{
	ClearAllFood();
	ClearAllSpiky();
}

void BBFoodSpikyManager::ClearAllFood()
{
	for (auto iter : mapFoodIdxs)
	{
		delete iter.second;
		iter.second = NULL;
	}
	mapFoodIdxs.clear();
	mapFoodPos.clear();
	FoodGridMap.Clear();
}

void BBFoodSpikyManager::RemoveFoodByIdx(int idx)
{
	if (mapFoodIdxs.find(idx) != mapFoodIdxs.end())
	{
		Food* food = mapFoodIdxs[idx];
		int posKey = food->posKey;
		mapFoodIdxs.erase(idx);
		mapFoodPos.erase(posKey);
		//FoodTree.RemoveCircleNode(food);
		FoodGridMap.RemoveObject(food);
		delete food;
	}
}

void BBFoodSpikyManager::RemoveFoodByPos(int posKey)
{
	if (mapFoodPos.find(posKey) != mapFoodPos.end())
	{
		Food* food = mapFoodPos[posKey];
		int idx = food->Idx;
		mapFoodIdxs.erase(idx);
		mapFoodPos.erase(posKey);
		//FoodTree.RemoveCircleNode(food);
		FoodGridMap.RemoveObject(food);
		delete food;
	}
}

void BBFoodSpikyManager::RemoveSpikyByIdx(int idx)
{
	if (mapSpikyBalls.find(idx) != mapSpikyBalls.end())
	{
		SpikyBall* ball = mapSpikyBalls[idx];
		mapSpikyBalls.erase(idx);
		//ObjectTree.RemoveCircleNode(ball);
		SpikyGridMap.RemoveObject(ball);
		delete ball;
	}
}

void BBFoodSpikyManager::ClearAllSpiky()
{
    for(auto iter: mapSpikyBalls){
        delete iter.second;
        iter.second = NULL;
    }
	mapSpikyBalls.clear();
	SpikyGridMap.Clear();
}

void BBFoodSpikyManager::InitFoodSpiky()
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


int BBFoodSpikyManager::CreateFood()
{
	int x, y;
	if (gameManager->hitManager.FindFreeFoodPos(x, y))
	{
		Food* food = gameManager->objectManager.CreateFood(x, y);
		mapFoodPos.emplace(food->posKey, food);
		mapFoodIdxs.emplace(food->Idx, food);
		//FoodTree.AddCircleNode(food);

		FoodGridMap.AddObjectMapKey(food, x, y);
		return food->Idx;
	}
	return 0;
}

int BBFoodSpikyManager::CreateSpiky()
{
	int x, y;
	int mass;
	gameManager->hitManager.FindFreeSpikyPos(mass, x, y);

	SpikyBall* newSpikyBall = gameManager->objectManager.CreateSpikyBall(x, y, mass);
	mapSpikyBalls.emplace(newSpikyBall->Idx, newSpikyBall);
	//ObjectTree.AddCircleNode(newSpikyBall);
	SpikyGridMap.AddObjectMapKey(newSpikyBall, x, y);
	return newSpikyBall->Idx;
}

bool BBFoodSpikyManager::ClientFinishGenerateFoodAndSpiky()
{
	if (gameManager->GetGameFrame() != 0)
	{
		return false;
	}

	auto& foodInfos = gameManager->frameCacheManager.newFoodOperates.newInfoList;
	for (int i = 0; i < foodInfos.size(); i += 2)
	{
		int idx = foodInfos[i];
		int posKey = foodInfos[i + 1];
		int x = posKey % BBConst::MaxWidth;
		int y = posKey / BBConst::MaxWidth;

		Food* food = gameManager->objectManager.CreateFood(x, y);
		mapFoodPos.emplace(food->posKey, food);
		mapFoodIdxs.emplace(food->Idx, food);

		FoodGridMap.AddObjectMapKey(food, x, y);
	}

	auto& spikyInfos = gameManager->frameCacheManager.newSpikyOperates.newInfoList;
	for (int i = 0; i < spikyInfos.size(); i += 3)
	{
		int idx = spikyInfos[i];
		int posKey = spikyInfos[i + 1];
		int mass = spikyInfos[i + 2];

		int x = posKey % BBConst::MaxWidth;
		int y = posKey / BBConst::MaxWidth;
		SpikyBall* newSpikyBall = gameManager->objectManager.CreateSpikyBall(x, y, mass);
		mapSpikyBalls.emplace(newSpikyBall->Idx, newSpikyBall);
		SpikyGridMap.AddObjectMapKey(newSpikyBall, x, y);
	}

	gameManager->frameCacheManager.newFoodOperates.newInfoList.clear();
	gameManager->frameCacheManager.newSpikyOperates.newInfoList.clear();
	return true;
}

std::vector<int> BBFoodSpikyManager::ClientGenerateFood(int num)
{
	std::vector<int> vec;
	vec.reserve(num * 2);

	for (int i = 0; i < num; i++)
	{
		int x, y;
		if (gameManager->hitManager.FindFreeFoodPos(x, y))
		{
			Food* food = gameManager->objectManager.CreateFood(x, y);
			mapFoodPos.emplace(food->posKey, food);
			mapFoodIdxs.emplace(food->Idx, food);
			FoodGridMap.AddObjectMapKey(food, x, y);
			vec.emplace_back(food->Idx);
			vec.emplace_back(food->posKey);
		}
	}
	ClearAllFood();
	return std::move(vec);
}
std::vector<int> BBFoodSpikyManager::ClientGenerateSpiky(int num)
{
	std::vector<int> vec;
	vec.reserve(num * 3);

	for (int i = 0; i < num; i++)
	{
		int x, y;
		int mass;
		gameManager->hitManager.FindFreeSpikyPos(mass, x, y);

		SpikyBall* newSpikyBall = gameManager->objectManager.CreateSpikyBall(x, y, mass);
		mapSpikyBalls.emplace(newSpikyBall->Idx, newSpikyBall);
		SpikyGridMap.AddObjectMapKey(newSpikyBall, x, y);

		int posKey = y * BBConst::MaxWidth + x;
		vec.emplace_back(newSpikyBall->Idx);
		vec.emplace_back(posKey);
		vec.emplace_back(mass);
	}
	ClearAllSpiky();
	return std::move(vec);
}

std::vector<int> BBFoodSpikyManager::GetAllFoodInfos()
{
	std::vector<int> ret;
	for (auto iter : mapFoodPos)
	{
		ret.emplace_back(iter.first);
	}
	return ret;
}

int BBFoodSpikyManager::GetFoodIdxByPos(int pos)
{
	return mapFoodPos[pos]->Idx;
}

int BBFoodSpikyManager::GetFoodPosByIdx(int idx)
{
	return mapFoodIdxs[idx]->posKey;
}

SpikyBall* BBFoodSpikyManager::GetSpikyInfo(int idx)
{
	auto iter = mapSpikyBalls.find(idx);
	if (iter == mapSpikyBalls.end())
	{
		return NULL;
	}
	return (*iter).second;
}

std::vector<int> BBFoodSpikyManager::GetAllSpikyInfos()
{
	std::vector<int> ret;
	for (auto iter : mapSpikyBalls)
	{
		ret.emplace_back(iter.first);
	}
	return ret;
}

void BBFoodSpikyManager::SpikyBeEat(int idx)
{
	int nextRefreshFrame = gameManager->GetGameFrame() + BBConst::FPS * BBConst::SpikyTime;
	SpikyRefresh.emplace_back(nextRefreshFrame);
	RemoveSpikyByIdx(idx);
}

void BBFoodSpikyManager::CheckRegenFood()
{
	int curFrame = gameManager->GetGameFrame();
	while (SpikyRefresh.size() > 0)
	{
		int nextFrame = SpikyRefresh.front();
		if (curFrame >= nextFrame)
		{
			int newIdx = CreateSpiky();
			gameManager->frameCacheManager.NotifyClientNewSpikyCommad(newIdx);
			SpikyRefresh.pop_front();
		}
		else
		{
			break;
		}
	}

	for (int i = 0; i < BBConst::FoodNum - mapFoodIdxs.size(); i++)
	{
		int idx = CreateFood();
		gameManager->frameCacheManager.NotifyClientNewFoodCommand(idx);
	}
}