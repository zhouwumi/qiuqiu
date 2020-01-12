#include "BBMovingBall.h"
#include "BBMathUtils.h"
#include<math.h>

MovingBall::MovingBall():
	Init(0),
	FromId(0),
	speed(0)
{
	Direction = BBVector::ZERO;
	Current = BBVector::ZERO;
}
MovingBall::~MovingBall()
{

}

void MovingBall::SetSpeedVec(int targetX, int targetY)
{
	Current = BBVector::GetFixedVetor2(BBVector(targetX, targetY), speed);
	if (Current != BBVector::ZERO)
	{
		Direction = Current;
	}
}

void MovingBall::SetBallMass(int _mass)
{
	mass = _mass;
	radius = BBMathUtils::Mass2Radius(mass);
	speed = BBMathUtils::Mass2Speed(mass);
}

void MovingBall::ChangeMass(int deltaMas)
{
	float rawspeed = BBMathUtils::Mass2Speed(mass);
    SetBallMass(mass + deltaMas);
	float newSpeed = BBMathUtils::Mass2Speed(mass);
	float deltaX = newSpeed / rawspeed;
	Current *= deltaX;
}

Spore::Spore()
{

}

Spore::~Spore()
{

}
BBVector& Spore::InitMove()
{
	initSpeed -= initDeltaSpeed;
	BBVector temp = BBVector::GetFixedVetor2(Direction, initSpeed);
	Current = temp;
	return Current;
}

void Spore::CalculateInitMoveParams(int radius, int frame, int initSpeed, int finalSpeed)
{
	float delta = fabs((float)(finalSpeed - initSpeed) / frame);
	delta = radius / float(frame * (frame - 1) / 2) + delta;

	int intDelta = delta;
	int init = finalSpeed + frame * intDelta;

	this->initSpeed = init;
	this->initDeltaSpeed = delta;
}

void Spore::MakePhoto(int keyFrame)
{
	SporeData data;
	data.Init = this->Init;
	data.initSpeed = this->initSpeed;
	data.Cd = this->Cd;
	data.Current = this->Current;
	data.Direction = this->Direction;
	data.speed = this->speed;
	data.Location = this->Location;
	frameCacheDatas.emplace(keyFrame, data);
}

void Spore::ResetPhoto(int keyFrame)
{
	if (frameCacheDatas.find(keyFrame) == frameCacheDatas.end())
	{
		return;
	}
	SporeData& data = frameCacheDatas[keyFrame];
	this->Init = data.Init;
	this->Cd = data.Cd;
	this->initSpeed = data.initSpeed;
	this->Current = data.Current;
	this->Direction = data.Direction;
	this->Location = data.Location;
	this->speed = data.speed;
	frameCacheDatas.erase(keyFrame);
}