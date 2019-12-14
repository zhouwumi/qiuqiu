#include "BBObjectManager.h"
#include "BBGameManager.h"
#include "BBConst.h"
#include "BBPlayerNode.h"

BBObjectManager::BBObjectManager():
	_curObjIdx(1)
{
}


BBObjectManager::~BBObjectManager()
{
}

unsigned int BBObjectManager::_getNextObjIdx()
{
	return _curObjIdx++;
}

Food* BBObjectManager::CreateFood(int x, int y)
{
	int posKey = y * BBConst::MaxWidth + x;
	Food* newFood = new Food();
	newFood->posKey = posKey;
	newFood->Idx = _getNextObjIdx();
	newFood->radius = 0;
	newFood->ChangePosition(x, y);
	return newFood;
}

SpikyBall* BBObjectManager::CreateSpikyBall(int x, int y, int mass)
{
	SpikyBall* newSpikyBall = new SpikyBall();
	newSpikyBall->SetMass(mass);

	newSpikyBall->ChangePosition(x, y);
	newSpikyBall->Idx = _getNextObjIdx();
	return newSpikyBall;
}

Player* BBObjectManager::CreatePlayer(int uid)
{
	Player* player = new Player(uid);
	return player;
}

PlayerNode* BBObjectManager::CreatePlayerNode(int uid, int mass, int cd, const BBPoint& FromLocation, const BBVector& Current, 
	const BBVector& Delta, const BBVector& Final, const BBVector& Direction)
{
	PlayerNode* newPlayerNode = new PlayerNode();
	newPlayerNode->Uid = uid;
	newPlayerNode->SetMass(mass);
	newPlayerNode->Idx = _getNextObjIdx();
	newPlayerNode->FromLocation = FromLocation;
	newPlayerNode->Current = Current;
	newPlayerNode->Delta = Delta;
	newPlayerNode->Final = Final;
	newPlayerNode->Direction = Direction;
	newPlayerNode->Cd = cd;
	return newPlayerNode;
}

PlayerNode* BBObjectManager::CreatePlayerNode(int uid, int mass)
{
	return CreatePlayerNode(uid, mass, 0, BBPoint::ZERO, BBVector::ZERO, BBVector::ZERO, BBVector::ZERO, BBVector::ZERO);
}