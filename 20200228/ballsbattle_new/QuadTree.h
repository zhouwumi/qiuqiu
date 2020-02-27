#ifndef BB_QUAD_TREE_CPP
#define BB_QUAD_TREE_CPP

#include "BBRect.h"
#include "BaseCircleNode.h"
#include "QuadNode.h"

#include<vector>
#include<unordered_map>

enum QuadType
{
	All = -1,
	UL = 0,// up-left
	UR = 1,//up-right
	BL = 2,//buttom-left
	BR = 3,//buttom_right
	MAX = 4,
};

class QuadTree
{
public:
	QuadTree();
	void SetTreeWH(int width, int height);
	void SetTreeRect(int _depth, BBRect r);
	void SetCustomMaxNodeInQuad(int _maxCircleInQuad);
	void SetCustomMaxDepth(int _maxDepth);

	void PreBuildAllSubTrees(); //一开始就构建完成所有的
	void OnBuildFinish(QuadTree* parentTree); //树构建完成 需要主动调

	void AddCircleNode(BaseCircleNode* node); //添加新的节点
	void RemoveCircleNode(BaseCircleNode* node);
	void UpdateCircleNode(BaseCircleNode* node); //更新这个节点在那棵树

	QuadTree* FindTree(BaseCircleNode* node); //寻找所在的树
	
	void GetAllHitCircleNodeIdxs(int treeIndex, std::vector<int>& idxs);
	void GetAllHitCircleNodeIdxs(std::vector<int>& idxs);
	void GetAllHitCircleNodeIdxs(const BBRect& rect, std::vector<int>& idxs);

	QuadType GetQuadType(BaseCircleNode* node);
	QuadType GetQuadType(const BBRect& checkHit);

	unsigned int GetCurrTreeId();
	
private:
	BBRect _getRect(QuadType type);
	//获取树下的所有圆信息的idx
	void _getAllTreeNodeIdxs(const QuadTree* tree, std::vector<int>& idxs);
	void _getAllParentNodeIdx(std::vector<int>& idxs);
	void _getAllSelfChildNodeIdx(std::vector<int>& idxs);
	void _splitTree(); //分子树
	bool _buildSubTrees(); //构建子树

public:
	unsigned int treeIdx;
	QuadTree* rootTree;
	QuadTree* parentTree;
	QuadType treeType;
	bool isRoot;

	std::vector<QuadTree> subTrees;
	QuadNode rootNode;
	std::unordered_map<unsigned int, QuadTree*> mapTrees;
		
private:
	BBRect rect;
	int depth;

	int maxCircleInQuad; //每个节点存储的最大数量
	int maxDepth;
};

#endif