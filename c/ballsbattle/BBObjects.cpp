#include "BBObjects.h"
#include "BBMathUtils.h"

Food::Food()
{
}
Food::~Food()
{

}

BaseBall::BaseBall():
	mass(0),
	FromMass(0)
{

}
BaseBall::~BaseBall()
{

}

void BaseBall::SetMass(int _mass)
{
	mass = _mass;
	radius = BBMathUtils::Mass2Radius(mass);
}

BBRect BaseBall::FoodBound(int extraRadius)
{
	return BBRect(minX - extraRadius, minY - extraRadius, maxX + extraRadius, maxY + extraRadius);
}


SpikyBall::SpikyBall()
{

}
SpikyBall::~SpikyBall()
{

}