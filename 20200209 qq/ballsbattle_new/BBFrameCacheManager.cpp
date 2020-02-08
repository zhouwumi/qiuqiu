#include "BBFrameCacheManager.h"

BBFrameCacheManager::BBFrameCacheManager()
{
}


BBFrameCacheManager::~BBFrameCacheManager()
{
}

//清除上一个关键帧里收到的所有命令
//清除上一个关键帧的执行的一些结果
void BBFrameCacheManager::OnKeyFrameUpdate()
{
	newSpikyIdxs.clear();
	frameRemovePlayerNodeIdxs.clear();
	newSporeIdxs.clear();
}


void BBFrameCacheManager::AddNewSpore(int sporeIdx)
{
	newSporeIdxs.emplace_back(sporeIdx);
}

void BBFrameCacheManager::AddFrameRemovePlayerNodeIdxs(int playerNodeIdx, int playerId)
{
	frameRemovePlayerNodeIdxs.emplace_back(playerId);
	frameRemovePlayerNodeIdxs.emplace_back(playerNodeIdx);
}


void BBFrameCacheManager::NotifyClientNewSpikyCommad(int idx)
{
	newSpikyIdxs.emplace_back(idx);
}