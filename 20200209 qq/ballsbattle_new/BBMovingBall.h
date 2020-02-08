#ifndef BB_MOVING_BALL_CPP
#define BB_MOVING_BALL_CPP
#include"BBObjects.h"
#include"BBVector.h"
#include<unordered_map>

// 移动的球
class MovingBall : public BaseBall
{
public:
	MovingBall();
	~MovingBall();
	void SetBallMass(int _mass);
	void SetSpeedVec(float targetX, float targetY);
	void ChangeDeltaMass(int deltaMas);
public:
	int fromId; //从哪个球分裂出来
	BBVector currentSpeedVec;//当前的速度
	int speed;//速度向量的长度
};


//孢子
class Spore : public MovingBall
{
public:
	Spore();
	~Spore();
	void InitMove();
	void CalculateInitMoveParams(int radius, int frame, int initSpeed);
public:
	int uid;
	int cd;
	//初始的一些参数记录		
	int initSpeed;
	int initDeltaSpeed;
	int initStopFrame; //停下来降为0的帧数
};

#endif