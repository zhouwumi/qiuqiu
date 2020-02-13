#include "BBMovingBall.h"
#include "BBMathUtils.h"
#include<math.h>
#include "BBGameManager.h"

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

void MovingBall::SetSpeedVec(double targetX, double targetY)
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
	double rawspeed = speed;
    SetBallMass(mass + deltaMas);
	double newSpeed = speed;
	double deltaX = newSpeed / rawspeed;
	currentSpeedVec *= deltaX;
	BBMathUtils::bb_fix_bb_vector(currentSpeedVec);
}

int MovingBall::GetIntSpeedX()
{
	return BBMathUtils::bb_float_to_int(currentSpeedVec.x);
}
int MovingBall::GetIntSpeedY()
{
	return BBMathUtils::bb_float_to_int(currentSpeedVec.y);
}

int MovingBall::GetIntLocationX()
{
	return BBMathUtils::bb_float_to_int(this->location.x);
}

int MovingBall::GetIntLocationY()
{
	return BBMathUtils::bb_float_to_int(this->location.y);
}

int MovingBall::GetRenderX()
{
	return BBMathUtils::bb_float_to_int(mDeltaData.fixedLocation.x);
}

void MovingBall::ChangeRenderPosition(double x, double y)
{
	double new_x = BBMathUtils::bb_fix_float(x);
	double new_y = BBMathUtils::bb_fix_float(y);
	mDeltaData.location.x = new_x;
	mDeltaData.location.y = new_y;
}

int MovingBall::GetRenderY()
{
	return BBMathUtils::bb_float_to_int(mDeltaData.fixedLocation.y);
}

Spore::Spore():
	initStopFrame(0),
	uid(0),
	cd(0),
	initDeltaSpeed(0),
	initSpeed(0)
{

}

Spore::~Spore()
{

}
void Spore::InitMove()
{
	currentSpeedVec = BBVector::GetFixedVetor2(currentSpeedVec, initSpeed);
	BBMathUtils::bb_fix_bb_vector(currentSpeedVec);
	ChangePosition(location.x + currentSpeedVec.x, location.y + currentSpeedVec.y);
	initStopFrame -= 1;
	initSpeed -= initDeltaSpeed;
	BBMathUtils::BBLOG("spore pos  %i, %f-%f", idx, location.x, location.y);
}

void Spore::CalculateInitMoveParams(int radius, int frame, int initSpeed)
{
	initStopFrame = frame;
	double delta = fabs((double)(initSpeed) / frame);
	//delta = radius / double(frame * (frame - 1) / 2) + delta;

	int intDelta = delta;
	int init = frame * intDelta;

	this->initSpeed = init;
	this->initDeltaSpeed = intDelta;
}

void Spore::ChangeRenderPosition(double x, double y)
{
	double new_x = BBMathUtils::bb_fix_float(x);
	double new_y = BBMathUtils::bb_fix_float(y);
	mDeltaData.location.x = new_x;
	mDeltaData.location.y = new_y;

	double fixedX, fixedY;
	BBMathUtils::FixCircleDefaultRect(new_x, new_y, this->GetRadius(), fixedX, fixedY);
	mDeltaData.fixedLocation.x = fixedX;
	mDeltaData.fixedLocation.y = fixedY;
}

void Spore::CalcBallDelta()
{
	mDeltaData.wrapTicks = initStopFrame;
	mDeltaData.wrapLocationOffset.x = (location.x - mDeltaData.location.x) / mDeltaData.wrapTicks;
	mDeltaData.wrapLocationOffset.y = (location.y - mDeltaData.location.y) / mDeltaData.wrapTicks;
}

