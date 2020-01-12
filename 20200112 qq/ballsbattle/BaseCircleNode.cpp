#include "BaseCircleNode.h"



BaseCircleNode::BaseCircleNode() :
	Idx(0),
	radius(0),
	treeIndex(0),
	positionX(0),
	positionY(0),
	minX(0),
	minY(0),
	maxX(0),
	maxY(0)
{
}

void BaseCircleNode::ChangePosition(int x, int y)
{
	positionX = x;
	positionY = y;
	minX = x - radius;
	maxX = x + radius;
	minY = y - radius;
	maxY = y + radius;

	Location.x = x;
	Location.y = y;
}

void BaseCircleNode::ChangeTreeIdx(unsigned int newIdx)
{
	treeIndex = newIdx;
}

void BaseCircleNode::OnRemoveFromTreeNode()
{
	treeIndex = -1;
}