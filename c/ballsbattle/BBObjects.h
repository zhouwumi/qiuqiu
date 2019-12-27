#ifndef BB_OBJECTS_CPP
#define BB_OBJECTS_CPP
#include"BaseCircleNode.h"
//食物
class Food : public BaseCircleNode
{
public:
	Food();
	~Food();
public:
	unsigned int posKey;
};

//球
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

//刺球
class SpikyBall : public BaseBall
{
public:
	SpikyBall();
	~SpikyBall();
};

#endif