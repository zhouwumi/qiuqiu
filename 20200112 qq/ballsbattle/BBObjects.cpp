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

void BaseBall::SetBallMass(int _mass)
{
	mass = _mass;
	radius = BBMathUtils::Mass2Radius(mass);
}


SpikyBall::SpikyBall()
{

}
SpikyBall::~SpikyBall()
{

}