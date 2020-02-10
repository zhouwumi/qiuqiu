#ifndef BB_MATH_UTILS_CPP
#define BB_MATH_UTILS_CPP
#include "BBPoint.h"
#include "BBRect.h"
#include "BBVector.h"
#include "BaseCircleNode.h"
#include <random>
#include<time.h>

class BBMathUtils
{
public:
	static BBVector AngleToFixedVector(int angle, int length);

	static void FixPoint(const BBRect& rect, BBPoint& point);
	static BBPoint GetRandomPoint(const BBRect& rect);

	static bool CheckFoodGrip(BaseCircleNode& circle, BBPoint& point, int radius);
	static bool CheckPlayerCollsion(BaseCircleNode& circle1, BaseCircleNode& circle2);
	static bool NeedRollback(BaseCircleNode* circle1, BaseCircleNode* circle2, double delta);
	static bool CanEat(BaseCircleNode& circle1, BaseCircleNode& circle2, double delta);

	static double PointToLineDistance(BBPoint& point, BBPoint& line1, BBPoint& line2);

	static int Mass2Radius(int mass);
	static int Mass2Speed(int mass);
	static int PressureToPercent(int pressure);

	static int KillToExp(int kill, int fixed, int fixBase);
	static double RoundDown(double num, int perc);

	static BBRect GetVision(int baseW, int baseH, BBRect& bound);

	static int GetRandom(int min, int max);
	static bool FixCircle(const BBRect& rect, double circleX, double circleY, int radius, double& fixedX, double& fixedY);
	static bool CheckCircleHit(BaseCircleNode& node1, BaseCircleNode& node2);
	static bool CheckCircleHit(BaseCircleNode& node1, int circleX, int circleY, int radius);
	static int abs_int(int x);

	static void BBLOG(const char *format, ...);
	static void ClearLog();
	static int xy_to_location(int x, int y);
	static void location_to_xy(int location, int& x, int& y);
	static double bb_fix_float(double value);
	static int bb_float_to_int(double value);
	static double bb_int_to_float(int value);
	static void bb_fix_bb_vector(BBVector& vec);

	static void init_crc_table();
	static unsigned int crc32(char *buffer, unsigned int size);
	

	static double SIN_VALUE[];
	static double COS_VALUE[];
	static unsigned int crc_table[];
	static std::vector<std::string> serverLog;
};

#endif
