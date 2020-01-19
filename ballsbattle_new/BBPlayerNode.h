#ifndef BB_PLAYER_NODE_CPP
#define BB_PLAYER_NODE_CPP
#include"BBMovingBall.h"
#include "BBVector.h"
#include<vector>
#include<unordered_map>


class PlayerNode : public MovingBall
{
public:
	PlayerNode();
	~PlayerNode();

	void ChangeCd(int timeDelta);

	BBVector& InitMove();
	void CalculateInitMoveParams(int radius, int frame, int initSpeed, int finalSpeed);
	unsigned int GetCrc();
public:
	int Uid;
	int Cd;//不能融合的CD
	int Protect;//刚出生的球不能被吃和不能吃别人的CD
	//初始的一些参数记录		
	int initSpeed;
	int initDeltaSpeed;
};

class Player
{
public:
	Player(int id);
	~Player();

	void ResetPoint(int x, int y);
	void UpdateFinalPoint(int x, int y);

	BBRect GetGroupRect();
	void RemoveMass();

	unsigned int GetCrc();
public:
	std::vector<PlayerNode*> vecPlayerNodes;

	int uid;
	bool Stopped;
	BBPoint FinalPoint;
	BBVector Direction;
	int NMass;
};

#endif