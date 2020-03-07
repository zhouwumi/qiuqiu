#include "QuadNode.h"
#include "QuadTree.h"

QuadNode::QuadNode()
{
}

void QuadNode::OnAttachTree(QuadTree* _tree)
{
	this->tree = _tree;
}

void QuadNode::InsertCircleNode(BaseCircleNode* node)
{
	subNodes.emplace_back(node);
	node->ChangeTreeIdx(this->tree->treeIdx);
}

void QuadNode::RemoveCircleNode(BaseCircleNode* node)
{
	int size = subNodes.size();
	for (auto iter = subNodes.begin(); iter != subNodes.end(); iter++)
	{
		if (*iter == node)
		{
			subNodes.erase(iter);
			node->OnRemoveFromTreeNode();
			break;
		}
	}
}

QuadNode::~QuadNode()
{
}