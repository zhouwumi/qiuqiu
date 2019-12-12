#pragma once
#include<unordered_map>
#include"BBPlayerNode.h"
#include"QuadTree.h"

class BBGameManager
{
public:
	//初始化房间
	void InitRoom();
	void InitFood();

	//food食物
	void AddFoodByKey(int key, int x, int y);


	//玩家
	void AddPlayer(int uid);
	//给这个玩家加一个球
	void AddPlayerNode(int uid);

	PlayerNode* SpawnPlayerNode();

	static BBGameManager* getInstance();
	void destroyInstance();
protected:
	BBGameManager();
	~BBGameManager();
	static BBGameManager* s_sharedGameManager;
private:
	unsigned int _getNextObjIdx();

	//玩家部分
	void _updatePlayerCenter(int uid);
public:
	std::unordered_map<unsigned int, PlayerNode*> mapPlayNodes; //玩家的球
	std::unordered_map<unsigned int, Player*> mapPlayers;//玩家
	std::unordered_map<unsigned int, Spore*> mapSpores;//孢子
	std::unordered_map<unsigned int, Food*> mapFoods;//食物
	std::unordered_map<unsigned int, SpikyBall*> mapSpikyBalls;//刺球

	QuadTree ObjectTree; //孢子,刺球
	QuadTree FoodTree; //食物
	QuadTree NodeTree; //玩家的球

private:
	BBRect spikyRect; //刺球生成的范围
	BBRect mapRect;  //整个地图的范围
	unsigned int _curObjIdx;
};

