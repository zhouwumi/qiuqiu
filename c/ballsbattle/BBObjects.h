#pragma once
#include"BaseCircleNode.h"
//ʳ��
class Food : public BaseCircleNode
{
public:
	Food();
	~Food();
public:
	unsigned int posKey;
};

//��
class BaseBall :public BaseCircleNode
{
public:
	BaseBall();
	~BaseBall();
	inline int Mass()
	{
		return mass;
	}

	void SetMass(int _mass);

	inline BBRect FoodBound(int extraRadius);

public:
	int mass;
	int FromMass;
};

//����
class SpikyBall : public BaseBall
{
public:
	SpikyBall();
	~SpikyBall();
};