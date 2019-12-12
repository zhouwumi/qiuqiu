#pragma once

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
	TOP = 4,
	BUTTOM = 5,
	LEFT = 6,
	RIGHT = 7,
	MAX = 8,
};

class QuadTree
{
public:
	QuadTree();
	void SetTreeWH(int width, int height);
	void SetTreeRect(int _depth, BBRect r);
	void SetCustomMaxNodeInQuad(int _maxCircleInQuad);
	void SetCustomMaxDepth(int _maxDepth);

	void PreBuildAllSubTrees(); //һ��ʼ�͹���������е�
	void OnBuildFinish(QuadTree* parentTree); //��������� ��Ҫ������

	void AddCircleNode(BaseCircleNode* node); //����µĽڵ�
	void RemoveCircleNode(BaseCircleNode* node);
	void UpdateCircleNode(BaseCircleNode* node); //��������ڵ����ǿ���

	QuadTree* FindTree(BaseCircleNode* node); //Ѱ�����ڵ���
	
	void GetAllHitCircleNodeIdxs(int treeIndex, std::vector<int>& idxs);
	void GetAllHitCircleNodeIdxs(QuadTree* tree, std::vector<int>& idxs);
	void GetAllHitCircleNodeIdxs(const BBRect& rect, std::vector<int>& idxs);

	QuadType GetQuadType(BaseCircleNode* node);
	QuadType GetQuadType(const BBRect& checkHit);

	unsigned int GetCurrTreeId();
	
private:
	BBRect _getRect(QuadType type);
	//��ȡ���µ�����Բ��Ϣ��idx
	void _getAllTreeNodeIdxs(const QuadTree* tree, std::vector<int>& idxs); 
	void _splitTree(); //������
	bool _buildSubTrees(); //��������

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

	int maxCircleInQuad; //ÿ���ڵ�洢���������
	int maxDepth;
};