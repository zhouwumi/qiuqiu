#include "QuadTreeTestCase.h"
#include <random>
#include "cocos2d.h"
#include <sstream>


int _next_content_node_idx = 1;
int get_next_node_idx()
{
	return _next_content_node_idx++;
}

QuadTreeTestCase::QuadTreeTestCase(int w, int h):
width(w),
height(h)
{
	quadTree.SetTreeWH(width, height);
	quadTree.OnBuildFinish(&quadTree);
	//quadTree.PreBuildAllSubTrees();
}


QuadTreeTestCase::~QuadTreeTestCase()
{
	for each (auto iter in allBaseCircleNodes)
	{
		BaseCircleNode* BaseCircleNode = iter.second;
		delete BaseCircleNode;
	}
	allBaseCircleNodes.clear();
}

void QuadTreeTestCase::AddContentNodes()
{
	for (int i = 0; i < 1000; i++)
	{
		BaseCircleNode* baseCircleNode = new BaseCircleNode(get_next_node_idx());
		int x = 1 + (int)(width * rand() / (RAND_MAX + 1.0));
		int y = 1 + (int)(height * rand() / (RAND_MAX + 1.0));
		int c = 0;
		baseCircleNode->ChangePosition(x, y, c);
		allBaseCircleNodes.emplace(baseCircleNode->Idx, baseCircleNode);
		quadTree.AddCircleNode(baseCircleNode);
	}
}

void QuadTreeTestCase::SwapNodes(int idx1, int idx2)
{
	if (allBaseCircleNodes.find(idx1) == allBaseCircleNodes.end() || allBaseCircleNodes.find(idx2) == allBaseCircleNodes.end())
	{
		std::ostringstream stream;
		stream << idx1 << "," << idx2;
		cocos2d::log("idx is not valid:  %s", stream.str().c_str());
		return;
	}
	
	int oldNodeTree1 = quadTree.mapTrees[allBaseCircleNodes[idx1]->treeIndex]->treeIdx;
	int oldNodeTree2 = quadTree.mapTrees[allBaseCircleNodes[idx2]->treeIndex]->treeIdx;

	int idxPositionX = allBaseCircleNodes[idx2]->positionX;
	int idxPositionY = allBaseCircleNodes[idx2]->positionY;
	allBaseCircleNodes[idx2]->ChangePosition(allBaseCircleNodes[idx1]->positionX, allBaseCircleNodes[idx1]->positionY, allBaseCircleNodes[idx2]->radius);
	allBaseCircleNodes[idx1]->ChangePosition(idxPositionX, idxPositionY, allBaseCircleNodes[idx1]->radius);
	quadTree.UpdateCircleNode(allBaseCircleNodes[idx1]);
	quadTree.UpdateCircleNode(allBaseCircleNodes[idx2]);

	int newNodeTree1 = quadTree.mapTrees[allBaseCircleNodes[idx1]->treeIndex]->treeIdx;
	int newNodeTree2 = quadTree.mapTrees[allBaseCircleNodes[idx2]->treeIndex]->treeIdx;

	if (newNodeTree1 != oldNodeTree2 || newNodeTree2 != oldNodeTree1)
	{
		_ASSERT("swap fail !  reason: tree not swap right");
	}
}

void QuadTreeTestCase::DebugLog()
{
	std::vector<const QuadTree*> trees;
	trees.emplace_back(&quadTree);

	std::vector<const QuadTree*> copyTrees;
	while (trees.size() > 0)
	{
		for (int i = 0; i < trees.size(); i++)
		{
			const QuadTree* tree = trees[i];
			
			for each (const QuadTree& subTree in tree->subTrees)
			{
				copyTrees.emplace_back(&subTree);
			}

			std::ostringstream stream;
			for each (auto node in tree->rootNode.subNodes)
			{
				stream << node->Idx << ",";
			}
			
			std::ostringstream stream1;
			stream1 << tree->treeIdx;

			const QuadTree* tempTree = tree;
			while (!tempTree->isRoot)
			{
				stream1 << " <-- "<< tempTree->parentTree->treeIdx;
				tempTree = tempTree->parentTree;
			}

			std::ostringstream stream2;
			stream2 << tree->rootNode.rect.minX << "," << tree->rootNode.rect.minY << "," << tree->rootNode.rect.maxX << "," << tree->rootNode.rect.maxY << ",";

			std::string str3 = stream2.str();
			std::string str1 = stream.str();
			std::string str2 = stream1.str();
			cocos2d::log("treeId = %s scope %s, BaseCircleNodes = %s", str2.c_str(), str3.c_str(), str1.c_str());
		}

		trees.clear();
		trees = std::move(copyTrees);
		copyTrees.clear();
	}
}