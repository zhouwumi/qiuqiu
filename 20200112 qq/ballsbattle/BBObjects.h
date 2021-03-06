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

	void SetBallMass(int _mass);
public:
	int mass;
};

//刺球
class SpikyBall : public BaseBall
{
public:
	SpikyBall();
	~SpikyBall();
};

#endif