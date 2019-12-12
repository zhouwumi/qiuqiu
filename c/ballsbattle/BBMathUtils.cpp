#include "BBMathUtils.h"
#include <random>
#include "BBConst.h"

BBVector BBMathUtils::AngleToUnitVector(float angle)
{
	return BBVector(cosf(BB_M_PI * angle / 180), sinf(BB_M_PI * angle / 180));
}

BBVector BBMathUtils::AngleToFixedVector(float angle, int length)
{
	return BBVector(cosf(BB_M_PI * angle / 180) * length, sinf(BB_M_PI * angle / 180) * length);
}

void BBMathUtils::FixPoint(const BBRect& rect, BBPoint& point)
{
	if (point.x < rect.minX) {
		point.x = rect.minX;
	}
	if (point.x > rect.maxX)
	{
		point.x = rect.maxX;
	}
	if (point.y < rect.minY)
	{
		point.y = rect.minY;
	}
	if (point.y > rect.maxY)
	{
		point.y = rect.maxY;
	}
}

BBPoint BBMathUtils::GetRandomPoint(const BBRect& rect)
{
	int x = rect.minX + (int)((rect.maxX - rect.minX) * rand() / (RAND_MAX + 1.0));
	int y = rect.minY + (int)((rect.maxY - rect.minY) * rand() / (RAND_MAX + 1.0));
	return BBPoint(x, y);
}

bool BBMathUtils::CheckFoodGrip(BaseCircleNode& circle, BBPoint& point, int radius)
{
	int deltaX = circle.positionX - point.x;
	int deltaY = circle.positionY - point.y;
	int totalRadius = radius + circle.radius;
	return deltaX * deltaX + deltaY * deltaY < totalRadius * totalRadius;
}

bool BBMathUtils::CheckPlayerCollsion(BaseCircleNode& circle1, BaseCircleNode& circle2)
{
	int x = circle1.positionX - circle2.positionX;
	int y = circle1.positionY - circle2.positionY;
	int radius = circle1.radius + circle2.radius;
	return x * x + y * y < radius * radius;
}

bool BBMathUtils::NeedRollback(BaseCircleNode& circle1, BaseCircleNode& circle2, float delta)
{
	return circle1.radius <= circle2.radius * (1 + delta) && circle2.radius <= circle1.radius * (1 + delta);
}

bool BBMathUtils::CanEat(BaseCircleNode& circle1, BaseCircleNode& circle2, float delta)
{
	if (circle1.radius < circle2.radius * (1 + delta))
	{
		return false;
	}
	int x = circle1.positionX - circle2.positionX;
	int y = circle1.positionY - circle2.positionY;
	return x * x + y * y < circle1.radius * circle1.radius;
}

float BBMathUtils::PointToLineDistance(BBPoint& point, BBPoint& line1, BBPoint& line2)
{
	int a = line2.y - line1.y;
	int b = line1.x - line2.x;
	int c = line2.x * line1.y - line1.x * line2.y;
	return fabs(a * point.x + b * point.y + c) / sqrtf(a * a + b * b);
}

int BBMathUtils::Mass2Radius(int mass)
{
	return 4 + ceilf(sqrt(mass) * 6);
}

float BBMathUtils::Mass2Speed(int mass)
{
	return floorf(8 * 10 * sqrtf(20.0f / (mass + 10))) / 10;
}

float BBMathUtils::PressureToPercent(int pressure)
{
	return pressure / 10.0f;
}

int BBMathUtils::KillToExp(int kill, int fixed, int fixBase)
{
	return ceilf(10.0f * kill * fixed / fixBase);
}

float BBMathUtils::RoundDown(float num, int perc)
{
	int scale = pow(10, perc);
	return floorf(num * scale) / scale;
}

BBRect BBMathUtils::GetVision(int baseW, int baseH, BBRect& bound)
{
	int width = bound.maxX - bound.minX;
	int height = bound.maxY - bound.minY;
	float length = sqrtf(width * width + height * height) / 2;
	float buf = 1 / (0.003132 * length + 0.90604);
	int widthX = ceilf(baseW / buf);
	int heightY = ceilf(baseH / buf);
	return BBRect(bound.centerX - widthX, bound.centerY - heightY, bound.centerX + widthX, bound.centerY + heightY);
	
}

int BBMathUtils::GetRandom(int min, int max)
{
	return min + (int)((max - min) * rand() / (RAND_MAX + 1.0));
}

void BBMathUtils::FixCircle(const BBRect& rect, int circleX, int circleY, int radius, int& fixedX, int& fixedY, bool& isFixedX, bool& isFixedY)
{
	
	if (circleX + radius > rect.maxX)
	{
		isFixedX = true;
		fixedX = rect.maxX - radius;
	}
	else if (circleX - radius < rect.minX) {
		isFixedX = true;
		fixedX = rect.minX + radius;
	}
	else {
		isFixedX = false;
		fixedX = circleX;
	}

	if (circleY + radius > rect.maxY)
	{
		isFixedY = true;
		fixedY = rect.maxY - radius;
	}
	else if (circleY - radius < rect.minY) {
		isFixedY = true;
		fixedY = rect.minY + radius;
	}
	else {
		isFixedY = false;
		fixedY = circleY;
	}
}

bool BBMathUtils::CheckCircleHit(BaseCircleNode& node1, BaseCircleNode& node2)
{
	int deltaX = node1.positionX - node2.positionX;
	int deltaY = node1.positionY - node2.positionY;
	int radius = node1.radius + node2.radius;
	return pow(deltaX, 2) + pow(deltaY, 2) < pow(radius, 2);
}

bool BBMathUtils::CheckCircleHit(BaseCircleNode& node1, int circleX, int circleY, int radius)
{
	int deltaX = node1.positionX - circleX;
	int deltaY = node1.positionY - circleY;
	int totalRadius = node1.radius + radius;
	return pow(deltaX, 2) + pow(deltaY, 2) < pow(totalRadius, 2);
}