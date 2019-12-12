#pragma once
#include"BBObjects.h"
#include"BBVector.h"
// �ƶ�����
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
	int Init; //ͣ������Ϊ0��֡��
	int FromId; //���ĸ�����ѳ���
	BBPoint FromLocation; //��һ֡��λ��
	BBVector Direction; //��ǰ�ķ���
	BBVector Current;//��ǰ���ٶ�
	BBVector Delta;//��ǰ�ļ��ٶ�
	BBVector Final;//Ŀ���ٶ�
	double speed;//�ٶ������ĳ���
};

//����
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