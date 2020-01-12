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
	int posKey = BBMathUtils::xy_to_location(x, y);
	Food* newFood = new Food();
	newFood->posKey = posKey;
	newFood->Idx = posKey;
	newFood->radius = 0;
	newFood->ChangePosition(x, y);
	return newFood;
}

SpikyBall* BBObjectManager::CreateSpikyBall(int x, int y, int mass)
{
	int idx = GetNextObjIdx();
	return CreateSpikyBall(x, y, mass, idx);
}

SpikyBall* BBObjectManager::CreateSpikyBall(int x, int y, int mass, int idx)
{
	SpikyBall* newSpikyBall = new SpikyBall();
    newSpikyBall->SetBallMass(mass);

	newSpikyBall->ChangePosition(x, y);
	newSpikyBall->Idx = idx;
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

PlayerNode* BBObjectManager::CreatePlayerNode(int uid, int mass, int cd, const BBVector& Current, 
	const BBVector& Delta, const BBVector& Final, const BBVector& Direction)
{
	PlayerNode* newPlayerNode = new PlayerNode();
	newPlayerNode->Uid = uid;
    newPlayerNode->SetBallMass(mass);
	newPlayerNode->Idx = GetNextObjIdx();
	newPlayerNode->Current = Current;
	newPlayerNode->Direction = Direction;
	newPlayerNode->Cd = cd;
	return newPlayerNode;
}

PlayerNode*  BBObjectManager::CreatePlayerNode(int uid, int mass, int idx)
{
	PlayerNode* newPlayerNode = new PlayerNode();
	newPlayerNode->Uid = uid;
	newPlayerNode->SetBallMass(mass);
	newPlayerNode->Idx = idx;
	return newPlayerNode;
}

PlayerNode* BBObjectManager::CreatePlayerNode(int uid, int mass)
{
	//mass = 200;
	//return CreatePlayerNode(uid, mass, 0, BBPoint::ZERO, BBVector::ZERO, BBVector::ZERO, BBVector::ZERO, BBVector::ZERO);
	return CreatePlayerNode(uid, mass, GetNextObjIdx());
}

Spore* BBObjectManager::CreateSpore(PlayerNode* sourceNode)
{
	Spore* newSpore = new Spore();
	newSpore->FromId = sourceNode->Idx;
	newSpore->Uid = sourceNode->Uid;
	BBVector moveVec = BBVector::GetFixedVetor2(sourceNode->Direction, sourceNode->radius);
	newSpore->ChangePosition(sourceNode->Location.x + moveVec.x, sourceNode->Location.y + moveVec.y);
	if (sourceNode->Direction == BBVector::ZERO) {
		newSpore->Direction = BBVector::GetFixedVetor2(BBVector(1, 0), BBMathUtils::Mass2Speed(BBConst::SporeMass));
	}
	else {
		newSpore->Direction = sourceNode->Direction;
	}
    newSpore->SetBallMass(BBConst::SporeMass);
	newSpore->Idx = GetNextObjIdx();
	newSpore->Current = newSpore->Direction;
	newSpore->Init = BBConst::SporeStopFrame;
	newSpore->Cd = BBConst::SporeCd;
	newSpore->CalculateInitMoveParams(newSpore->radius, BBConst::SporeStopFrame, BBConst::SporeInitSpeed, BBConst::SporeStopSpeed);
	return newSpore;
}