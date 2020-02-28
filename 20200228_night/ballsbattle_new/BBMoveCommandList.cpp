#include "BBMoveCommandList.h"
#include "BBMoveManager.h"
#include "BBConfigManager.h"

enum BBCommandContants {
	MaxBBCommandQueueSize = 15,
	MaxBBServerExecuteCount = 3,
	MaxBBCommandSendCount = 1,
};

BBMoveCommandList::BBMoveCommandList():
	mLastCommandAck(-1),
	mControlMismatch(false),
	isServer(false)
{
}


BBMoveCommandList::~BBMoveCommandList()
{
}

//client collect next command
void BBMoveCommandList::collectCommand()
{
	if (isServer)
	{
		return;
	}
	if (mCommandVec.size() < MaxBBCommandQueueSize)
	{
		BBMoveManager::IncreaseFrame();
		BBMoveCommand command;
		command.angle = BBMoveManager::angle;
		command.pressure = BBMoveManager::pressure;
		command.isShoot = BBMoveManager::isShoot;
		command.isSplit = BBMoveManager::isSplit;
		command.checkSum = CHECKSUM_MISMATCH;
		command.idx = BBMoveManager::nextIndex;
		command.sendCount = 0;
		mCommandVec.emplace_back(command);
	}
}

//server push
void BBMoveCommandList::pushCommand(int angle, int pressure, bool isShoot, bool isSplit, unsigned int checkSum, int idx)
{
	BBMoveCommand command;
	command.angle = angle;
	command.pressure = pressure;
	command.isShoot = isShoot;
	command.isSplit = isSplit;
	command.checkSum = checkSum;
	command.idx = idx;
	mCommandVec.emplace_back(command);
}

void BBMoveCommandList::pushCommand(BBMoveCommand command)
{
	if (isBacklogged())
	{
		return;
	}
	mCommandVec.emplace_back(command);
}

//client block
bool BBMoveCommandList::isBacklogged()
{
	if (isServer)
	{
		return false;
	}
	return mCommandVec.size() >= BBConfigManager::maxCommandNoAck;
}

void BBMoveCommandList::getCommands(std::vector<BBMoveCommand>& commands)
{
	if (isServer)
	{
		for (int i = 0; i < mCommandVec.size(); i++)
		{
			commands.emplace_back(mCommandVec[i]);
			if (commands.size() >= MaxBBServerExecuteCount)
			{
				return;
			}
		}
	}
	else
	{
		if (mCommandVec.size() > 0)
		{
			commands.emplace_back(mCommandVec[0]);
		}
	}
}

void BBMoveCommandList::clearCommands(int count)
{
	if (count <= 0)
	{
		return;
	}
	if (count > mCommandVec.size())
	{
		return;
	}
	if (isServer)
	{
		mLastCommandAck = mCommandVec[count - 1].idx;
	}
	if (count == mCommandVec.size())
	{
		mCommandVec.clear();
	}
	else
	{
		while (count--)
		{
			mCommandVec.erase(mCommandVec.begin());
		}
	}
}

vector<BBMoveCommand> BBMoveCommandList::clientWriteSendCommands()
{
	vector<BBMoveCommand> ret;
	for (int i = 0; i < mCommandVec.size(); i++)
	{
		if (mCommandVec[i].sendCount < BBConfigManager::maxCommandNoAck)
		{
			mCommandVec[i].sendCount++;
			ret.emplace_back(mCommandVec[i]);
		}
	}
	return ret;
}

void BBMoveCommandList::clientSyncServerCommandAck(int serverAck)
{
	if (isServer)
	{
		return;
	}
	int clearCount = 0;
	while (mCommandVec.size() > clearCount)
	{
		if (mCommandVec[clearCount].idx <= serverAck)
		{
			clearCount++;
		}
		else
		{
			break;
		}
	}
	clearCommands(clearCount);
}