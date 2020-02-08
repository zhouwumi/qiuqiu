#include "BBMathUtils.h"
#include <random>
#include "BBConst.h"

#if defined(_WIN32) && defined(_WINDOWS)
	#include "cocos2d.h"
	void BBSendLogToWindow(const char *log)
	{
		static const int CCLOG_STRING_TAG = 1;
		// Send data as a message
		COPYDATASTRUCT myCDS;
		myCDS.dwData = CCLOG_STRING_TAG;
		myCDS.cbData = (DWORD)strlen(log) + 1;
		myCDS.lpData = (PVOID)log;
		if (cocos2d::Director::getInstance()->getOpenGLView())
		{
			HWND hwnd = cocos2d::Director::getInstance()->getOpenGLView()->getWin32Window();
			SendMessage(hwnd,
				WM_COPYDATA,
				(WPARAM)(HWND)hwnd,
				(LPARAM)(LPVOID)&myCDS);
		}
	}
#else
#include<stdarg.h>
#include<sys/time.h>
void BBSendLogToWindow(const char *log)
{
}
#endif // COCOS2D_DEBUG

#define MAX_LOG_LENGTH 16 * 1024

float BBMathUtils::SIN_VALUE[] = {
	0, 0.0175, 0.0349, 0.0523, 0.0698, 0.0872, 0.1045, 0.1219, 0.1392, 0.1564, 
	0.1736, 0.1908, 0.2079, 0.2250, 0.2419, 0.2588, 0.2756, 0.2924, 0.3090, 0.3256, 
	0.3420, 0.3584, 0.3746, 0.3907, 0.4067, 0.4226, 0.4384, 0.4540, 0.4695, 0.4848, 0.5, 0.5150, 0.5299, 0.5446, 0.5592, 0.5736, 0.5878, 0.6018, 0.6157, 0.6293, 0.6428, 0.6561, 0.6691, 0.6820, 0.6947, 0.7071, 0.7193, 0.7314, 0.7431, 0.7547, 0.7660, 0.7771, 0.7880, 0.7986, 0.8090, 0.8192, 0.8290, 0.8387, 0.8480, 0.8572, 0.8660, 0.8746, 0.8829, 0.8910, 0.8988, 0.9063, 0.9135, 0.9205, 0.9272, 0.9336, 0.9397, 0.9455, 0.9511, 0.9563, 0.9613, 0.9659, 0.9703, 0.9744, 0.9781, 0.9816, 0.9848, 0.9877, 0.9903, 0.9925, 0.9945, 0.9962, 0.9976, 0.9986, 0.9994, 0.9998, 1, 0.9998, 0.9994, 0.9986, 0.9976, 0.9962, 0.9945, 0.9925, 0.9903, 0.9877, 0.9848, 0.9816, 0.9781, 0.9744, 0.9703, 0.9659, 0.9613, 0.9563, 0.9511, 0.9455, 0.9397, 0.9336, 0.9272, 0.9205, 0.9135, 0.9063, 0.8988, 0.8910, 0.8829, 0.8746, 0.8660, 0.8572, 0.8480, 0.8387, 0.8290, 0.8192, 0.8090, 0.7986, 0.7880, 0.7771, 0.7660, 0.7547, 0.7431, 0.7314, 0.7193, 0.7071, 0.6947, 0.6820, 0.6691, 0.6561, 0.6428, 0.6293, 0.6157, 0.6018, 0.5878, 0.5736, 0.5592, 0.5446, 0.5299, 0.5150, 0.5, 0.4848, 0.4695, 0.4540, 0.4384, 0.4226, 0.4067, 0.3907, 0.3746, 0.3584, 0.3420, 0.3256, 0.3090, 0.2924, 0.2756, 0.2588, 0.2419, 0.2250, 0.2079, 0.1908, 0.1736, 0.1564, 0.1392, 0.1219, 0.1045, 0.0872, 0.0698, 0.0523, 0.0349, 0.0175, 0, -0.0175, -0.0349, -0.0523, -0.0698, -0.0872, -0.1045, -0.1219, -0.1392, -0.1564, -0.1736, -0.1908, -0.2079, -0.2250, -0.2419, -0.2588, -0.2756, -0.2924, -0.3090, -0.3256, -0.3420, -0.3584, -0.3746, -0.3907, -0.4067, -0.4226, -0.4384, -0.4540, -0.4695, -0.4848, -0.5, -0.5150, -0.5299, -0.5446, -0.5592, -0.5736, -0.5878, -0.6018, -0.6157, -0.6293, -0.6428, -0.6561, -0.6691, -0.6820, -0.6947, -0.7071, -0.7193, -0.7314, -0.7431, -0.7547, -0.7660, -0.7771, -0.7880, -0.7986, -0.8090, -0.8192, -0.8290, -0.8387, -0.8480, -0.8572, -0.8660, -0.8746, -0.8829, -0.8910, -0.8988, -0.9063, -0.9135, -0.9205, -0.9272, -0.9336, -0.9397, -0.9455, -0.9511, -0.9563, -0.9613, -0.9659, -0.9703, -0.9744, -0.9781, -0.9816, -0.9848, -0.9877, -0.9903, -0.9925, -0.9945, -0.9962, -0.9976, -0.9986, -0.9994, -0.9998, -1, -0.9998, -0.9994, -0.9986, -0.9976, -0.9962, -0.9945, -0.9925, -0.9903, -0.9877, -0.9848, -0.9816, -0.9781, -0.9744, -0.9703, -0.9659, -0.9613, -0.9563, -0.9511, -0.9455, -0.9397, -0.9336, -0.9272, -0.9205, -0.9135, -0.9063, -0.8988, -0.8910, -0.8829, -0.8746, -0.8660, -0.8572, -0.8480, -0.8387, -0.8290, -0.8192, -0.8090, -0.7986, -0.7880, -0.7771, -0.7660, -0.7547, -0.7431, -0.7314, -0.7193, -0.7071, -0.6947, -0.6820, -0.6691, -0.6561, -0.6428, -0.6293, -0.6157, -0.6018, -0.5878, -0.5736, -0.5592, -0.5446, -0.5299, -0.5150, -0.5, -0.4848, -0.4695, -0.4540, -0.4384, -0.4226, -0.4067, -0.3907, -0.3746, -0.3584, -0.3420, -0.3256, -0.3090, -0.2924, -0.2756, -0.2588, -0.2419, -0.2250, -0.2079, -0.1908, -0.1736, -0.1564, -0.1392, -0.1219, -0.1045, -0.0872, -0.0698, -0.0523, -0.0349, -0.0175
};

