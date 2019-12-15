#include "BBPlayerNode.h"
#include<math.h>

#include "BBConst.h"
#include "BBGameManager.h"
#include "BBMathUtils.h"

Player::Player(int id):
	uid(id),
	Stopped(true)
{

}

Player::~Player()
{

}

void Player::ResetPoint(int x, int y)
{
	FromPoint.SetPoint(0, 0);
	FinalPoint.SetPoint(0, 0);
}

void Player::UpdateFinalPoint(int x, int y)
{
	FromPoint = FinalPoint;
	FinalPoint.SetPoint(x, y);
}

PlayerNode::PlayerNode()
{

}
PlayerNode::~PlayerNode()
{

}

void PlayerNode::ChangeCd(int timeDelta)
{
	int cd = roundf((timeDelta * mass * BBConst::FPS) / 1000.0f);
	Cd = cd;
}

void PlayerNode::SpikySplit(int maxChildNode, int spikyMass, std::vector<PlayerNode*>& newPlayerNodes)
{
	if (mass < spikyMass)
	{
		return;
	}
	if (maxChildNode == 0)
	{
		ChangeMass(spikyMass);
		return;
	}
	int assignMass = 2 * spikyMass;
	int childMass = assignMass / (maxChildNode + 1);
	if (childMass > 40)
	{
		childMass = 40;
	}
	int centerMass = mass + spikyMass - childMass * maxChildNode;
	ChangeMass(centerMass - mass);
	int splitAngle = ceilf(360.0f / maxChildNode);
	float directionAngle = Direction.GetAngle();
	for (int i = 0; i < maxChildNode; i++)
	{
		PlayerNode* newPlayerNode = BBGameManager::getInstance()->SpawnPlayerNode();
		newPlayerNode->Uid = this->Uid;
		newPlayerNode->Cd = 0;
		newPlayerNode->SetMass(childMass);
		newPlayerNode->Init = BBConst::SplitFrame;
		newPlayerNode->Direction = BBMathUtils::AngleToFixedVector(splitAngle * i + directionAngle, BBMathUtils::Mass2Speed(childMass));

		BBVector moveVec = BBVector::GetFixedVetor2(newPlayerNode->Direction, radius);
		newPlayerNode->FromLocation.x = Location.x + moveVec.x;
		newPlayerNode->FromLocation.y = Location.y + moveVec.y;
		newPlayerNode->Location = newPlayerNode->FromLocation;
		newPlayerNode->FromId = Idx;
		newPlayerNode->CalculateInitMoveParams(newPlayerNode->GetRadius(), BBConst::SplitFrame, BBConst::SplitInitSpeed, BBConst::SplitFinalSpeed);
		newPlayerNodes.emplace_back(newPlayerNode);
	}
}

PlayerNode* PlayerNode::SelfSplit()
{
	float childMas = mass / 2.0f;
	ChangeMass(-childMas);
	PlayerNode* newPlayerNode = BBGameManager::getInstance()->SpawnPlayerNode();
	newPlayerNode->Uid = Uid;
	if (Direction == BBVector::ZERO) {
		newPlayerNode->Direction = BBVector::GetFixedVetor2(BBVector(1, 0), BBMathUtils::Mass2Speed(childMas));
	}
	else {
		newPlayerNode->Direction = Direction;
	}
	newPlayerNode->SetMass(childMas);
	BBVector moveVec = BBVector::GetFixedVetor2(newPlayerNode->Direction, radius);
	newPlayerNode->FromLocation.x = Location.x + moveVec.x;
	newPlayerNode->FromLocation.y = Location.y + moveVec.y;
	newPlayerNode->Location = newPlayerNode->FromLocation;
	newPlayerNode->FromId = Idx;
	newPlayerNode->Init = BBConst::SplitFrame;
	newPlayerNode->Current = newPlayerNode->Direction;
	newPlayerNode->CalculateInitMoveParams(newPlayerNode->GetRadius(), BBConst::SplitFrame, BBConst::SplitInitSpeed, BBConst::SplitFinalSpeed);
	return newPlayerNode;
}

BBVector& PlayerNode::InitMove()
{
	initSpeed -= initDeltaSpeed;
	BBVector temp = BBVector::GetFixedVetor2(Direction, initSpeed);
	Current = temp;
	Final = Current;
	return Current;
}

void PlayerNode::CalculateInitMoveParams(int radius, int frame, float initSpeed, int finalSpeed)
{
	float init = initSpeed;
	float _final = finalSpeed;
	float delta = fabs((finalSpeed - initSpeed) / frame);
	delta = radius / float(frame * (frame - 1) / 2) + delta;
	init = _final + frame * delta;

	this->initSpeed = init;
	this->initDeltaSpeed = delta;
}