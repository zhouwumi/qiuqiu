#ifndef BASE_FRAME_SYNC_DATA_CPP
#define BASE_FRAME_SYNC_DATA_CPP

#include "BBPoint.h"
#include "BBVector.h"

enum CheckSum
{
	CHECKSUM_MISMATCH = 1
};

class BaseCommand
{
public:
	BaseCommand();
public:
	int id;
	int sendCount; // client
	int checkSum; // client server
};

class BaseFrameData
{
public:
	BBPoint pos;
	BBVector velocity;
	BaseFrameData();
};

class BaseFrameDataDelta
{
public:
	BaseFrameDataDelta();
	BaseCommand lastCommand;
	BBPoint pos;
	BBVector velocity;

	int wrapTicks;
	BBVector wrapPosOffset;
	BBVector wrapVelocityOffset;
};

#endif