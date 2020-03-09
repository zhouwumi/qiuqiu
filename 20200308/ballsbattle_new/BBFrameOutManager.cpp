#include "BBFrameOutManager.h"

#include "BBMathUtils.h"

BBFrameOutManager::BBFrameOutManager()
{
}


BBFrameOutManager::~BBFrameOutManager()
{
}

void BBFrameOutManager::FrameClear()
{
	joinPlayerIds.clear();
	newPlayerNodes.clear();

	newFoodIdxs.clear();

	foodEatInfos.clear();
	nodeId2EatFoods.clear();

	spikyEatInfos.clear();
	nodeId2EatSpikys.clear();

	nodeEatInfos.clear();
	nodeId2BeEatNodes.clear();
	beEatNodeId2EatInfos.clear();
	markBeEatNodeId2EatNodeId.clear();
	markBeEatUid2EatUids.clear();

	markDirtyUids.clear();

	sporeEatInfos.clear();
	nodeId2EatSpores.clear();

	markAddNodeCds.clear();

	newSporeIdxs.clear();
	mapSporeIdxs.clear();
	markSporeEat.clear();

	newSpikyIdxs.clear();

	uid2crcs.clear();
	uid2LuaCrcs.clear();
	deadPlayerIds.clear();
}

void BBFrameOutManager::AddNewPlayer(int uid)
{
	joinPlayerIds.emplace_back(uid);
}


void BBFrameOutManager::AddPlayerDie(int uid)
{
	deadPlayerIds.emplace_back(uid);
}

void BBFrameOutManager::AddNewPlayerNode(int playerId, int playerNodeId)
{
	if (newPlayerNodes.find(playerId) == newPlayerNodes.end())
	{
		newPlayerNodes.emplace(playerId, std::vector<int>());
	}
	std::vector<int>& vec = newPlayerNodes[playerId];
	vec.emplace_back(playerNodeId);
}

void BBFrameOutManager::AddNewFood(int idx)
{
	newFoodIdxs.emplace_back(idx);
	//BBMathUtils::BBLOG("new food: %i", idx);
}

void BBFrameOutManager::AddNewSpore(int sporeIdx)
{
	newSporeIdxs.emplace_back(sporeIdx);
	mapSporeIdxs.emplace(sporeIdx, true);
	//BBMathUtils::BBLOG("new spore: %i", sporeIdx);
}

void BBFrameOutManager::AddNewSpiky(int spikyIdx)
{
	newSpikyIdxs.emplace_back(spikyIdx);
}

void BBFrameOutManager::AddFoodEatInfo(int uid, int nodeId, int foodIdx)
{
	FoodEatInfo info;
	info.uid = uid;
	info.nodeId = nodeId;
	info.foodIdx = foodIdx;
	foodEatInfos.emplace_back(info);

	if (nodeId2EatFoods.find(nodeId) == nodeId2EatFoods.end())
	{
		std::vector<FoodEatInfo> vec;
		vec.emplace_back(info);
		nodeId2EatFoods.emplace(nodeId, vec);
	}
	else
	{
		std::vector<FoodEatInfo>& tempVec = nodeId2EatFoods[nodeId];
		tempVec.emplace_back(info);
	}

	//BBMathUtils::BBLOG("node eat food  %i:%i", nodeId, foodIdx);
}

void BBFrameOutManager::AddSpikyEatInfo(int uid, int nodeId, int spikyIdx, int spikyBallMass)
{
	SpikyEatInfo info;
	info.uid = uid;
	info.nodeId = nodeId;
	info.spikyIdx = spikyIdx;
	info.spikyBallMass = spikyBallMass;
	spikyEatInfos.emplace_back(info);
	
	if (nodeId2EatSpikys.find(nodeId) == nodeId2EatSpikys.end())
	{
		std::vector<SpikyEatInfo> vec;
		vec.emplace_back(info);
		nodeId2EatSpikys.emplace(nodeId, vec);
	}
	else
	{
		std::vector<SpikyEatInfo>& tempVec = nodeId2EatSpikys[nodeId];
		tempVec.emplace_back(info);
	}
	markDirtyUids[uid] = true;
}


