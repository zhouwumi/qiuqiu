#include "BBObjectManager.h"
#include "BBGameManager.h"
#include "BBConst.h"
#include "BBPlayerNode.h"
#include "BBMathUtils.h"

BBObjectManager::BBObjectManager():
	_curObjIdx(1)
{
}


BBObjectManager::~BBObjectManager()
{
}

unsigned int BBObjectManager::GetNextObjIdx()
{
	return _curObjIdx++;
}

Food* BBObjectManager::CreateFood(int x, int y)
{
	int posKey = y * BBConst::MaxWidth + x;
	Food* newFood = new Food();
	newFood->posKey = posKey;
	newFood->Idx = GetNextObjIdx();
	newFood->radius = 0;
	newFood->ChangePosition(x, y);
	return newFood;
}

SpikyBall* BBObjectManager::CreateSpikyBall(int x, int y, int mass)
{
	SpikyBall* newSpikyBall = new SpikyBall();
	newSpikyBall->SetMass(mass);

	newSpikyBall->ChangePosition(x, y);
	newSpikyBall->Idx = GetNextObjIdx();
	return newSpikyBall;
}

Player* BBObjectManager::CreatePlayer(int uid)
{
	Player* player = new Player(uid);
	return player;
}

PlayerNode* BBObjectManager::CreateSimplePlayerNode()
{
	PlayerNode* newPlayerNode = new PlayerNode();
	newPlayerNode->Idx = GetNextObjIdx();
	return newPlayerNode;
}

PlayerNode* BBObjectManager::CreatePlayerNode(int uid, int mass, int cd, const BBPoint& FromLocation, const BBVector& Current, 
	const BBVector& Delta, const BBVector& Final, const BBVector& Direction)
{
	PlayerNode* newPlayerNode = new PlayerNode();
	newPlayerNode->Uid = uid;
	newPlayerNode->SetMass(mass);
	newPlayerNode->Idx = GetNextObjIdx();
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
	mass = 200;
	return CreatePlayerNode(uid, mass, 0, BBPoint::ZERO, BBVector::ZERO, BBVector::ZERO, BBVector::ZERO, BBVector::ZERO);
}

Spore* BBObjectManager::CreateSpore(PlayerNode* sourceNode)
{
	Spore* newSpore = new Spore();
	newSpore->FromId = sourceNode->Idx;
	newSpore->Uid = sourceNode->Uid;
	BBVector moveVec = BBVector::GetFixedVetor2(sourceNode->Direction, sourceNode->radius);
	newSpore->FromLocation.x = sourceNode->Location.x + moveVec.x;
	newSpore->FromLocation.y = sourceNode->Location.y + moveVec.y;
	newSpore->ChangePosition(newSpore->FromLocation.x, newSpore->FromLocation.y);
	if (sourceNode->Direction == BBVector::ZERO) {
		newSpore->Direction = BBVector::GetFixedVetor2(BBVector(1, 0), BBMathUtils::Mass2Speed(BBConst::SporeMass));
	}
	else {
		newSpore->Direction = sourceNode->Direction;
	}
	newSpore->SetMass(BBConst::SporeMass);
	newSpore->Idx = GetNextObjIdx();
	newSpore->Current = newSpore->Direction;
	newSpore->Delta = BBVector::ZERO;
	newSpore->Final = newSpore->Direction;
	newSpore->Init = BBConst::SporeStopFrame;
	newSpore->Cd = BBConst::SporeCd;
	newSpore->CalculateInitMoveParams(newSpore->radius, BBConst::SporeStopFrame, BBConst::SporeInitSpeed, BBConst::SporeStopSpeed);
	return newSpore;
}