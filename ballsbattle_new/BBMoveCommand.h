#ifndef BB_MOVE_COMMAND_H
#define BB_MOVE_COMMAND_H

#include "BaseFrameSyncData.h"

class BBMoveCommand : public BaseCommand
{
public:
	BBMoveCommand();
	~BBMoveCommand();
	
	static BBMoveCommand Create();

	int angle;
	int pressure;
	bool isSplit; //split command
	bool isShoot; //shoot command
};

#endif //BB_MOVE_COMMAND_H

