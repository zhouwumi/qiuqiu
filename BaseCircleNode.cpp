#include "BaseCircleNode.h"



BaseCircleNode::BaseCircleNode(int i) :
	idx(i),
	treeIndex(0)
{
}

void BaseCircleNode::ChangePosition(int x, int y, int c)
{
	positionX = x;
	positionY = y;
	circle = c;
	minX = x - circle;
	maxX = x + circle;
	minY = y - circle;
	maxY = y + circle;
}

void BaseCircleNode::ChangeTreeIdx(unsigned int newIdx)
{
	treeIndex = newIdx;
}

void BaseCircleNode::OnRemoveFromTreeNode()
{
	treeIndex = -1;
}