#include "BBFoodSpikyManager.h"
#include "BBGameManager.h"

#include "BBMathUtils.h"
#include "BBConfigManager.h"

BBFoodSpikyManager::BBFoodSpikyManager()
{
	int w = BBConfigManager::gameWidth;
	int h = BBConfigManager::gameHeight;
	FoodGridMap.SetMap(w, h, 5, BBConfigManager::foodCount);
	SpikyGridMap.SetMap(w, h, 4, BBConfigManager::spikyCount);
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
	FoodGridMap.Clear();
}

void BBFoodSpikyManager::RemoveFoodByIdx(int idx)
{
	if (mapFoodIdxs.find(idx) != mapFoodIdxs.end())
	{
		Food* food = mapFoodIdxs[idx];
		mapFoodIdxs.erase(idx);
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

void BBFoodSpikyManager::ServerInitFoodSpiky()
{
	//刺球
	for (int i = 0; i < BBConfigManager::spikyCount; i++)
	{
		ServerCreateSpiky();
	}

	//食物
	for (int i = 0; i < BBConfigManager::foodCount; i++)
	{
		ServerCreateFood();
	}
}


int BBFoodSpikyManager::ServerCreateFood()
{
	int x, y;
	if (gameManager->hitManager.FindFreeFoodPos(x, y))
	{
		Food* food = gameManager->objectManager.CreateFood(x, y);
		mapFoodIdxs.emplace(food->idx, food);

		FoodGridMap.AddObjectMapKey(food, x, y);
		return food->idx;
	}
	return 0;
}

int BBFoodSpikyManager::ServerCreateSpiky()
{
	int x, y;
	int mass;
	gameManager->hitManager.FindFreeSpikyPos(mass, x, y);

	SpikyBall* newSpikyBall = gameManager->objectManager.CreateSpikyBall(x, y, mass);
	mapSpikyBalls.emplace(newSpikyBall->idx, newSpikyBall);
	SpikyGridMap.AddObjectMapKey(newSpikyBall, x, y);
	return newSpikyBall->idx;
}

void BBFoodSpikyManager::ClientAddNewFoodFromServer(int posKey)
{
	int x, y;
	BBMathUtils::location_to_xy(posKey, x, y);
	Food* food = gameManager->objectManager.CreateFood(x, y);
	mapFoodIdxs.emplace(posKey, food);

	FoodGridMap.AddObjectMapKey(food, x, y);
}

void BBFoodSpikyManager::ClientAddNewSpikyFromServer(int idx, int posKey, int mass)
{
	int x, y;
	BBMathUtils::location_to_xy(posKey, x, y);

	SpikyBall* newSpikyBall = gameManager->objectManager.CreateSpikyBall(x, y, mass, idx);
	mapSpikyBalls.emplace(newSpikyBall->idx, newSpikyBall);
	SpikyGridMap.AddObjectMapKey(newSpikyBall, x, y);
}

std::vector<int> BBFoodSpikyManager::GetAllFoodInfos()
{
	std::vector<int> ret;
	for (auto iter : mapFoodIdxs)
	{
		ret.emplace_back(iter.first);
	}
	return ret;
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
	int nextRefreshFrame = gameManager->GetGameFrame() + BBConfigManager::spikyTime;
	SpikyRefresh.emplace_back(nextRefreshFrame);
	RemoveSpikyByIdx(idx);
}

void BBFoodSpikyManager::ServerCheckRegenFood()
{
	if (!gameManager->IsServer())
	{
		return;
	}
	int curFrame = gameManager->GetGameFrame();
	while (SpikyRefresh.size() > 0)
	{
		int nextFrame = SpikyRefresh.front();
		if (curFrame >= nextFrame)
		{
			int newIdx = ServerCreateSpiky();
			gameManager->frameOutManager.AddNewSpiky(newIdx);
			SpikyRefresh.pop_front();
		}
		else
		{
			break;
		}
	}

	for (int i = 0; i < BBConfigManager::foodCount - mapFoodIdxs.size(); i++)
	{
		int idx = ServerCreateFood();
		if (idx != 0 && mapFoodIdxs.find(idx) != mapFoodIdxs.end())
		{
			gameManager->frameOutManager.AddNewFood(idx);
		}
	}
}

unsigned int BBFoodSpikyManager::GetAllFoodCrc()
{
	static unsigned int foodBuffer[1024 * 4];
	unsigned int index = 0;

	std::vector<unsigned int> idxs;
	for (auto iter : mapFoodIdxs)
	{
		idxs.emplace_back(iter.first);
	}
	std::sort(idxs.begin(), idxs.end());
	for (int i = 0; i < idxs.size(); i++)
	{
		int id = idxs[i];
		Food* food = mapFoodIdxs[id];
		foodBuffer[index++] = food->location.x;
		foodBuffer[index++] = food->location.y;
	}
	
	unsigned int size = index * sizeof(int) / sizeof(char);
	unsigned int ret = BBMathUtils::crc32((char*)(&foodBuffer), size);
	return ret;
}

unsigned int BBFoodSpikyManager::GetAllSpikyCrc()
{
	static unsigned int spikyBuffer[64 * 4];
	unsigned int index = 0;

	std::vector<unsigned int> idxs;
	for (auto iter : mapSpikyBalls)
	{
		idxs.emplace_back(iter.first);
	}
	std::sort(idxs.begin(), idxs.end());
	for (int i = 0; i < idxs.size(); i++)
	{
		int id = idxs[i];
		SpikyBall* ball = mapSpikyBalls[id];
		spikyBuffer[index++] = ball->location.x;
		spikyBuffer[index++] = ball->location.y;
	}

	unsigned int size = index * sizeof(int) / sizeof(char);
	unsigned int ret = BBMathUtils::crc32((char*)(&spikyBuffer), size);
	return ret;
}