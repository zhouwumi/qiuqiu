#ifndef BB_CLIENT_GAME_MANAGER_H
#define BB_CLIENT_GAME_MANAGER_H
#include<math.h>

class BBTestUtils
{
public:
	BBTestUtils();
	~BBTestUtils();

	int Mass2Radius(int mass)
	{
		return 4 + ceilf(sqrt(mass) * 6);
	}

	int Mass2Speed(int mass)
	{
		if (mass < 10)
		{
			mass = 10;
		}
		int rate = floorf(mass / 5.0) * 5 + 10;
		return int(floorf(8 * 2 * sqrtf(20.0 / rate)));
	}
	int PressureToPercent(int pressure)
	{
		return pressure / 10.0;
	}

	double bb_fix_float(double value)
	{
		return floor(value * 1000) / 1000.0;
	}

	int bb_float_to_int(double value)
	{
		return floorf(value * 1000);
	}

	double bb_int_to_float(int value)
	{
		return value / 1000.0;
	}
};

#endif