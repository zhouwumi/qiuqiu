#include "BBMathUtils.h"
#include <random>
#include "BBConst.h"

float BBMathUtils::SIN_VALUE[] = {
	0, 0.0175, 0.0349, 0.0523, 0.0698, 0.0872, 0.1045, 0.1219, 0.1392, 0.1564, 
	0.1736, 0.1908, 0.2079, 0.2250, 0.2419, 0.2588, 0.2756, 0.2924, 0.3090, 0.3256, 
	0.3420, 0.3584, 0.3746, 0.3907, 0.4067, 0.4226, 0.4384, 0.4540, 0.4695, 0.4848, 0.5, 0.5150, 0.5299, 0.5446, 0.5592, 0.5736, 0.5878, 0.6018, 0.6157, 0.6293, 0.6428, 0.6561, 0.6691, 0.6820, 0.6947, 0.7071, 0.7193, 0.7314, 0.7431, 0.7547, 0.7660, 0.7771, 0.7880, 0.7986, 0.8090, 0.8192, 0.8290, 0.8387, 0.8480, 0.8572, 0.8660, 0.8746, 0.8829, 0.8910, 0.8988, 0.9063, 0.9135, 0.9205, 0.9272, 0.9336, 0.9397, 0.9455, 0.9511, 0.9563, 0.9613, 0.9659, 0.9703, 0.9744, 0.9781, 0.9816, 0.9848, 0.9877, 0.9903, 0.9925, 0.9945, 0.9962, 0.9976, 0.9986, 0.9994, 0.9998, 1, 0.9998, 0.9994, 0.9986, 0.9976, 0.9962, 0.9945, 0.9925, 0.9903, 0.9877, 0.9848, 0.9816, 0.9781, 0.9744, 0.9703, 0.9659, 0.9613, 0.9563, 0.9511, 0.9455, 0.9397, 0.9336, 0.9272, 0.9205, 0.9135, 0.9063, 0.8988, 0.8910, 0.8829, 0.8746, 0.8660, 0.8572, 0.8480, 0.8387, 0.8290, 0.8192, 0.8090, 0.7986, 0.7880, 0.7771, 0.7660, 0.7547, 0.7431, 0.7314, 0.7193, 0.7071, 0.6947, 0.6820, 0.6691, 0.6561, 0.6428, 0.6293, 0.6157, 0.6018, 0.5878, 0.5736, 0.5592, 0.5446, 0.5299, 0.5150, 0.5, 0.4848, 0.4695, 0.4540, 0.4384, 0.4226, 0.4067, 0.3907, 0.3746, 0.3584, 0.3420, 0.3256, 0.3090, 0.2924, 0.2756, 0.2588, 0.2419, 0.2250, 0.2079, 0.1908, 0.1736, 0.1564, 0.1392, 0.1219, 0.1045, 0.0872, 0.0698, 0.0523, 0.0349, 0.0175, 0, -0.0175, -0.0349, -0.0523, -0.0698, -0.0872, -0.1045, -0.1219, -0.1392, -0.1564, -0.1736, -0.1908, -0.2079, -0.2250, -0.2419, -0.2588, -0.2756, -0.2924, -0.3090, -0.3256, -0.3420, -0.3584, -0.3746, -0.3907, -0.4067, -0.4226, -0.4384, -0.4540, -0.4695, -0.4848, -0.5, -0.5150, -0.5299, -0.5446, -0.5592, -0.5736, -0.5878, -0.6018, -0.6157, -0.6293, -0.6428, -0.6561, -0.6691, -0.6820, -0.6947, -0.7071, -0.7193, -0.7314, -0.7431, -0.7547, -0.7660, -0.7771, -0.7880, -0.7986, -0.8090, -0.8192, -0.8290, -0.8387, -0.8480, -0.8572, -0.8660, -0.8746, -0.8829, -0.8910, -0.8988, -0.9063, -0.9135, -0.9205, -0.9272, -0.9336, -0.9397, -0.9455, -0.9511, -0.9563, -0.9613, -0.9659, -0.9703, -0.9744, -0.9781, -0.9816, -0.9848, -0.9877, -0.9903, -0.9925, -0.9945, -0.9962, -0.9976, -0.9986, -0.9994, -0.9998, -1, -0.9998, -0.9994, -0.9986, -0.9976, -0.9962, -0.9945, -0.9925, -0.9903, -0.9877, -0.9848, -0.9816, -0.9781, -0.9744, -0.9703, -0.9659, -0.9613, -0.9563, -0.9511, -0.9455, -0.9397, -0.9336, -0.9272, -0.9205, -0.9135, -0.9063, -0.8988, -0.8910, -0.8829, -0.8746, -0.8660, -0.8572, -0.8480, -0.8387, -0.8290, -0.8192, -0.8090, -0.7986, -0.7880, -0.7771, -0.7660, -0.7547, -0.7431, -0.7314, -0.7193, -0.7071, -0.6947, -0.6820, -0.6691, -0.6561, -0.6428, -0.6293, -0.6157, -0.6018, -0.5878, -0.5736, -0.5592, -0.5446, -0.5299, -0.5150, -0.5, -0.4848, -0.4695, -0.4540, -0.4384, -0.4226, -0.4067, -0.3907, -0.3746, -0.3584, -0.3420, -0.3256, -0.3090, -0.2924, -0.2756, -0.2588, -0.2419, -0.2250, -0.2079, -0.1908, -0.1736, -0.1564, -0.1392, -0.1219, -0.1045, -0.0872, -0.0698, -0.0523, -0.0349, -0.0175
};

