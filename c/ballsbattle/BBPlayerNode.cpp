#include "BBPlayerNode.h"
#include<math.h>

#include "BBConst.h"
#include "BBGameManager.h"
#include "BBMathUtils.h"

Player::Player(int id):
	Respawn(0),
	uid(id),
	Stopped(true)
{

}

Player::~Player()
{

}

void Player::ResetPoint(int x, int y)
{
	FromPoint.SetPoint(x, y);
	FinalPoint.SetPoint(x, y);
}

void Player::UpdateFinalPoint(int x, int y)
{
	FromPoint = FinalPoint;
	FinalPoint.SetPoint(x, y);
}

BBRect Player::GetGroupRect()
{
	BBRect ret;
	if (vecPlayerNodes.size() <= 0)
	{
		return ret;
	}
	PlayerNode* node = vecPlayerNodes[0];
	ret.setRect(node->minX, node->minY, node->maxX, node->maxY);
	for (int i = 1; i < vecPlayerNodes.size(); i++)
	{
		PlayerNode* stepNode = vecPlayerNodes[i];
		if (ret.minX > stepNode->minX) {
			ret.minX = stepNode->minX;
		}
		if (ret.maxX < stepNode->maxX) {
			ret.maxX = stepNode->maxX;
		}
		if (ret.minY > stepNode->minY) {
			ret.minY = stepNode->minY;
		}
		if (ret.maxY < stepNode->maxY) {
			ret.maxY = stepNode->maxY;
		}
	}
	ret.centerX = (ret.minX + ret.maxX) / 2;
	ret.centerY = (ret.minY + ret.maxY) / 2;
	return ret;
}

PlayerNode::PlayerNode():
	Cd(0),
	Protect(0),
	initSpeed(0),
	initDeltaSpeed(0)
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

void Player::RemoveMass()
{
	if (vecPlayerNodes.size() == 0)
	{
		return;
	}
	int maxMass = -1;
	int maxMassIdx = -1;
	PlayerNode* maxPlayerNode = NULL;
	for (int i = 0; i < vecPlayerNodes.size(); i++)
	{
		PlayerNode* node = vecPlayerNodes[i];
		if (maxMass < node->mass)
		{
			maxMass = node->mass;
			maxMassIdx = node->Idx;
			maxPlayerNode = node;
		}
		else if (maxMass == node->mass && node->Idx < maxMassIdx)
		{
			maxMassIdx = node->Idx;
			maxPlayerNode = node;
		}
	}
	int massToRemove = maxMass * 2;
	int delta = (massToRemove + NMass) / 1000;
	if (delta > 0)
	{
		NMass = (massToRemove + NMass) % 1000;
		if (maxMass - delta >= BBConst::InitMass)
		{
			maxPlayerNode->ChangeMass(-delta);
		}
	}
	else {
		NMass += massToRemove;
	}
}