float BBMathUtils::COS_VALUE[] = {
	1,0.9998,0.9994,0.9986,0.9976,0.9962,0.9945,0.9925,0.9903,0.9877,0.9848,0.9816,0.9781,0.9744,0.9703,0.9659,0.9613,0.9563,0.9511,0.9455,0.9397,0.9336,0.9272,0.9205,0.9135,0.9063,0.8988,0.8910,0.8829,0.8746,0.8660,0.8572,0.8480,0.8387,0.8290,0.8192,0.8090,0.7986,0.7880,0.7771,0.7660,0.7547,0.7431,0.7314,0.7193,0.7071,0.6947,0.6820,0.6691,0.6561,0.6428,0.6293,0.6157,0.6018,0.5878,0.5736,0.5592,0.5446,0.5299,0.5150,0.5,0.4848,0.4695,0.4540,0.4384,0.4226,0.4067,0.3907,0.3746,0.3584,0.3420,0.3256,0.3090,0.2924,0.2756,0.2588,0.2419,0.2250,0.2079,0.1908,0.1736,0.1564,0.1392,0.1219,0.1045,0.0872,0.0698,0.0523,0.0349,0.0175,0,-0.0175,-0.0349,-0.0523,-0.0698,-0.0872,-0.1045,-0.1219,-0.1392,-0.1564,-0.1736,-0.1908,-0.2079,-0.2250,-0.2419,-0.2588,-0.2756,-0.2924,-0.3090,-0.3256,-0.3420,-0.3584,-0.3746,-0.3907,-0.4067,-0.4226,-0.4384,-0.4540,-0.4695,-0.4848,-0.5,-0.5150,-0.5299,-0.5446,-0.5592,-0.5736,-0.5878,-0.6018,-0.6157,-0.6293,-0.6428,-0.6561,-0.6691,-0.6820,-0.6947,-0.7071,-0.7193,-0.7314,-0.7431,-0.7547,-0.7660,-0.7771,-0.7880,-0.7986,-0.8090,-0.8192,-0.8290,-0.8387,-0.8480,-0.8572,-0.8660,-0.8746,-0.8829,-0.8910,-0.8988,-0.9063,-0.9135,-0.9205,-0.9272,-0.9336,-0.9397,-0.9455,-0.9511,-0.9563,-0.9613,-0.9659,-0.9703,-0.9744,-0.9781,-0.9816,-0.9848,-0.9877,-0.9903,-0.9925,-0.9945,-0.9962,-0.9976,-0.9986,-0.9994,-0.9998,-1,-0.9998,-0.9994,-0.9986,-0.9976,-0.9962,-0.9945,-0.9925,-0.9903,-0.9877,-0.9848,-0.9816,-0.9781,-0.9744,-0.9703,-0.9659,-0.9613,-0.9563,-0.9511,-0.9455,-0.9397,-0.9336,-0.9272,-0.9205,-0.9135,-0.9063,-0.8988,-0.8910,-0.8829,-0.8746,-0.8660,-0.8572,-0.8480,-0.8387,-0.8290,-0.8192,-0.8090,-0.7986,-0.7880,-0.7771,-0.7660,-0.7547,-0.7431,-0.7314,-0.7193,-0.7071,-0.6947,-0.6820,-0.6691,-0.6561,-0.6428,-0.6293,-0.6157,-0.6018,-0.5878,-0.5736,-0.5592,-0.5446,-0.5299,-0.5150,-0.5,-0.4848,-0.4695,-0.4540,-0.4384,-0.4226,-0.4067,-0.3907,-0.3746,-0.3584,-0.3420,-0.3256,-0.3090,-0.2924,-0.2756,-0.2588,-0.2419,-0.2250,-0.2079,-0.1908,-0.1736,-0.1564,-0.1392,-0.1219,-0.1045,-0.0872,-0.0698,-0.0523,-0.0349,-0.0175,0,0.0175,0.0349,0.0523,0.0698,0.0872,0.1045,0.1219,0.1392,0.1564,0.1736,0.1908,0.2079,0.2250,0.2419,0.2588,0.2756,0.2924,0.3090,0.3256,0.3420,0.3584,0.3746,0.3907,0.4067,0.4226,0.4384,0.4540,0.4695,0.4848,0.5,0.5150,0.5299,0.5446,0.5592,0.5736,0.5878,0.6018,0.6157,0.6293,0.6428,0.6561,0.6691,0.6820,0.6947,0.7071,0.7193,0.7314,0.7431,0.7547,0.7660,0.7771,0.7880,0.7986,0.8090,0.8192,0.8290,0.8387,0.8480,0.8572,0.8660,0.8746,0.8829,0.8910,0.8988,0.9063,0.9135,0.9205,0.9272,0.9336,0.9397,0.9455,0.9511,0.9563,0.9613,0.9659,0.9703,0.9744,0.9781,0.9816,0.9848,0.9877,0.9903,0.9925,0.9945,0.9962,0.9976,0.9986,0.9994,0.9998
};

