#pragma once
#include "QuadTree.h"
#include<unordered_map>

class QuadTreeTestCase
{
public:
	QuadTreeTestCase(int w, int h);
	~QuadTreeTestCase();

	void AddContentNodes();
	void DebugLog();
	void SwapNodes(int idx1, int idx2);
private:
	std::unordered_map<int, BaseCircleNode*> allBaseCircleNodes;
	QuadTree quadTree;
	int width;
	int height;
};

