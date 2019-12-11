#pragma once
class BaseCircleNode
{
public:
	BaseCircleNode(int i);
	void OnRemoveFromTreeNode();

	void ChangeTreeIdx(unsigned int newIdx);
	void ChangePosition(int x, int y, int c);
public:
	unsigned int idx;
	unsigned int treeIndex;

	int positionX;
	int positionY;
	int circle;
	int minX;
	int minY;
	int maxX;
	int maxY;
};