std::vector<std::string> BBMathUtils::serverLog;

unsigned int BBMathUtils::crc_table[256];

void BBMathUtils::init_crc_table()
{
	unsigned int c;
	unsigned int i, j;

	for (i = 0; i < 256; i++) {
		c = (unsigned int)i;
		for (j = 0; j < 8; j++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[i] = c;
	}
}

unsigned int BBMathUtils::crc32(char *buffer, unsigned int size)
{
	unsigned int crc = 0xffffffff;
	unsigned int i;
	for (i = 0; i < size; i++) {
		crc = crc_table[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);
	}
	return crc;
}

int BBMathUtils::xy_to_location(int x, int y)
{
	return x << 16 | y;
}

void BBMathUtils::location_to_xy(int location, int& x, int& y)
{
	x = location >> 16;
	y = location & 0xFFFF;
}

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
	int deltaX = circle.location.x - point.x;
	int deltaY = circle.location.y - point.y;
	int totalRadius = radius + circle.GetRadius();
	return deltaX * deltaX + deltaY * deltaY < totalRadius * totalRadius;
}

bool BBMathUtils::CheckPlayerCollsion(BaseCircleNode& circle1, BaseCircleNode& circle2)
{
	int x = circle1.location.x - circle2.location.x;
	int y = circle1.location.y - circle2.location.y;
	int radius = circle1.GetRadius() + circle2.GetRadius();
	return x * x + y * y < radius * radius;
}

bool BBMathUtils::NeedRollback(BaseCircleNode* circle1, BaseCircleNode* circle2, float delta)
{
	return circle1->GetRadius() <= circle2->GetRadius() * (1 + delta) && circle2->GetRadius() <= circle1->GetRadius() * (1 + delta);
}

