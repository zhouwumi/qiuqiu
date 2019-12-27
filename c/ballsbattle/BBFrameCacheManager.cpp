#include "BBFrameCacheManager.h"

BBFrameCacheManager::BBFrameCacheManager()
{
}


BBFrameCacheManager::~BBFrameCacheManager()
{
}

void BBFrameCacheManager::BeginUpdate()
{
	
}

//清除上一个关键帧里收到的所有命令
//清除上一个关键帧的执行的一些结果
void BBFrameCacheManager::OnKeyFrameUpdate()
{
	joinOperates.clear();
	splitOperates.clear();
	moveOperates.clear();
	playerQuitOperates.clear();
	playerShootOperates.clear();

	joinPlayerIds.clear();
	newPlayerNodes.clear();
	
	newFoodIdxs.clear();
	newSpikyIdxs.clear();
	SpikysEaten.clear();
	FoodEeaten.clear();
	RemovedNodes.clear();
	InCdNodes.clear();
	NodeEatSeq.clear();
	NodesEaten.clear();
	frameRemovePlayerNodeIdxs.clear();
	newSporeIdxs.clear();
}

void BBFrameCacheManager::AddEatenFood(int nodeId)
{
	if (FoodEeaten.find(nodeId) == FoodEeaten.end())
	{
		FoodEeaten.emplace(nodeId, 1);
	}
	else
	{
		FoodEeaten[nodeId] += 1;
	}
}

void BBFrameCacheManager::AddEatenSpiKy(int nodeId, int spikyMass)
{
	SpikysEaten.emplace(nodeId, spikyMass);
}



void BBFrameCacheManager::SetRemoved(int nodeId, int eatNodeId)
{
	RemovedNodes.emplace(nodeId, eatNodeId);
}

void BBFrameCacheManager::AddCd(int nodeId, int playerId)
{
	InCdNodes.emplace(nodeId, playerId);
}

void BBFrameCacheManager::AddKiller(int beEatPlayerId, int eatPlayerId)
{
	if (NodeEatSeq.find(beEatPlayerId) == NodeEatSeq.end())
	{
		std::vector<int> vec;
		vec.emplace_back(eatPlayerId);
		NodeEatSeq.emplace(beEatPlayerId, std::move(vec));
		return;
	}
	std::vector<int>& tempVec = NodeEatSeq[beEatPlayerId];
	tempVec.emplace_back(eatPlayerId);
}

void BBFrameCacheManager::AddEatenNode(int nodeId, int eatNodeId)
{
	if (NodesEaten.find(eatNodeId) == NodesEaten.end())
	{
		std::vector<int> vec;
		vec.emplace_back(nodeId);
		NodesEaten.emplace(eatNodeId, std::move(vec));
		return;
	}
	std::vector<int>& tempVec = NodesEaten[eatNodeId];
	tempVec.emplace_back(nodeId);
}

void BBFrameCacheManager::AddEatenSpores(int nodeId, int sporeId)
{
	if (SporesEaten.find(nodeId) == SporesEaten.end())
	{
		std::vector<int> vec;
		vec.emplace_back(sporeId);
		SporesEaten.emplace(nodeId, std::move(vec));
		return;
	}
	std::vector<int>& tempVec = SporesEaten[nodeId];
	tempVec.emplace_back(sporeId);
	
}

void BBFrameCacheManager::AddNewPlayerNode(int playerId, int playerNodeId)
{
	if (newPlayerNodes.find(playerId) == newPlayerNodes.end())
	{
		newPlayerNodes.emplace(playerId, std::vector<int>());
	}
	std::vector<int>& vec = newPlayerNodes[playerId];
	vec.emplace_back(playerNodeId);
}

void BBFrameCacheManager::AddNewSpore(int sporeIdx)
{
	newSporeIdxs.emplace_back(sporeIdx);
}

void BBFrameCacheManager::AddNewPlayer(int uid)
{
	joinPlayerIds.emplace_back(uid);
}

void BBFrameCacheManager::AddFrameRemovePlayerNodeIdxs(int playerNodeIdx, int playerId)
{
	frameRemovePlayerNodeIdxs.emplace_back(playerId);
	frameRemovePlayerNodeIdxs.emplace_back(playerNodeIdx);
}

void BBFrameCacheManager::AddOperatePlayerJoin(int uid)
{
	SimpleJoinOperate operate;
	operate.uid = uid;
	joinOperates.emplace_back(std::move(operate));
}

void BBFrameCacheManager::AddOperatePlayerSplit(int uid)
{
	SimpleSplitOperate command;
	command.uid = uid;
	splitOperates.emplace_back(std::move(command));
}

void BBFrameCacheManager::AddOperateMove(int uid, int angle, int percent)
{
	if (moveOperates.find(uid) == moveOperates.end())
	{
		SimpleMoveOperate command;
		command.uid = uid;
		command.angle = angle;
		command.pressure = percent;
		moveOperates.emplace(uid, std::move(command));
	}
	else
	{
		SimpleMoveOperate& command = moveOperates[uid];
		command.uid = uid;
		command.angle = angle;
		command.pressure = percent;
	}
}

void BBFrameCacheManager::AddOperatePlayerQuit(int uid)
{
	SimplePlayerQuitOperate command;
	command.uid = uid;
	playerQuitOperates.emplace_back(std::move(command));
}

void BBFrameCacheManager::AddOperateNewFood(int idx, int posKey)
{
	newFoodOperates.newInfoList.emplace_back(idx);
	newFoodOperates.newInfoList.emplace_back(posKey);
}

void BBFrameCacheManager::AddOperateNewSpiky(int idx, int posKey, int mass)
{
	newSpikyOperates.newInfoList.emplace_back(idx);
	newSpikyOperates.newInfoList.emplace_back(posKey);
	newSpikyOperates.newInfoList.emplace_back(mass);
}

void BBFrameCacheManager::AddOperatePlayerShoot(int uid)
{
	playerShootOperates.emplace(uid, true);
}

void BBFrameCacheManager::NotifyClientNewFoodCommand(int idx)
{
	newFoodIdxs.emplace_back(idx);
}
void BBFrameCacheManager::NotifyClientNewSpikyCommad(int idx)
{
	newSpikyIdxs.emplace_back(idx);
}