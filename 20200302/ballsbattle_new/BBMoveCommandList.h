#ifndef BB_MOVE_COMMAND_LIST_H
#define BB_MOVE_COMMAND_LIST_H

#include "BBMoveCommand.h"
#include <vector>
using namespace std;

class BBMoveCommandList
{
public:
	BBMoveCommandList();
	~BBMoveCommandList();

	//client get next command
	virtual void collectCommand();
	//if the input queue is full block
	virtual bool isBacklogged();

	//client get server checksum mismatch, sync state info then quick catch up now
	//virtual void clientCatchup();

	virtual void pushCommand(int angle, int pressure, bool isShoot, bool isSplit, unsigned int checkSum, int idx);
	virtual void pushCommand(BBMoveCommand command);
	virtual void getCommands(std::vector<BBMoveCommand>& commands);
	virtual void clearCommands(int count);

	virtual vector<BBMoveCommand> clientWriteSendCommands();
	//virtual void serverReadClientCommand(BBMoveCommand command);
	//server get ack
	inline int serverGetCommandAck() { return mLastCommandAck; };
	//client sync server ack
	virtual void clientSyncServerCommandAck(int serverAck);
public:
	vector<BBMoveCommand> mCommandVec;
	int mLastCommandAck;
	bool mControlMismatch;
	bool isServer;
};
#endif //BB_MOVE_COMMAND_LIST_H

