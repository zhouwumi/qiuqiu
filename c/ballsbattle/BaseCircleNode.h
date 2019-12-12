#pragma once
#include "BBPoint.h"
#include "BBRect.h"

class BaseCircleNode
{
public:
	BaseCircleNode()
	{}
	~BaseCircleNode()
	{}
	BaseCircleNode(int i);
	void OnRemoveFromTreeNode();

	void ChangeTreeIdx(unsigned int newIdx);
	virtual void ChangePosition(int x, int y, int c);

	inline int GetRadius() const
	{
		return radius;
	}

	inline BBPoint GetLoction()
	{
		return Location;
	}


	inline BBRect Bound()
	{
		return BBRect(minX, minY, maxX, maxY);
	}

public:
	BBPoint Location;
	unsigned int Idx;
	unsigned int treeIndex;

	int positionX;
	int positionY;
	int radius;
	int minX;
	int minY;
	int maxX;
	int maxY;
};

