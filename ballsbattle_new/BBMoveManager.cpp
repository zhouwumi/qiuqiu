#include "BBMoveManager.h"

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
	BBMoveManager::isSplit = true;
}

void BBMoveManager::Shoot()
{
	BBMoveManager::isShoot = true;
}