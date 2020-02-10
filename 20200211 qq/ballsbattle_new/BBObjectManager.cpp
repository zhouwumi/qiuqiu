#include "BBObjectManager.h"
#include "BBGameManager.h"
#include "BBConst.h"
#include "BBPlayerNode.h"
#include "BBMathUtils.h"

BBObjectManager::BBObjectManager():
	_curObjIdx(1),
	_nextSpikyIdx(1),
	_nextPlayerIdx(1)
{
}


BBObjectManager::~BBObjectManager()
{
}

Food* BBObjectManager::CreateFood(int x, int y)
{
	int idx = BBMathUtils::xy_to_location(x, y);
	Food* newFood = new Food();
	newFood->idx = idx;
	newFood->SetRadius(0);
	newFood->ChangePosition(x, y);
	return newFood;
}

SpikyBall* BBObjectManager::CreateSpikyBall(int x, int y, int mass)
{
	int idx = GetNextSpikyIdx();
	return CreateSpikyBall(x, y, mass, idx);
}

SpikyBall* BBObjectManager::CreateSpikyBall(int x, int y, int mass, int idx)
{
	SpikyBall* newSpikyBall = new SpikyBall();
    newSpikyBall->SetBallMass(mass);

	newSpikyBall->ChangePosition(x, y);
	newSpikyBall->idx = idx;
	return newSpikyBall;
}

Spore* BBObjectManager::CreateSpore(BBPlayerNode* sourceNode)
{
	Spore* newSpore = new Spore();
	newSpore->fromId = sourceNode->idx;
	newSpore->uid = sourceNode->uid;
	BBVector moveVec = BBVector::GetFixedVetor2(sourceNode->currentSpeedVec, sourceNode->GetRadius());
	newSpore->ChangePosition(sourceNode->location.x + moveVec.x, sourceNode->location.y + moveVec.y);
	if (sourceNode->currentSpeedVec == BBVector::ZERO) {
		newSpore->currentSpeedVec = BBVector::GetFixedVetor2(BBVector(1, 0), BBMathUtils::Mass2Speed(BBConst::SporeMass));
		BBMathUtils::bb_fix_bb_vector(newSpore->currentSpeedVec);
	}
	else {
		newSpore->currentSpeedVec = sourceNode->currentSpeedVec;
		BBMathUtils::bb_fix_bb_vector(newSpore->currentSpeedVec);
	}
    newSpore->SetBallMass(BBConst::SporeMass);
	newSpore->idx = sourceNode->player->GetNextPlayerNodeIdx();
	//newSpore->currentSpeedVec = newSpore->direction;
	
	newSpore->cd = BBConst::SporeCd;
	newSpore->CalculateInitMoveParams(newSpore->GetRadius(), BBConst::SporeStopFrame, BBConst::SporeInitSpeed);
	return newSpore;
}