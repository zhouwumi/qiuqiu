#include "BaseFrameSyncData.h"

const BaseCommand NullCommand;

BaseCommand::BaseCommand():
	id(0),
	sendCount(0),
	checkSum(CHECKSUM_MISMATCH)
{
}


BaseFrameData::BaseFrameData() :
	pos(BBPoint::ZERO),
	velocity(BBVector::ZERO)
{

}

BaseFrameDataDelta::BaseFrameDataDelta() :
	pos(BBPoint::ZERO),
	velocity(BBVector::ZERO),
	wrapTicks(0),
	wrapPosOffset(BBVector::ZERO),
	wrapVelocityOffset(BBVector::ZERO)
{
	lastCommand = NullCommand;
}