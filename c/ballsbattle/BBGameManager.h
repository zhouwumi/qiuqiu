#pragma once
#include<unordered_map>
#include"BBPlayerNode.h"
#include"QuadTree.h"
#include "BBHitManager.h"
#include "BBObjectManager.h"
#include<functional>

class BBGameManager
{
public:
	//初始化房间
	void InitRoom();
	void InitFood();

	//food食物
	void CreateFood();
	void RemoveFoodByIdx(int idx);
	void RemoveFoodByPos(int posKey);

	//刺球
	void CreateSpiky();
	void RemoveSpikyByIdx(int idx);

	//玩家
	void CreatePlayer(int uid);
	//给这个玩家加一个球
	void CreatePlayerNode(int uid);
	void RemovePlayer(int uid);
	void RemovePlayerNode(int uid, int nodeIdx, bool removeFromVec = true);

	//每帧更新
	void OnUpdate();
	
	PlayerNode* SpawnPlayerNode();
	static BBGameManager* getInstance();
	void destroyInstance();
protected:
	BBGameManager();
	~BBGameManager();
	static BBGameManager* s_sharedGameManager;
public:
	std::vector<int> GetAllFoodInfos();
	std::vector<int> GetAllSpikyInfos();
	int GetFoodIdxByPos(int pos);
	int GetFoodPosByIdx(int idx);
	SpikyBall* GetSpikyInfo(int idx);
	std::vector<int> GetPlayerNodeIdx(int uid);
	PlayerNode* GetPlayerNodeInfo(int idx);
	std::vector<int> CheckPlayerHit(int uid);
	void ChangePlayerNodePos(int idx, int x, int y);

	void CheckSpikyHit(int idx1, int idx2);

	void SetEatCbFunction(std::function<void(int, int, int, int)> eatFunc)
	{
		eatCallback = eatFunc;
	}
	void RemoveEatCbFunction()
	{
		eatCallback = NULL;
	}

	void DebugPrintLog();

	void StartMovePlayer(int uid);

private:
	//玩家部分
	void _updatePlayerCenter(int uid);
	//玩家移动
	void MovePlayer(Player* player);
public:
	std::unordered_map<unsigned int, PlayerNode*> mapPlayNodes; //玩家的球
	std::unordered_map<unsigned int, Player*> mapPlayers;//玩家
	std::unordered_map<unsigned int, Spore*> mapSpores;//孢子
	std::unordered_map<unsigned int, Food*> mapFoodPos;//食物位置key
	std::unordered_map<unsigned int, Food*> mapFoodIdxs;//食物idx key
	std::unordered_map<unsigned int, SpikyBall*> mapSpikyBalls;//刺球

	QuadTree ObjectTree; //孢子,刺球
	QuadTree FoodTree; //食物
	QuadTree NodeTree; //玩家的球

	BBHitManager hitManager;
	BBObjectManager objectManager;

	BBRect spikyRect; //刺球生成的范围
	BBRect mapRect;  //整个地图的范围
private:
	//obj_type, obj_idx, eat_obj_type, eat_obj_idx
	std::function<void(int, int, int, int)> eatCallback;
};

