#ifndef BASE_SYNC_OBJECT_H
#define BASE_SYNC_OBJECT_H
#include "BBPoint.h"
#include "BBVector.h"
#include "BaseFrameSyncData.h"
#include "CommandList.h"

class BaseSyncObject
{
public:
	BaseSyncObject();
	~BaseSyncObject();

	virtual void processTick(BaseCommand* command);
	//server send frame data to client
	virtual BaseFrameData serverGetFrameData();
	//client sync server frame data
	virtual void clientReadFrameData(BaseFrameData frameData);
	//server pack other player data to client
	virtual BaseFrameData packUpdate();
	//client uppack other player data
	virtual void unPackUpdate(BaseFrameData frameData);

	//client get server sync state, quick run frame to catch up now
	virtual void clientCatchup();
public:
	int id;
	BBPoint mPos;
	BBVector mVelocity;
	BBPoint mLocalPos;
	BBPoint mLocalVelocity;
	int mPredictionCount;
	BaseFrameDataDelta mDelta;
	CommandList mCommandList;
};

#endif //BASE_SYNC_OBJECT_H
