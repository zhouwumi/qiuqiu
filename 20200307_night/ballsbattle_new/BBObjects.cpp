#include "BBObjects.h"
#include "BBMathUtils.h"

Food::Food()
{
}
Food::~Food()
{

}

BaseBall::BaseBall():
	mass(0)
{

}
BaseBall::~BaseBall()
{

}

void BaseBall::SetBallMass(int mass)
{
	this->mass = mass;
	SetRadius(BBMathUtils::Mass2Radius(mass));
}


SpikyBall::SpikyBall()
{

}
SpikyBall::~SpikyBall()
{

}