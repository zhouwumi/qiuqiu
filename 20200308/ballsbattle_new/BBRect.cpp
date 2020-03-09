#include "BBRect.h"
#include "math.h"
#include "BBMath.h"

BBRect::BBRect()
{
	setRect(0, 0, 0, 0);
}

BBRect::BBRect(int minX, int minY, int maxX, int maxY)
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

BBRect BBRect::expand(int expandW, int expandH)
{
	BBRect retRect;
	retRect.minX = minX - expandW;
	retRect.minY = minY - expandH;
	retRect.maxX = maxX + expandW;
	retRect.maxY = maxY + expandH;
	retRect.centerX = (minX + maxX) / 2;
	retRect.centerY = (minY + maxY) / 2;
	return retRect;
}

bool BBRect::intersectsRect(const BBRect& rect) const
{
	return !(maxX < rect.minX || minX > rect.maxX || maxY < rect.minY || minY > rect.maxY);
}

bool BBRect::intersctsRect(int minX, int maxX, int minY, int maxY) const
{
	return !(this->maxX < minX || this->minX > maxX || this->maxY < minY || this->minY > maxY);
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