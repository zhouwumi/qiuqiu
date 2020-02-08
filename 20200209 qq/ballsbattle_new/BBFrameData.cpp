#include "BBFrameData.h"
BBFrameBallData::BBFrameBallData()
{
}


BBFrameBallData::~BBFrameBallData()
{
}

BBBallDeltaData::BBBallDeltaData():
	wrapTicks(0)
{
	location = BBVector::ZERO;
	wrapLocationOffset = BBVector::ZERO;
}

BBBallDeltaData::~BBBallDeltaData()
{
	
}

BBFrameData::BBFrameData()
{

}

BBPredictionData::BBPredictionData():
	mPredictionRight(true),
	lastAngle(0),
	lastPressure(0)
{
}

BBPredictionData::~BBPredictionData()
{

}

void BBPredictionData::ClearData()
{
	this->lastAngle = 0;
	this->lastPressure = 0;
	this->predictionCommands.clear();
}
