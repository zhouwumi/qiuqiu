#ifndef BB_PREDICTION_DATA_H
#define BB_PREDICTION_DATA_H

#include<deque>

class BBSimplePrediction
{
public:
	int angle;
	int pressure;
	unsigned int crc;
};

class BBPredictionData
{
public:
	BBPredictionData();
	~BBPredictionData();
	void ClearData();

public:
	std::deque<BBSimplePrediction> predictionCommands;
	int lastAngle;
	int lastPressure;
	bool mPredictionRight;
	bool canSkipSync;
};

#endif