float BBMathUtils::COS_VALUE[] = {
	1,0.9998,0.9994,0.9986,0.9976,0.9962,0.9945,0.9925,0.9903,0.9877,0.9848,0.9816,0.9781,0.9744,0.9703,0.9659,0.9613,0.9563,0.9511,0.9455,0.9397,0.9336,0.9272,0.9205,0.9135,0.9063,0.8988,0.8910,0.8829,0.8746,0.8660,0.8572,0.8480,0.8387,0.8290,0.8192,0.8090,0.7986,0.7880,0.7771,0.7660,0.7547,0.7431,0.7314,0.7193,0.7071,0.6947,0.6820,0.6691,0.6561,0.6428,0.6293,0.6157,0.6018,0.5878,0.5736,0.5592,0.5446,0.5299,0.5150,0.5,0.4848,0.4695,0.4540,0.4384,0.4226,0.4067,0.3907,0.3746,0.3584,0.3420,0.3256,0.3090,0.2924,0.2756,0.2588,0.2419,0.2250,0.2079,0.1908,0.1736,0.1564,0.1392,0.1219,0.1045,0.0872,0.0698,0.0523,0.0349,0.0175,0,-0.0175,-0.0349,-0.0523,-0.0698,-0.0872,-0.1045,-0.1219,-0.1392,-0.1564,-0.1736,-0.1908,-0.2079,-0.2250,-0.2419,-0.2588,-0.2756,-0.2924,-0.3090,-0.3256,-0.3420,-0.3584,-0.3746,-0.3907,-0.4067,-0.4226,-0.4384,-0.4540,-0.4695,-0.4848,-0.5,-0.5150,-0.5299,-0.5446,-0.5592,-0.5736,-0.5878,-0.6018,-0.6157,-0.6293,-0.6428,-0.6561,-0.6691,-0.6820,-0.6947,-0.7071,-0.7193,-0.7314,-0.7431,-0.7547,-0.7660,-0.7771,-0.7880,-0.7986,-0.8090,-0.8192,-0.8290,-0.8387,-0.8480,-0.8572,-0.8660,-0.8746,-0.8829,-0.8910,-0.8988,-0.9063,-0.9135,-0.9205,-0.9272,-0.9336,-0.9397,-0.9455,-0.9511,-0.9563,-0.9613,-0.9659,-0.9703,-0.9744,-0.9781,-0.9816,-0.9848,-0.9877,-0.9903,-0.9925,-0.9945,-0.9962,-0.9976,-0.9986,-0.9994,-0.9998,-1,-0.9998,-0.9994,-0.9986,-0.9976,-0.9962,-0.9945,-0.9925,-0.9903,-0.9877,-0.9848,-0.9816,-0.9781,-0.9744,-0.9703,-0.9659,-0.9613,-0.9563,-0.9511,-0.9455,-0.9397,-0.9336,-0.9272,-0.9205,-0.9135,-0.9063,-0.8988,-0.8910,-0.8829,-0.8746,-0.8660,-0.8572,-0.8480,-0.8387,-0.8290,-0.8192,-0.8090,-0.7986,-0.7880,-0.7771,-0.7660,-0.7547,-0.7431,-0.7314,-0.7193,-0.7071,-0.6947,-0.6820,-0.6691,-0.6561,-0.6428,-0.6293,-0.6157,-0.6018,-0.5878,-0.5736,-0.5592,-0.5446,-0.5299,-0.5150,-0.5,-0.4848,-0.4695,-0.4540,-0.4384,-0.4226,-0.4067,-0.3907,-0.3746,-0.3584,-0.3420,-0.3256,-0.3090,-0.2924,-0.2756,-0.2588,-0.2419,-0.2250,-0.2079,-0.1908,-0.1736,-0.1564,-0.1392,-0.1219,-0.1045,-0.0872,-0.0698,-0.0523,-0.0349,-0.0175,0,0.0175,0.0349,0.0523,0.0698,0.0872,0.1045,0.1219,0.1392,0.1564,0.1736,0.1908,0.2079,0.2250,0.2419,0.2588,0.2756,0.2924,0.3090,0.3256,0.3420,0.3584,0.3746,0.3907,0.4067,0.4226,0.4384,0.4540,0.4695,0.4848,0.5,0.5150,0.5299,0.5446,0.5592,0.5736,0.5878,0.6018,0.6157,0.6293,0.6428,0.6561,0.6691,0.6820,0.6947,0.7071,0.7193,0.7314,0.7431,0.7547,0.7660,0.7771,0.7880,0.7986,0.8090,0.8192,0.8290,0.8387,0.8480,0.8572,0.8660,0.8746,0.8829,0.8910,0.8988,0.9063,0.9135,0.9205,0.9272,0.9336,0.9397,0.9455,0.9511,0.9563,0.9613,0.9659,0.9703,0.9744,0.9781,0.9816,0.9848,0.9877,0.9903,0.9925,0.9945,0.9962,0.9976,0.9986,0.9994,0.9998
};

BBVector BBMathUtils::AngleToFixedVector(int angle, int length)
{
	int realAngle = angle % 360;
	int x = int(roundf(COS_VALUE[realAngle] * length));
	int y = int(roundf(SIN_VALUE[realAngle] * length));
	return BBVector(x, y);
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
	int x = rect.minX + rand() % (rect.maxX - rect.minX + 1);
	int y = rect.minY + rand() % (rect.maxY - rect.minY + 1);
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

int BBMathUtils::Mass2Speed(int mass)
{
	return int(floorf(8 * sqrtf(20.0f / (mass + 10))));
}

int BBMathUtils::PressureToPercent(int pressure)
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
	return min + rand() % (max - min + 1);
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

int BBMathUtils::abs_int(int x) {
    return static_cast<int>(abs(static_cast<double>(x)));
}