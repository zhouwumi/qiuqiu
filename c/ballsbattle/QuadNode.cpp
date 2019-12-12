#include "QuadNode.h"
#include "QuadTree.h"

unsigned int _next_tree_node_id = 1;
unsigned int _get_next_tree_node_id()
{
	return _next_tree_node_id++;
}

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