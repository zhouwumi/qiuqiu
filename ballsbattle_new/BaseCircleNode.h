#ifndef BASE_CIRCLE_NODE_CPP
#define BASE_CIRCLE_NODE_CPP

#include "BBPoint.h"
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

	virtual void ChangePosition(int x, int y);

	inline int GetRadius() const
	{
		return _radius;
	}
	inline void SetRadius(int radius)
	{
		_radius = radius;
	}

public:
	BBPoint Location;
	BBRect rect;
	unsigned int Idx;
	unsigned int treeIndex;
	int _radius;
};

#endif