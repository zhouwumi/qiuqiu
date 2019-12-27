#ifndef COMMAND_CPP
#define COMMAND_CPP
#include<vector>

class SimpleJoinOperate
{
public:
	int uid;
};

class SimpleSplitOperate
{
public:
	int uid;
};

class SimpleMoveOperate
{
public:
	int uid;
	int angle;
	int pressure;
};

class SimplePlayerQuitOperate
{
public:
	int uid;
};

class SimpleNewFoodSpikyOperate
{
public:
	std::vector<int> newInfoList;
};

class SimplePlayerShootOperate
{
public:
	int uid;
};

#endif