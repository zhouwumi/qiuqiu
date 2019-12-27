#include "BBMovingBall.h"
#include "BBMathUtils.h"
#include<math.h>

MovingBall::MovingBall():
	Init(0),
	FromId(0),
	speed(0)
{
	FromLocation = BBPoint::ZERO;
	Direction = BBVector::ZERO;
	Current = BBVector::ZERO;
	Delta = BBVector::ZERO;
	Final = BBVector::ZERO;
}
MovingBall::~MovingBall()
{

}

void MovingBall::Move(BBVector v)
{
	ChangePosition(positionX + v.x, positionY + v.y, radius);
}

void MovingBall::UpdateLocation(BBPoint v)
{
	ChangePosition(v.x, v.y, radius);
}

void MovingBall::SetSpeedVec(int targetX, int targetY)
{
	Current = BBVector::GetFixedVetor2(BBVector(targetX, targetY), speed);
	if (Current != BBVector::ZERO)
	{
		Direction = Current;
	}
}

void MovingBall::SetMass(int _mass)
{
	mass = _mass;
	radius = BBMathUtils::Mass2Radius(mass);
	speed = BBMathUtils::Mass2Speed(mass);
}

void MovingBall::ChangeMass(int deltaMas)
{
	float rawspeed = BBMathUtils::Mass2Speed(mass);
	SetMass(mass + deltaMas);
	float newSpeed = BBMathUtils::Mass2Speed(mass);
	float deltaX = newSpeed / rawspeed;
	Current *= deltaX;
	Delta *= deltaX;
	Final *= deltaX;
}

void MovingBall::Stop()
{
	Current = BBVector::ZERO;
	Delta = BBVector::ZERO;
	Final = BBVector::ZERO;
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
	Final = Current;
	return Current;
}

void Spore::CalculateInitMoveParams(int radius, int frame, float initSpeed, int finalSpeed)
{
	float init = initSpeed;
	float _final = finalSpeed;
	float delta = fabs((finalSpeed - initSpeed) / frame);
	delta = radius / float(frame * (frame - 1) / 2) + delta;
	init = _final + frame * delta;

	this->initSpeed = init;
	this->initDeltaSpeed = delta;
}
