#ifndef BB_FRAME_OUT_MANAGER_CPP
#define BB_FRAME_OUT_MANAGER_CPP

#include<vector>
#include<unordered_map>

class FoodEatInfo
{
public:
	int uid;
	int nodeId;
	int foodIdx;
};

class SpikyEatInfo
{
public:
	int uid;
	int nodeId;
	int spikyIdx;
	int spikyBallMass;
};


class NodeEatInfo
{
public:
	int uid;
	int nodeId;
	int beEatUid;
	int beEatNodeId;
	int beEatBallMass;
};

class SporeEatInfo
{
public:
	int uid;
	int nodeId;
	int beEatSporeId;
	int beEatSporeMass;
};

class BBFrameOutManager
{
public:
	BBFrameOutManager();
	~BBFrameOutManager();

	void FrameClear();

	void AddNewPlayer(int uid);
	void AddNewPlayerNode(int playerId, int playerNodeId);

	void AddNewFood(int idx);
	void AddNewSpore(int sporeIdx);
	void AddNewSpiky(int spikyIdx);

	void AddFoodEatInfo(int uid, int nodeId, int foodIdx);
	void AddSpikyEatInfo(int uid, int nodeId, int spikyIdx, int spikyBallMass);
	void AddNodeEatInfo(int uid, int nodeId, int beEatUid,int beEatNodeId, int beEatBallMass);
	void AddSporeEatInfo(int uid, int nodeId, int beEatSporeId, int beEatSporeMass);
	void AddCd(int uid, int nodeId);

	bool IsNodeHasBeEat(int nodeId);

	void AddPlayerDie(int uid);

	void AddCrc(unsigned int uid, unsigned int crc);
public:
	std::vector<int> joinPlayerIds;// new players
	std::unordered_map<int, std::vector<int>> newPlayerNodes;

	std::vector<int> newFoodIdxs;

	std::vector<FoodEatInfo> foodEatInfos; //eat food
	std::unordered_map<int, std::vector<FoodEatInfo>> nodeId2EatFoods;//node id-> food list

	std::vector<SpikyEatInfo> spikyEatInfos;
	std::unordered_map<int, std::vector<SpikyEatInfo>> nodeId2EatSpikys;//node id-> spiky list

	std::vector<NodeEatInfo> nodeEatInfos;
	std::unordered_map<int, std::vector<NodeEatInfo>> nodeId2BeEatNodes;//node id-> be eat node list
	std::unordered_map<int, NodeEatInfo> beEatNodeId2EatInfos;//be eat node id-> node info
	std::unordered_map<int, int> markBeEatNodeId2EatNodeId; //be eat node id -> eat node id
	std::unordered_map<int, int> markAddNodeCds; //eat self node add cd
	std::unordered_map<int, std::vector<int>> markBeEatUid2EatUids; // be eat uid -> eat uid
	std::unordered_map<int, bool> markDirtyUids; //dirty uid

	std::vector<SporeEatInfo> sporeEatInfos;
	std::unordered_map<int, std::vector<SporeEatInfo>> nodeId2EatSpores;//node id-> node list
	std::unordered_map<int, bool> markSporeEat;

	std::vector<int> newSporeIdxs;//new spores
	std::unordered_map<int, bool> mapSporeIdxs;

	std::vector<int> newSpikyIdxs;

	std::unordered_map<unsigned int, std::vector<int>> uid2crcs; //uid->crc list

	std::vector<int> deadPlayerIds;
};

#endif