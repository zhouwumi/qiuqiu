#ifndef BB_FRAME_IN_MANAGER_CPP
#define BB_FRAME_IN_MANAGER_CPP

#include<vector>
#include<unordered_map>
#include "BBMoveCommand.h"

class BBFrameInManager
{
public:
	BBFrameInManager();
	~BBFrameInManager();

	void FrameClear();

	//server
	void AddPlayerCommand(unsigned int uid, int angle, int pressure, bool isSplit, bool isShoot, int idx, unsigned int checkSum);
	void AddPlayerQuit(unsigned int uid);
	void AddPlayerJoin(unsigned int uid);
public:
	std::unordered_map<unsigned int, std::vector<BBMoveCommand>> playerCommands;
	std::vector<unsigned int> playerQuitIds;
	std::vector<unsigned int> playerJoinIds;
};
#endif
