#include "BBPlayerNode.h"
#include<math.h>

#include "BBConst.h"
#include "BBMathUtils.h"

Player::Player(int id):
	//Respawn(0),
	uid(id),
	Stopped(true)
{

}

Player::~Player()
{

}

void Player::ResetPoint(int x, int y)
{
	FinalPoint.SetPoint(x, y);
}

void Player::UpdateFinalPoint(int x, int y)
{
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
	BBRect rect = node->rect;
	ret.setRect(rect.minX, rect.minY, rect.maxX, rect.maxY);
	for (int i = 1; i < vecPlayerNodes.size(); i++)
	{
		PlayerNode* stepNode = vecPlayerNodes[i];
		BBRect stepRect = stepNode->rect;
		if (ret.minX > stepRect.minX) {
			ret.minX = stepRect.minX;
		}
		if (ret.maxX < stepRect.maxX) {
			ret.maxX = stepRect.maxX;
		}
		if (ret.minY > stepRect.minY) {
			ret.minY = stepRect.minY;
		}
		if (ret.maxY < stepRect.maxY) {
			ret.maxY = stepRect.maxY;
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
	cd = (cd / 5) * 5;
	Cd = cd;
}

BBVector& PlayerNode::InitMove()
{
	initSpeed -= initDeltaSpeed;
	BBVector temp = BBVector::GetFixedVetor2(Direction, initSpeed);
	Current = temp;
	return Current;
}

void PlayerNode::CalculateInitMoveParams(int radius, int frame, int initSpeed, int finalSpeed)
{
	float delta = fabs((float)(finalSpeed - initSpeed) / frame);
	delta = radius / float(frame * (frame - 1) / 2) + delta;

	int intDelta = delta;
	int init = finalSpeed + frame * intDelta;

	this->initSpeed = init;
	this->initDeltaSpeed = delta;
}


unsigned int PlayerNode::GetCrc()
{
	static unsigned int playerNodeBuffer[4];
	unsigned int index = 0;
	playerNodeBuffer[index++] = this->Location.x;
	playerNodeBuffer[index++] = this->Location.y;

	unsigned int size = index * sizeof(int) / sizeof(char);
	unsigned int ret = BBMathUtils::crc32((char*)(&playerNodeBuffer), size);
	return ret;
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
		int mass = node->GetBallMass();
		if (maxMass < mass)
		{
			maxMass = mass;
			maxMassIdx = node->Idx;
			maxPlayerNode = node;
		}
		else if (maxMass == mass && node->Idx < maxMassIdx)
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
			maxPlayerNode->ChangeDeltaMass(-delta);
		}
	}
	else {
		NMass += massToRemove;
	}
}


unsigned int Player::GetCrc()
{
	static unsigned int playerBuffer[20 * 4];
	unsigned int index = 0;
	
	for (int i = 0; i < vecPlayerNodes.size(); i++)
	{
		PlayerNode* node = vecPlayerNodes[i];
		playerBuffer[index++] = node->GetCrc();
	}
	unsigned int size = index * sizeof(int) / sizeof(char);
	unsigned int ret = BBMathUtils::crc32((char*)(&playerBuffer), size);
	return ret;
}