bool BBMathUtils::CanEat(BaseCircleNode& circle1, BaseCircleNode& circle2, float delta)
{
	if (circle1.GetRadius() < circle2.GetRadius() * (1 + delta))
	{
		return false;
	}
	int x = circle1.location.x - circle2.location.x;
	int y = circle1.location.y - circle2.location.y;
	return x * x + y * y < circle1.GetRadius() * circle1.GetRadius();
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
	return int(floorf(8 * 2 * sqrtf(20.0f / (mass + 10))));
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

bool BBMathUtils::FixCircle(const BBRect& rect, float circleX, float circleY, int radius, float& fixedX, float& fixedY)
{
	bool ret = false;
	if (circleX + radius > rect.maxX)
	{
		ret = true;
		fixedX = rect.maxX - radius;
	}
	else if (circleX - radius < rect.minX) {
		ret = true;
		fixedX = rect.minX + radius;
	}
	else {
		fixedX = circleX;
	}

	if (circleY + radius > rect.maxY)
	{
		ret = true;
		fixedY = rect.maxY - radius;
	}
	else if (circleY - radius < rect.minY) {
		ret = true;
		fixedY = rect.minY + radius;
	}
	else {
		fixedY = circleY;
	}
	return ret;
}

bool BBMathUtils::CheckCircleHit(BaseCircleNode& node1, BaseCircleNode& node2)
{
	int deltaX = node1.location.x - node2.location.x;
	int deltaY = node1.location.y - node2.location.y;
	int radius = node1.GetRadius() + node2.GetRadius();
	return pow(deltaX, 2) + pow(deltaY, 2) < pow(radius, 2);
}

bool BBMathUtils::CheckCircleHit(BaseCircleNode& node1, int circleX, int circleY, int radius)
{
	int deltaX = node1.location.x - circleX;
	int deltaY = node1.location.y - circleY;
	int totalRadius = node1.GetRadius() + radius;
	return pow(deltaX, 2) + pow(deltaY, 2) < pow(totalRadius, 2);
}

int BBMathUtils::abs_int(int x) {
    return static_cast<int>(abs(static_cast<double>(x)));
}

float BBMathUtils::bb_fix_float(float value)
{
	return floorf(value * 1000) / 1000.0f;
}

void BBMathUtils::bb_fix_bb_vector(BBVector& vec)
{
	vec.x = BBMathUtils::bb_fix_float(vec.x);
	vec.y = BBMathUtils::bb_fix_float(vec.y);
}

int BBMathUtils::bb_float_to_int(float value)
{
	return floorf(value * 1000);
}


float BBMathUtils::bb_int_to_float(int value)
{
	return value / 1000.0f;
}

void BBMathUtils::BBLOG(const char *format, ...)
{
#if 1
	int bufferSize = MAX_LOG_LENGTH;
	char* buf = nullptr;
	int nret = 0;
	va_list args;
	do
	{
		buf = new (std::nothrow) char[bufferSize];
		if (buf == nullptr)
			return;
		va_start(args, format);
		nret = vsnprintf(buf, bufferSize - 3, format, args);
		va_end(args);

		if (nret >= 0)
		{ // VS2015/2017
			if (nret <= bufferSize - 3)
			{// success, so don't need to realloc
				break;
			}
			else
			{
				bufferSize = nret + 3;
				delete[] buf;
			}
		}
		else // < 0
		{	// VS2013 or Unix-like System(GCC)
			bufferSize *= 2;
			delete[] buf;
		}
	} while (true);
	buf[nret] = '\n';
	buf[++nret] = '\0';

#if defined(_WIN32) && defined(_WINDOWS)
	int pos = 0;
	int len = nret;
	char tempBuf[MAX_LOG_LENGTH + 1] = { 0 };
	WCHAR wszBuf[MAX_LOG_LENGTH + 1] = { 0 };
	do
	{
		int dataSize = std::min(MAX_LOG_LENGTH, len - pos);
		std::copy(buf + pos, buf + pos + dataSize, tempBuf);

		tempBuf[dataSize] = 0;

		MultiByteToWideChar(CP_UTF8, 0, tempBuf, -1, wszBuf, sizeof(wszBuf));
		OutputDebugStringW(wszBuf);
		WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, tempBuf, sizeof(tempBuf), nullptr, FALSE);
		printf("%s", tempBuf);

		pos += dataSize;

	} while (pos < len);
	BBSendLogToWindow(buf);
	fflush(stdout);
#else
	serverLog.emplace_back(buf);
	// Linux, Mac, iOS, etc
	/*fprintf(stdout, "%s", buf);
	fflush(stdout);*/
#endif

	delete[] buf;
#endif // 0
}

void BBMathUtils::ClearLog()
{
	serverLog.clear();
}

//timeval BBMathUtils::getCurrentTime()
//{
//	timeval tm = { 0 };
//#if defined(_WIN32) && defined(_WINDOWS)
//	cocos2d::gettimeofday(&tm, NULL);
//#else
//	gettimeofday(&tm, NULL);
//#endif
//	return tm;
//}
//
//int BBMathUtils::getTimeCost(timeval time1, timeval time2)
//{
//	return (time1.tv_sec - time2.tv_sec) * 1000000 + (time1.tv_usec - time2.tv_usec);
//}