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
	fixedLocation = BBVector::ZERO;
}

BBBallDeltaData::~BBBallDeltaData()
{
	
}

BBFrameData::BBFrameData()
{

}

