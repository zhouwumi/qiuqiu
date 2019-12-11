#include "BBRect.h"



BBRect::BBRect()
{
	setRect(0, 0, 0, 0);
}

BBRect::BBRect(int minX, float minY, int maxX, int maxY)
{
	setRect(minX, minY, maxX, maxY);
}

BBRect::BBRect(const BBRect& other)
{
	setRect(other.minX, other.minY, other.maxX, other.maxY);
}

BBRect& BBRect::operator= (const BBRect& other)
{
	setRect(other.minX, other.minY, other.maxX, other.maxY);
	return *this;
}

void BBRect::setRect(int minX, int minY, int maxX, int maxY)
{
	this->minX = minX;
	this->minY = minY;
	this->maxX = maxX;
	this->maxY = maxY;
	centerX = (minX + maxX) / 2;
	centerY = (minY + maxY) / 2;
}

BBRect::~BBRect()
{
}

int BBRect::GetMinX()
{
	return minX;
}
int BBRect::GetMaxX()
{
	return maxX;
}
int BBRect::GetMinY()
{
	return minY;
}
int BBRect::GetMaxY()
{
	return maxY;
}
int BBRect::GetCenterX()
{
	return centerX;
}
int BBRect::GetCenterY()
{
	return centerY;
}

const BBRect BBRect::ZERO = BBRect(0, 0, 0, 0);