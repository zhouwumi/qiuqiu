#ifndef BB_SYNC_OBJECT_H
#define BB_SYNC_OBJECT_H

#include "BBFrameData.h"
#include "BBMoveCommandList.h"

class BBSyncObject
{
public:
	BBSyncObject();
	~BBSyncObject();

	virtual void processTick(BBMoveCommand& command) {};
	//server send frame data to client
	virtual BBFrameData serverGetFrameData() { BBFrameData ret; return ret; };
	//client sync server frame data
	virtual void clientReadFrameData(BBFrameData frameData) {};
	//server pack other player data to client
	virtual BBFrameData packUpdate() { BBFrameData ret; return ret; };
	//client uppack other player data
	virtual void unPackUpdate(BBFrameData frameData) {};

	//client get server sync state, quick run frame to catch up now
	virtual void clientCatchup() {};

public:
	BBBallDeltaData mDelta;
	BBMoveCommandList mCommandList;
};
#endif //BB_SYNC_OBJECT_H
