#pragma once
#include"BBObjects.h"
#include"BBVector.h"
// 移动的球
class MovingBall : public BaseBall
{
public:
	MovingBall();
	~MovingBall();

	void Move(BBVector v);
	void UpdateLocation(BBPoint v);

	inline double Speed()
	{
		return speed;
	}

	void SetMass(int _mass);

	void SetSpeedVec(int targetX, int targetY);
	inline void SaveMass()
	{
		FromMass = mass;
	}

	void ChangeMass(int deltaMas);

	void Stop();

public:
	int Init; //停下来降为0的帧数
	int FromId; //从哪个球分裂出来
	BBPoint FromLocation; //上一帧的位置
	BBVector Direction; //当前的方向
	BBVector Current;//当前的速度
	BBVector Delta;//当前的加速度
	BBVector Final;//目标速度
	double speed;//速度向量的长度
};

//孢子
class Spore : public MovingBall
{
public:
	Spore();
	~Spore();
	BBVector InitMove();

public:
	int Uid;
	int Cd;
};