void BBFrameOutManager::AddNodeEatInfo(int uid, int nodeId, int beEatUid,int beEatNodeId, int beEatBallMass)
{
	NodeEatInfo info;
	info.uid = uid;
	info.nodeId = nodeId;
	info.beEatUid = beEatUid;
	info.beEatNodeId = beEatNodeId;
	info.beEatBallMass = beEatBallMass;
	nodeEatInfos.emplace_back(info);

	markBeEatNodeId2EatNodeId.emplace(beEatNodeId, nodeId);
	if (beEatUid == uid)
	{
		markAddNodeCds.emplace(nodeId, uid);
	}
	
	if (nodeId2BeEatNodes.find(nodeId) == nodeId2BeEatNodes.end())
	{
		std::vector<NodeEatInfo> vec;
		vec.emplace_back(info);
		nodeId2BeEatNodes.emplace(nodeId, vec);
	}
	else
	{
		std::vector<NodeEatInfo>& tempVec = nodeId2BeEatNodes[nodeId];
		tempVec.emplace_back(info);
	}

	beEatNodeId2EatInfos.emplace(beEatNodeId, info);
	
	if (markBeEatUid2EatUids.find(beEatUid) == markBeEatUid2EatUids.end())
	{
		std::vector<int> vec;
		vec.emplace_back(uid);
		markBeEatUid2EatUids.emplace(beEatUid, vec);
	}
	else
	{
		std::vector<int> vec = markBeEatUid2EatUids[beEatUid];
		vec.emplace_back(uid);
	}
	markDirtyUids[uid] = true;
	markDirtyUids[beEatUid] = true;
}

void BBFrameOutManager::AddSporeEatInfo(int uid, int nodeId, int beEatSporeId, int beEatSporeMass)
{
	SporeEatInfo info;
	info.uid = uid;
	info.nodeId = nodeId;
	info.beEatSporeId = beEatSporeId;
	info.beEatSporeMass = beEatSporeMass;
	sporeEatInfos.emplace_back(info);
	markSporeEat.emplace(beEatSporeId, true);

	if (nodeId2EatSpores.find(nodeId) == nodeId2EatSpores.end())
	{
		std::vector<SporeEatInfo> vec;
		vec.emplace_back(info);
		nodeId2EatSpores.emplace(nodeId, vec);
	}
	else
	{
		std::vector<SporeEatInfo>& tempVec = nodeId2EatSpores[nodeId];
		tempVec.emplace_back(info);
	}
	markDirtyUids[uid] = true;
}

void BBFrameOutManager::AddCd(int uid, int nodeId)
{
	markAddNodeCds.emplace(nodeId, uid);
}

bool BBFrameOutManager::IsNodeHasBeEat(int nodeId)
{
	return markBeEatNodeId2EatNodeId.find(nodeId) != markBeEatNodeId2EatNodeId.end();
}

void BBFrameOutManager::AddCrc(unsigned int uid, unsigned int crc, unsigned int luaCrc)
{
	if (uid2crcs.find(uid) == uid2crcs.end())
	{
		std::vector<int> vec;
		vec.emplace_back(crc);
		uid2crcs.emplace(uid, vec);
	}
	else
	{
		std::vector<int>& vec = uid2crcs[uid];
		vec.emplace_back(crc);
	}

	if (uid2LuaCrcs.find(uid) == uid2LuaCrcs.end())
	{
		std::vector<int> vec;
		vec.emplace_back(luaCrc);
		uid2LuaCrcs.emplace(uid, vec);
	}
	else
	{
		std::vector<int>& vec = uid2LuaCrcs[uid];
		vec.emplace_back(luaCrc);
	}
}