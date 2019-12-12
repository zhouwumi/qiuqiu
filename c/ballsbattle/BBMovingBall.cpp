#include "BBMovingBall.h"
#include "BBMathUtils.h"
MovingBall::MovingBall():
	Init(0),
	FromId(0),
	speed(0)
{

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
BBVector Spore::InitMove()
{
	return BBVector::ZERO;
}