#include "BaseCircleNode.h"

BaseCircleNode::BaseCircleNode() :
	Idx(0),
	_radius(0),
	treeIndex(0)
{
	Location = BBPoint::ZERO;
	rect = BBRect::ZERO;
}

void BaseCircleNode::ChangePosition(int x, int y)
{
	Location.x = x;
	Location.y = y;
	rect.setRect(x - _radius, y - _radius, x + _radius, y + _radius);
}