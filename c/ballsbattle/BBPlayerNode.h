#pragma once
#include"BBMovingBall.h"
#include<vector>

class PlayerNode : public MovingBall
{
public:
	PlayerNode();
	~PlayerNode();

	void ChangeCd(int timeDelta);

	//刺球分裂
	void SpikySplit(int maxChildNode, int spikyMass, std::vector<PlayerNode*>& newPlayerNodes);
	PlayerNode* SelfSplit();
	BBVector& InitMove();
	void CalculateInitMoveParams(int radius, int frame, float initSpeed, int finalSpeed);

public:
	int Uid;
	int Cd;//不能融合的CD
	int Protect;//刚出生的球不能被吃和不能吃别人的CD
	//初始的一些参数记录		
	float initSpeed;
	float initDeltaSpeed;
};

class Player
{
public:
	Player(int id);
	~Player();

	void ResetPoint(int x, int y);
	void UpdateFinalPoint(int x, int y);
public:
	std::vector<PlayerNode*> vecPlayerNodes;
	int uid;
	BBPoint FromPoint;
	BBPoint FinalPoint;
};
