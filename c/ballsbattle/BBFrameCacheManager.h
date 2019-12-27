#ifndef BB_FRAME_CACHE_MANAGER_CPP
#define BB_FRAME_CACHE_MANAGER_CPP

#include<unordered_map>
#include<vector>
#include "Command.h"

class BBFrameCacheManager
{
public:
	BBFrameCacheManager();
	~BBFrameCacheManager();

	void BeginUpdate();
	void OnKeyFrameUpdate();

	//一些运行状态记录
	void AddEatenSpiKy(int nodeId, int spikyMass);
	void AddEatenFood(int nodeId);
	void SetRemoved(int nodeId, int eatNodeId);
	void AddCd(int nodeId, int playerId);
	void AddKiller(int beEatPlayerId, int eatPlayerId);
	void AddEatenNode(int nodeId, int eatNodeId);
	void AddEatenSpores(int nodeId, int sporeId);

	//需要返回的一些信息
	void AddNewPlayerNode(int playerId, int playerNodeId);
	void AddNewPlayer(int uid);
	void AddFrameRemovePlayerNodeIdxs(int playerNodeIdx, int playerId);
	void AddNewSpore(int sporeIdx);
	void NotifyClientNewFoodCommand(int idx);
	void NotifyClientNewSpikyCommad(int idx);

	//记录一些操作
	void AddOperatePlayerJoin(int uid);
	void AddOperatePlayerSplit(int uid);
	void AddOperateMove(int uid, int angle, int percent);
	void AddOperatePlayerQuit(int uid);
	void AddOperateNewFood(int idx, int posKey);
	void AddOperateNewSpiky(int idx, int posKey, int mass);
	void AddOperatePlayerShoot(int uid);
public:
	std::unordered_map<int, int> SpikysEaten;
	std::unordered_map<int, int> FoodEeaten;
	std::unordered_map<int, int> RemovedNodes;
	std::unordered_map<int, int> InCdNodes;
	std::unordered_map<int, std::vector<int>> NodeEatSeq;
	std::unordered_map<int, std::vector<int>> NodesEaten;
	std::unordered_map<int, std::vector<int>> SporesEaten;

	std::unordered_map<int, std::vector<int>> newPlayerNodes;
	std::vector<int> joinPlayerIds;
	std::vector<int> newFoodIdxs; //c->lua有新生成的食物
	std::vector<int> newSpikyIdxs; //c->lua有新生成的刺球
	std::vector<int> frameRemovePlayerNodeIdxs;
	std::vector<int> newSporeIdxs;//新的孢子

	std::vector<SimpleJoinOperate> joinOperates;
	std::vector<SimpleSplitOperate> splitOperates;
	std::unordered_map<int, SimpleMoveOperate> moveOperates;
	std::vector<SimplePlayerQuitOperate> playerQuitOperates;
	SimpleNewFoodSpikyOperate newFoodOperates; //lua->c有新生成的食物
	SimpleNewFoodSpikyOperate newSpikyOperates; //lua->c有新生成的刺球
	std::unordered_map<int, bool> playerShootOperates;
};

#endif