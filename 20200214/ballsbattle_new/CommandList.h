#ifndef COMMAND_LIST_CPP
#define COMMAND_LIST_CPP

#include<vector>
#include "BaseFrameSyncData.h"

using namespace std;
class CommandList
{
public:
	CommandList();
	~CommandList();

	//client get next command
	virtual void collectCommand();
	virtual bool getNextCommand(BaseCommand& command);
	//push command
	virtual void pushCommand(BaseCommand& command);
	//if the input queue is full block
	virtual bool isBacklogged();

	//client get server checksum mismatch, sync state info then quick catch up now
	//virtual void clientCatchup();

	virtual int getCommands(BaseCommand** commandPtrs, int* numCommand);
	virtual void clearCommands(int count);

	virtual void advanceMove() {};

	virtual vector<BaseCommand> clientWriteSendCommands();
	virtual void serverReadSendCommand(BaseCommand command);
	//server get ack
	virtual int serverGetCommandAck();
	//client sync server ack
	virtual void clientSetCommandAck(int serverAck);
public:
	vector<BaseCommand> mCommandVec;
	int mLastCommandAck;
	int mLastClientMove;
	int mFirstMoveIndex;
	bool mControlMismatch;
	bool isServer;
};

#endif