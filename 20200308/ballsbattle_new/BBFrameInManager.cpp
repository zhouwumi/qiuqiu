#include "BBFrameInManager.h"



BBFrameInManager::BBFrameInManager()
{
}


BBFrameInManager::~BBFrameInManager()
{
}

void BBFrameInManager::FrameClear()
{
	playerCommands.clear();
	playerQuitIds.clear();
	playerJoinIds.clear();
}

void BBFrameInManager::AddPlayerQuit(unsigned int uid)
{
	playerQuitIds.emplace_back(uid);
}

void BBFrameInManager::AddPlayerJoin(unsigned int uid)
{
	playerJoinIds.emplace_back(uid);
}

void BBFrameInManager::AddPlayerCommand(unsigned int uid, int angle, int pressure, bool isSplit, bool isShoot, int idx, unsigned int checkSum)
{
	BBMoveCommand command;
	command.angle = angle;
	command.pressure = pressure;
	command.isSplit = isSplit;
	command.isShoot = isShoot;
	command.idx = idx;
	command.checkSum = checkSum;

	if (playerCommands.find(uid) == playerCommands.end())
	{
		std::vector<BBMoveCommand> vec;
		vec.emplace_back(command);
		playerCommands.emplace(uid, vec);
	}
	else
	{
		std::vector<BBMoveCommand>& vec = playerCommands[uid];
		vec.emplace_back(command);
	}
}