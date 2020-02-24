#include "BBConfigManager.h"

bool BBConfigManager::clientDebug = false;

int BBConfigManager::fixLength = 50;
int BBConfigManager::spikyTime = 75;

int BBConfigManager::gameWidth = 4500;
int BBConfigManager::gameHeight = 4500;

int BBConfigManager::maxBallCount = 12;
int BBConfigManager::foodCount = 600;
int BBConfigManager::foodMass = 1;
int BBConfigManager::spikyCount = 30;

int BBConfigManager::initBallMass = 10;
int BBConfigManager::minSpikyMass = 50;
int BBConfigManager::maxSpikyMass = 200;
int BBConfigManager::maxSpikySplitChildCount = 9;

int BBConfigManager::minShootBallMass = 20;
int BBConfigManager::sporeMass = 10;
int BBConfigManager::sporeStopFrame = 10;
int BBConfigManager::sporeInitSpeed = 60;
int BBConfigManager::sporeCdTime = 5;

int BBConfigManager::splitStopFrame = 10;
int BBConfigManager::splitInitSpeed = 40;
int BBConfigManager::minSplitMass = 20;


int BBConfigManager::ballMoveRate = 2;
double BBConfigManager::ballEatRate = 0.05;

bool BBConfigManager::isOpenLog = false;
int BBConfigManager::maxPredictionCnt = 10;
int BBConfigManager::maxOtherStepCommandCnt = 3;
int BBConfigManager::maxCommandNoAck = 15;

int BBConfigManager::minCatchUpTicks = 3;
int BBConfigManager::maxCatchUpTicks = 5;
int BBConfigManager::otherTooSlowTicks = 10;

int BBConfigManager::protectTime = 75;
int BBConfigManager::protectGripRadius = 60;
int BBConfigManager::mergeCdTime = 12000;

int BBConfigManager::debugMyBallMass = 0;
int BBConfigManager::debugOtherBallMass = 0;

BBConfigManager::BBConfigManager()
{
}


BBConfigManager::~BBConfigManager()
{
}

void BBConfigManager::SetClientDebug(bool isDebug)
{
	BBConfigManager::clientDebug = isDebug;
}

void BBConfigManager::SetFixLength(int cnt)
{
	BBConfigManager::fixLength = cnt;
}

void BBConfigManager::SetSpikyTime(int cnt)
{
	BBConfigManager::spikyTime = cnt;
}

void BBConfigManager::SetGameWH(int w, int h)
{
	BBConfigManager::gameWidth = w;
	BBConfigManager::gameHeight = h;
}

void BBConfigManager::SetMaxBallCount(int cnt)
{
	BBConfigManager::maxBallCount = cnt;
}

void BBConfigManager::SetFoodCount(int cnt)
{
	BBConfigManager::foodCount = cnt;
}

void BBConfigManager::SetFoodMass(int mass)
{
	BBConfigManager::foodMass = mass;
}

void BBConfigManager::SetSpikyCount(int cnt)
{
	BBConfigManager::spikyCount = cnt;
}

void BBConfigManager::SetInitBallMass(int mass)
{
	BBConfigManager::initBallMass = mass;
}

void BBConfigManager::SetMinSpikyMass(int mass)
{
	BBConfigManager::minSpikyMass = mass;
}

void BBConfigManager::SetMaxSpikyMass(int mass)
{
	BBConfigManager::maxSpikyMass = mass;
}

void BBConfigManager::SetMaxSpikySplitChildCount(int cnt)
{
	BBConfigManager::maxSpikySplitChildCount = cnt;
}

void BBConfigManager::SetMinShootBallMass(int mass)
{
	BBConfigManager::minShootBallMass = mass;
}

void BBConfigManager::SetSporeMass(int mass)
{
	BBConfigManager::sporeMass = mass;
}

void BBConfigManager::SetSporeStopFrame(int frame)
{
	BBConfigManager::sporeStopFrame = frame;
}

void BBConfigManager::SetSporeInitSpeed(int speed)
{
	BBConfigManager::sporeInitSpeed = speed;
}

void BBConfigManager::SetSporeCdTime(int time)
{
	BBConfigManager::sporeCdTime = time;
}

void BBConfigManager::SetSplitStopFrame(int frame)
{
	BBConfigManager::splitStopFrame = frame;
}

void BBConfigManager::SetMinSplitMass(int mass)
{
	BBConfigManager::minSplitMass = mass;
}

void BBConfigManager::SetSplitInitSpeed(int speed)
{
	BBConfigManager::splitInitSpeed = speed;
}

void BBConfigManager::SetBallMoveRate(int rate)
{
	BBConfigManager::ballMoveRate = rate;
}

void BBConfigManager::SetBallEatRate(double rate)
{
	BBConfigManager::ballEatRate = rate;
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

void BBConfigManager::SetProtectGripRadius(int radius)
{
	BBConfigManager::protectGripRadius = radius;
}

void BBConfigManager::SetMergeCdTime(int cnt)
{
	BBConfigManager::mergeCdTime = cnt;
}

void BBConfigManager::SetDebugMyBallMass(int cnt)
{
	BBConfigManager::debugMyBallMass = cnt;
}

void BBConfigManager::SetDebugOtherBallMas(int cnt)
{
	BBConfigManager::debugOtherBallMass = cnt;
}

void BBConfigManager::SetOtherTooSlowTicks(int cnt)
{
	BBConfigManager::otherTooSlowTicks = cnt;
}