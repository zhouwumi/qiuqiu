#include "CommandList.h"

enum CommandContants {
	MaxCommandQueueSize = 30,
	MaxCommandCount = 30,
};

CommandList::CommandList():
	isServer(false)
{
}


CommandList::~CommandList()
{
}

void CommandList::collectCommand()
{
	BaseCommand command;
	if (getNextCommand(command))
	{
		command.checkSum = CHECKSUM_MISMATCH;
		pushCommand(command);
	}
}

bool CommandList::getNextCommand(BaseCommand& command)
{
	if (mCommandVec.size() > MaxCommandQueueSize)
		return false;
	return true;
}

void CommandList::pushCommand(BaseCommand& command)
{
	int next_id = mFirstMoveIndex + mCommandVec.size();
	int sz = mCommandVec.size();
	mCommandVec.emplace_back(command);
	mCommandVec[sz].id = next_id;
	mCommandVec[sz].sendCount = 0;
}

//client backlogged
bool CommandList::isBacklogged()
{
	if (isServer)
	{
		return false;
	}
	return mLastClientMove - mFirstMoveIndex + 1 == mCommandVec.size() && mCommandVec.size() >= MaxCommandCount;
}

int CommandList::getCommands(BaseCommand** commandPtrs, int* numCommand)
{
	if (isServer)
	{
		if (mCommandVec.size() > 0)
		{
			*commandPtrs = &mCommandVec[0];
		}
		
		*numCommand = mCommandVec.size();
	}
	else
	{
		if (mCommandVec.size() > 0)
		{
			*commandPtrs = &mCommandVec[0];
			*numCommand = 1;
		}
		else
		{
			*numCommand = 0;
		}
	}
	return *numCommand;
}

void CommandList::clearCommands(int count)
{
	if (isServer)
	{
		for (int i = 0; i < count; i++)
		{
			if (mCommandVec[i].checkSum == CHECKSUM_MISMATCH)
				mControlMismatch = true;
			else
				mControlMismatch = false;
		}
		if (count == mCommandVec.size())
			mCommandVec.clear();
		else
			while (count--)
			{
				mCommandVec.erase(mCommandVec.begin());
			}
	}
	else
	{
		mLastClientMove += count;
	}
}

vector<BaseCommand> CommandList::clientWriteSendCommands()
{
	vector<BaseCommand> commands;
	if (mCommandVec.size() > 0)
	{
		commands.emplace_back(mCommandVec.back());
	}
	return commands;
}

void CommandList::serverReadSendCommand(BaseCommand command)
{
	if (mLastCommandAck < command.id)
	{
		mLastCommandAck = command.id;
		mCommandVec.emplace_back(command);
	}
}

int CommandList::serverGetCommandAck()
{
	return mLastCommandAck;
}

void CommandList::clientSetCommandAck(int serverAck)
{
	mLastCommandAck = serverAck;
	while (mFirstMoveIndex <= serverAck)
	{
		if (mCommandVec.size())
		{
			mCommandVec.erase(mCommandVec.begin());
			mFirstMoveIndex++;
		}
		else
		{
			mFirstMoveIndex = mLastClientMove + 1;
		}
	}
}