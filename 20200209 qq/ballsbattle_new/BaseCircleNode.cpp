#include "BaseCircleNode.h"
#include<math.h>
#include "BBMathUtils.h"

BaseCircleNode::BaseCircleNode() :
	idx(0),
	radius(0),
	treeIndex(0)
{
	location = BBVector::ZERO;
	rect = BBRect::ZERO;
}

void BaseCircleNode::ChangePosition(float x, float y)
{
	location.x = BBMathUtils::bb_fix_float(x);
	location.y = BBMathUtils::bb_fix_float(y);
	rect.setRect(location.x - radius, location.y - radius, location.x + radius, location.y + radius);
}