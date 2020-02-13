#ifndef BB_FOOD_SPIKY_MANAGER_CPP
#define BB_FOOD_SPIKY_MANAGER_CPP
#include<vector>
#include<list>
#include "BBObjects.h"
#include "GridMap.h"

class BBGameManager;

class BBFoodSpikyManager
{
public:
	BBFoodSpikyManager();
	~BBFoodSpikyManager();

	void ServerInitFoodSpiky();
	int ServerCreateFood();
	int ServerCreateSpiky();

	void ServerCheckRegenFood();

	//food食物
	void RemoveFoodByIdx(int idx);
	void ClearAllFood();

	void RemoveSpikyByIdx(int idx);
	void ClearAllSpiky();

	void SpikyBeEat(int idx);

	void SetGameManager(BBGameManager* manager)
	{
		gameManager = manager;
	}

	std::vector<int> GetAllFoodInfos();

	SpikyBall* GetSpikyInfo(int idx);
	std::vector<int> GetAllSpikyInfos();
	void ClientAddNewFoodFromServer(int posKey);
	void ClientAddNewSpikyFromServer(int idx, int posKey, int mass);

	unsigned int GetAllFoodCrc();
	unsigned int GetAllSpikyCrc();
public:
	//std::unordered_map<unsigned int, Food*> mapFoodPos;//食物位置key
	std::unordered_map<unsigned int, Food*> mapFoodIdxs;//食物idx key
	std::unordered_map<unsigned int, SpikyBall*> mapSpikyBalls;//刺球

	GridMap FoodGridMap;//食物格子
	GridMap SpikyGridMap;//刺球格子

	std::list<int> SpikyRefresh; //记录刺球刷新信息
private:
	BBGameManager * gameManager;
};

#endif