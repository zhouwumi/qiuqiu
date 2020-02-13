#include "BBConfigManager.h"

bool BBConfigManager::isOpenLog = false;
int BBConfigManager::maxPredictionCnt = 10;
int BBConfigManager::maxOtherStepCommandCnt = 3;
int BBConfigManager::maxCommandNoAck = 15;

int BBConfigManager::minCatchUpTicks = 3;
int BBConfigManager::maxCatchUpTicks = 5;

int BBConfigManager::protectTime = 75;

BBConfigManager::BBConfigManager()
{
}


BBConfigManager::~BBConfigManager()
{
}

void BBConfigManager::SetLogState(bool open)
{
	BBConfigManager::isOpenLog = open;
}

void BBConfigManager::SetMaxPredictionCnt(int cnt)
{
	BBConfigManager::maxPredictionCnt = cnt;
}

void BBConfigManager::SetMaxOtherStepCommandCnt(int cnt)
{
	BBConfigManager::maxOtherStepCommandCnt = cnt;
}

void BBConfigManager::SetMaxCommandNoAck(int cnt)
{
	BBConfigManager::maxCommandNoAck = cnt;
}

void BBConfigManager::SetMinCatchUpTicks(int cnt)
{
	BBConfigManager::minCatchUpTicks = cnt;
}

void BBConfigManager::SetMaxCatchUpTicks(int cnt)
{
	BBConfigManager::maxCatchUpTicks = cnt;
}

void BBConfigManager::SetProtectTime(int cnt)
{
	BBConfigManager::protectTime = cnt;
}