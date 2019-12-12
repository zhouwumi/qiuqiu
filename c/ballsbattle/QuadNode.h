#pragma once

#include"BaseCircleNode.h"
#include "BBRect.h"
#include<vector>

class QuadTree;

class QuadNode
{
public:
	QuadNode();
	void InsertCircleNode(BaseCircleNode* node);
	void RemoveCircleNode(BaseCircleNode* node);
	void OnAttachTree(QuadTree* _tree);

	~QuadNode();
public:
	BBRect rect;
	std::vector<BaseCircleNode*> subNodes;
	QuadTree* tree;
};