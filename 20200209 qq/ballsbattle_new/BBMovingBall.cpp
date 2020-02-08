#include "BBMovingBall.h"
#include "BBMathUtils.h"
#include<math.h>

MovingBall::MovingBall():
	fromId(0),
	speed(0)
{
	//direction = BBVector::ZERO;
	currentSpeedVec = BBVector::ZERO;
}
MovingBall::~MovingBall()
{

}

void MovingBall::SetSpeedVec(float targetX, float targetY)
{
	currentSpeedVec = BBVector::GetFixedVetor2(BBVector(targetX, targetY), speed);
	BBMathUtils::bb_fix_bb_vector(currentSpeedVec);
}

void MovingBall::SetBallMass(int _mass)
{
	mass = _mass;
	SetRadius(BBMathUtils::Mass2Radius(mass));
	speed = BBMathUtils::Mass2Speed(mass);
}

void MovingBall::ChangeDeltaMass(int deltaMas)
{
	float rawspeed = speed;
    SetBallMass(mass + deltaMas);
	float newSpeed = speed;
	float deltaX = newSpeed / rawspeed;
	currentSpeedVec *= deltaX;
	BBMathUtils::bb_fix_bb_vector(currentSpeedVec);
}

Spore::Spore():
	initStopFrame(0)
{

}

Spore::~Spore()
{

}
void Spore::InitMove()
{
	initSpeed -= initDeltaSpeed;
	currentSpeedVec = BBVector::GetFixedVetor2(currentSpeedVec, initSpeed);
	BBMathUtils::bb_fix_bb_vector(currentSpeedVec);
	ChangePosition(location.x + currentSpeedVec.x, location.y + currentSpeedVec.y);
}

void Spore::CalculateInitMoveParams(int radius, int frame, int initSpeed)
{
	float delta = fabs((float)(initSpeed) / frame);
	delta = radius / float(frame * (frame - 1) / 2) + delta;

	int intDelta = delta;
	int init = frame * intDelta;

	this->initSpeed = init;
	this->initDeltaSpeed = intDelta;
}