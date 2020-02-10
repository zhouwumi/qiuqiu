#include "BaseSyncObject.h"



BaseSyncObject::BaseSyncObject():
	id(0),
	mPos(BBPoint::ZERO),
	mVelocity(BBVector::ZERO),
	mPredictionCount(0)
{
}


BaseSyncObject::~BaseSyncObject()
{
}

void BaseSyncObject::processTick(BaseCommand* command)
{

}

BaseFrameData BaseSyncObject::serverGetFrameData()
{
	BaseFrameData ret;
	return ret;
}

void BaseSyncObject::clientReadFrameData(BaseFrameData frameData)
{

}

BaseFrameData BaseSyncObject::packUpdate()
{
	BaseFrameData ret;
	ret.pos = mPos;
	ret.velocity = mVelocity;
	return ret;
}

void BaseSyncObject::unPackUpdate(BaseFrameData frameData)
{
}

void BaseSyncObject::clientCatchup()
{

}