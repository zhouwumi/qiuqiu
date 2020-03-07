#ifndef BB_OBJECTS_CPP
#define BB_OBJECTS_CPP
#include"BaseCircleNode.h"
//食物
class Food : public BaseCircleNode
{
public:
	Food();
	~Food();
};

//球
class BaseBall :public BaseCircleNode
{
public:
	BaseBall();
	~BaseBall();

	void SetBallMass(int mass);
	inline int GetBallMass()
	{
		return mass;
	}
protected:
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