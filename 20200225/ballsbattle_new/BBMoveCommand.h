#ifndef BB_MOVE_COMMAND_H
#define BB_MOVE_COMMAND_H

#include "BBFrameData.h"
#include "BBPredictionData.h"

enum CheckSum
{
	CHECKSUM_MISMATCH = 1
};

class BBMoveCommand
{
public:
	BBMoveCommand();
	~BBMoveCommand();

	void ClearData();
	
	static BBMoveCommand Create();

	int angle;
	int pressure;
	bool isSplit; //split command
	bool isShoot; //shoot command

	int idx;
	int sendCount; // client
	unsigned int checkSum; // client server

	bool IsEqual(BBSimplePrediction& command);
};

#endif //BB_MOVE_COMMAND_H

