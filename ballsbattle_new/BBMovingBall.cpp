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
	SetRadius(BBMathUtils::Mass2Radius(mass));
	speed = BBMathUtils::Mass2Speed(mass);
}

void MovingBall::ChangeDeltaMass(int deltaMas)
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
void Spore::InitMove()
{
	initSpeed -= initDeltaSpeed;
	Current = BBVector::GetFixedVetor2(Direction, initSpeed);
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