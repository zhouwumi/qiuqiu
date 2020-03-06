#include "BBPoint.h"

BBPoint::BBPoint()
{
	SetPoint(0, 0);
}

BBPoint::BBPoint(int _x, int _y)
{
	SetPoint(_x, _y);
}

BBPoint::BBPoint(const BBPoint& other)
{
	SetPoint(other.x, other.y);
}

BBPoint& BBPoint::operator= (const BBPoint& other)
{
	SetPoint(other.x, other.y);
	return *this;
}


void BBPoint::SetPoint(int _x, int _y)
{
	x = _x;
	y = _y;
}

const BBPoint BBPoint::ZERO = BBPoint(0, 0);