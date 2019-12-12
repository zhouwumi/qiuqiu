#include "BBRect.h"

#include<math.h>

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

bool BBRect::intersectsRect(const BBRect& rect) const
{
	return !(maxX > rect.minX || rect.maxX < minX || maxY < rect.minY || rect.maxY < minY);
}

bool BBRect::intersectsCircle(const BBPoint& center, int radius) const
{
	int w = abs(centerX - minX);
	int h = abs(centerY - minY);

	int dx = abs(centerX - center.x);
	int dy = abs(centerY - center.y);

	if (dx > (radius + w) || dy > (radius + h))
	{
		return false;
	}
	
	if (dx <= w || dy <= h)
	{
		return true;
	}
	int distance = pow(centerX - w, 2) + pow(centerY - h, 2);
	return distance <= pow(radius, 2);
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

BBRect::~BBRect()
{
}

const BBRect BBRect::ZERO = BBRect(0, 0, 0, 0);