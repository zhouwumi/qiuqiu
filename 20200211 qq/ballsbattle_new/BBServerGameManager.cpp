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

void BBServerGameManager::InitFoodSpiky()
{
	foodSpikyManager.ServerInitFoodSpiky();
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

bool BBServerGameManager::IsNeedSyncState(unsigned int uid)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		return player->IsNeedSyncState();
	}
	return false;
}

bool BBServerGameManager::CanSkipSyncState(unsigned int uid)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		return player->CanSkipSyncState();
	}
	return false;
}

void BBServerGameManager::AddOperatePlayerJoin(int uid)
{
	frameInManager.AddPlayerJoin(uid);
}

void BBServerGameManager::AddPlayerQuit(int uid)
{
	frameInManager.AddPlayerQuit(uid);
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

std::vector<int> BBServerGameManager::GetFrameNewPlayerNodeIdxs()
{
	std::vector<int> ret;
	if (frameOutManager.newPlayerNodes.size() > 0)
	{
		for (auto iter : frameOutManager.newPlayerNodes)
		{
			std::vector<int>& vecs = iter.second;
			for (int i = 0; i < vecs.size(); i++)
			{
				ret.emplace_back(vecs[i]);
			}
		}
	}
	return ret;
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
	return frameCacheManager.newSpikyIdxs;
}

std::vector<int>& BBServerGameManager::GetFrameRemovedPlayerNodeIdxs()
{
	return frameCacheManager.frameRemovePlayerNodeIdxs;
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