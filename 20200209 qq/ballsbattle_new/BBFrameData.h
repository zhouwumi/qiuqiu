#ifndef BB_FRAME_DATA_H
#define BB_FRAME_DATA_H

#include<vector>
#include<deque>
#include "BaseFrameSyncData.h"
using namespace std;

class BBFrameBallData
{
public:
	BBFrameBallData();
	~BBFrameBallData();

public:
	int idx; //absolute unique
	int mass; //mass
	int fromId; //generate from id ball
	int uid; // belong player
	int cd; //cd fusion cd time
	int protect; //birth can't eat and beEat time
	int initStopFrame; // spring move time
	int initSpeed;
	int initDeltaSpeed;
	int locationX;
	int locationY;
};

class BBBallDeltaData
{
public:
	BBBallDeltaData();
	~BBBallDeltaData();
public:
	int wrapTicks;
	BBVector location;
	BBVector wrapLocationOffset;
};

class BBSimplePrediction
{
public:
	int angle;
	int pressure;
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
};

class BBFrameData
{
public:
	BBFrameData();
	int uid;
	int NMass;
	int ackCommand;
	int angle;
	int pressure;
	int nextNodeIdx;
	vector<BBFrameBallData> ballDatas;
};



#endif //BB_FRAME_DATA_H
