#ifndef BASE_CIRCLE_NODE_CPP
#define BASE_CIRCLE_NODE_CPP

#include "BBVector.h"
#include "BBRect.h"

class BaseCircleNode
{
public:
	BaseCircleNode();
	virtual ~BaseCircleNode() {};
	inline void OnRemoveFromTreeNode()
	{
		treeIndex = -1;
	}

	inline void ChangeTreeIdx(unsigned int newIdx)
	{
		treeIndex = newIdx;
	}

	virtual void ChangePosition(double x, double y);

	inline int GetRadius() const
	{
		return radius;
	}
	inline void SetRadius(int radius)
	{
		this->radius = radius;
	}

public:
	BBVector location;
	BBRect rect;
	unsigned int idx;
	unsigned int treeIndex;
	int radius;
};

#endif