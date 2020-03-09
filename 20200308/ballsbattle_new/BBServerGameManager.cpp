#include "BBServerGameManager.h"

#include "BBMathUtils.h"

BBServerGameManager* BBServerGameManager::Create()
{
	return new BBServerGameManager();
}

BBServerGameManager::BBServerGameManager()
{
}


BBServerGameManager::~BBServerGameManager()
{
}

void BBServerGameManager::FrameUpdate()
{
	mGameFrame++;
	frameOutManager.FrameClear();
	playerManager.HandleFrameInputCommand();
	sporeManager.MoveSpores();
	playerManager.UpdatePlayer();

	playerManager.PlayerEat();
	playerManager.FinishEat();
	playerManager.DoSpikySplit();
	playerManager.ServerDoJoinPlayer();
	foodSpikyManager.ServerCheckRegenFood();

	frameInManager.FrameClear();
	AddNodesCd();
}

void BBServerGameManager::InitFoodSpiky()
{
	foodSpikyManager.ServerInitFoodSpiky();
	//sporeManager.DebugCreateSpore();
}

std::vector<int> BBServerGameManager::GetEatFoods()
{
	std::vector<int> ret;
	auto& foodEatInfos = frameOutManager.foodEatInfos;
	for (auto& foodInfo : foodEatInfos)
	{
		ret.emplace_back(foodInfo.foodIdx);
		ret.emplace_back(foodInfo.nodeId);
	}
	return ret;
}

std::vector<int> BBServerGameManager::GetEatSpores()
{
	std::vector<int> ret;
	auto& sporeEatInfos = frameOutManager.sporeEatInfos;
	for (auto& sporeInfo : sporeEatInfos)
	{
		ret.emplace_back(sporeInfo.beEatSporeId);
		ret.emplace_back(sporeInfo.nodeId);
	}
	return ret;
}

std::vector<int> BBServerGameManager::GetEatSpikys()
{
	std::vector<int> ret;
	auto& spikyEatInfos = frameOutManager.spikyEatInfos;
	for (auto& spikyEatInfo : spikyEatInfos)
	{
		ret.emplace_back(spikyEatInfo.spikyIdx);
	}
	return ret;
}


std::vector<int> BBServerGameManager::GetEatNodes()
{
	std::vector<int> ret;
	auto& markBeEatNodeId2EatNodeId = frameOutManager.markBeEatNodeId2EatNodeId;
	for (auto iter : markBeEatNodeId2EatNodeId)
	{
		ret.emplace_back(iter.first);
		ret.emplace_back(iter.second);
	}
	return ret;
}

void BBServerGameManager::AddOperatePlayerJoin(int uid)
{
	frameInManager.AddPlayerJoin(uid);
}

void BBServerGameManager::AddPlayerQuit(int uid)
{
	frameInManager.AddPlayerQuit(uid);
}

std::vector<int>& BBServerGameManager::GetFrameDiePlayerIds()
{
	return frameOutManager.deadPlayerIds;
}

unsigned int BBServerGameManager::GetAckCommand(unsigned int uid)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		return player->mCommandList.mLastCommandAck;
	}
	return 0;
}

bool BBServerGameManager::IsPlayerCommandMatch(unsigned int uid)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		return !player->mCommandList.mControlMismatch;
	}
	return false;
}



std::vector<int> BBServerGameManager::GetAllFoodInfos()
{
	return foodSpikyManager.GetAllFoodInfos();
}

std::vector<int> BBServerGameManager::GetAllSpikyInfos()
{
	return foodSpikyManager.GetAllSpikyInfos();
}

std::vector<int>& BBServerGameManager::GetAllSporeInfos()
{
	return sporeManager.vecSporeIds;
}

std::vector<int>& BBServerGameManager::GetAllPlayerIdxs()
{
	return playerManager.playerIds;
}

std::vector<int>& BBServerGameManager::GetFrameNewPlayer()
{
	return frameOutManager.joinPlayerIds;
}

std::vector<int>& BBServerGameManager::GetFrameNewFood()
{
	return frameOutManager.newFoodIdxs;
}

std::vector<int>& BBServerGameManager::GetFrameNewSpiky()
{
	return frameOutManager.newSpikyIdxs;
}

std::vector<int>& BBServerGameManager::GetFrameNewSpore()
{
	return frameOutManager.newSporeIdxs;
}

void BBServerGameManager::ClearServerLog()
{
	BBMathUtils::ClearLog();
}

std::vector < std::string> BBServerGameManager::GetServerLog()
{
	return BBMathUtils::serverLog;
}

std::vector<int> BBServerGameManager::GetCrcs(unsigned int uid)
{
	std::vector<int> ret;
	if (frameOutManager.uid2crcs.find(uid) != frameOutManager.uid2crcs.end())
	{
		ret = frameOutManager.uid2crcs[uid];
	}
	return ret;
}

std::vector<int> BBServerGameManager::GetLuaCrcs(unsigned int uid)
{
	std::vector<int> ret;
	if (frameOutManager.uid2LuaCrcs.find(uid) != frameOutManager.uid2LuaCrcs.end())
	{
		ret = frameOutManager.uid2LuaCrcs[uid];
	}
	return ret;
}