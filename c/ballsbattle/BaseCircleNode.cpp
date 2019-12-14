#include "BaseCircleNode.h"



BaseCircleNode::BaseCircleNode(int i) :
	Idx(i),
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

void BaseCircleNode::ChangePosition(int x, int y, int c)
{
	positionX = x;
	positionY = y;
	radius = c;
	minX = x - radius;
	maxX = x + radius;
	minY = y - radius;
	maxY = y + radius;

	Location.x = x;
	Location.y = y;
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