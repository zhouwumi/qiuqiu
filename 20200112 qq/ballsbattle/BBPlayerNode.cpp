#include "BBPlayerNode.h"
#include<math.h>

#include "BBConst.h"
#include "BBGameManager.h"
#include "BBMathUtils.h"

Player::Player(int id):
	//Respawn(0),
	uid(id),
	Stopped(true),
	IsBalance(true)
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
	if (x != FinalPoint.x || y != FinalPoint.y)
	{
		IsBalance = false;
	}
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

void Player::CopyFromOtherPlayer(Player* otherPlayer)
{
	this->uid = otherPlayer->uid;
	this->Stopped = otherPlayer->Stopped;
	this->FinalPoint = otherPlayer->FinalPoint;
	this->Direction = otherPlayer->Direction;
	this->NMass = otherPlayer->NMass;
	this->IsBalance = otherPlayer->IsBalance;
	for (int j = 0; j < otherPlayer->vecPlayerNodes.size(); j++)
	{
		PlayerNode* playerNode = new PlayerNode();
		playerNode->CopyPlayerNode(otherPlayer->vecPlayerNodes[j]);
		this->vecPlayerNodes.emplace_back(playerNode);
	}
}



PlayerNode::PlayerNode():
	Cd(0),
	Protect(0),
	initSpeed(0),
	initDeltaSpeed(0),
	isHit(false)
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

void PlayerNode::MakePhoto(int keyFrame)
{
	PlayerNodeData data;
	data.Cd = this->Cd;
	data.Current = this->Current;
	data.Direction = this->Direction;
	data.Protect = this->Protect;
	data.initSpeed = this->initSpeed;
	data.initDeltaSpeed = this->initDeltaSpeed;
	data.Init = this->Init;
	data.Location = this->Location;
	data.mass = this->mass;
	frameCacheDatas.emplace(keyFrame, data);
}

void PlayerNode::CopyPlayerNode(PlayerNode* otherPlayerNode)
{
	this->Idx = otherPlayerNode->Idx;
	this->FromId = otherPlayerNode->FromId;
	this->Cd = otherPlayerNode->Cd;
	this->Protect = otherPlayerNode->Protect;
	this->initDeltaSpeed = otherPlayerNode->initDeltaSpeed;
	this->initSpeed = otherPlayerNode->initSpeed;
	this->Current = otherPlayerNode->Current;
	this->Direction = otherPlayerNode->Direction;
	this->Init = otherPlayerNode->Init;
	this->speed = otherPlayerNode->speed;
	this->SetBallMass(otherPlayerNode->mass);
	this->ChangePosition(otherPlayerNode->Location.x, otherPlayerNode->Location.y);
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

void PlayerNode::ResetPhoto(int keyFrame)
{
	if (frameCacheDatas.find(keyFrame) == frameCacheDatas.end())
	{
		return;
	}
	PlayerNodeData& data = frameCacheDatas[keyFrame];
	this->Cd = data.Cd;
	this ->Current = data.Current;
	this->Direction = data.Direction;
	this->Protect = data.Protect;
	this->initSpeed = data.initSpeed;
	this->initDeltaSpeed = data.initDeltaSpeed;
	this->Init = data.Init;
	this->ChangePosition(data.Location.x, data.Location.y);
	this->SetBallMass(data.mass);

	frameCacheDatas.erase(keyFrame);
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


void Player::MakePhoto(int keyFrame)
{
	PlayerData data;
	data.Stopped = this->Stopped;
	data.NMass = this->NMass;
	data.Direction = this->Direction;
	data.IsBalance = this->IsBalance;
	frameCacheDatas.emplace(keyFrame, data);
	for (int i = 0; i < vecPlayerNodes.size(); i++)
	{
		PlayerNode* node = vecPlayerNodes[i];
		node->MakePhoto(keyFrame);
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

void Player::ResetPhoto(int keyFrame)
{
	if (frameCacheDatas.find(keyFrame) == frameCacheDatas.end())
	{
		return;
	}
	PlayerData& playerData = frameCacheDatas[keyFrame];
	this->Stopped = playerData.Stopped;
	this->Direction = playerData.Direction;
	this->IsBalance = playerData.IsBalance;
	for (int i = 0; i < vecPlayerNodes.size(); i++)
	{
		PlayerNode* node = vecPlayerNodes[i];
		node->ResetPhoto(keyFrame);
	}
	frameCacheDatas.erase(keyFrame);
}

