#include "BBPredictionData.h"

BBPredictionData::BBPredictionData() :
	mPredictionRight(true),
	canSkipSync(true),
	lastAngle(0),
	lastPressure(0),
	lastRunGameFrame(0)
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
