#include "BBMoveManager.h"

bool BBMoveManager::isSplit = false;
bool BBMoveManager::isShoot = false;
int BBMoveManager::angle = 0;
int BBMoveManager::pressure = 0;
int BBMoveManager::nextIndex = 0;
unsigned int BBMoveManager::checkSum = 0;

void BBMoveManager::ResetManager()
{
	isSplit = false;
	isShoot = false;
	angle = 0;
	pressure = 0;
	nextIndex = 0;
}

void BBMoveManager::ResetFrame()
{
	isSplit = false;
	isShoot = false;
}

void BBMoveManager::IncreaseFrame()
{
	nextIndex++;
}

void BBMoveManager::SetMove(int angle, int pressure)
{
	BBMoveManager::angle = angle;
	BBMoveManager::pressure = pressure;
}

void BBMoveManager::Split()
{
	isSplit = true;
}

void BBMoveManager::Shoot()
{
	isShoot = true;
}

bool BBMoveManager::IsSplit()
{
	return isSplit;
}

bool BBMoveManager::IsShoot()
{
	return isShoot;
}

int BBMoveManager::GetAngle()
{
	return angle;
}

int BBMoveManager::GetPressure()
{
	return pressure;
}

int BBMoveManager::GetNextIndex()
{
	return nextIndex;
}

void BBMoveManager::SetCheckSum(unsigned int checkSum)
{
	BBMoveManager::checkSum = checkSum;
}

unsigned int BBMoveManager::GetCheckSum()
{
	return checkSum;
}