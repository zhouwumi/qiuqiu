#include "QuadTree.h"

#define MAX_DEPTH				5
#define QUAD_COUNT				5
#define MAX_CIRCLE_COUNT		20

QuadTree::QuadTree():
	depth(1),
	rootTree(this),
	treeType(QuadType::All),
	isRoot(true),
	treeIdx(1),
	parentTree(NULL),
	maxCircleInQuad(MAX_CIRCLE_COUNT),
	maxDepth(MAX_DEPTH)
{
}

void QuadTree::SetTreeWH(int width, int height)
{
	rect.setRect(0, 0, width, height);
	rootNode.rect.setRect(0, 0, width, height);
}

void QuadTree::SetTreeRect(int _depth, BBRect r)
{
	depth = _depth;
	rect = r;
	rootNode.rect = r;
}

void QuadTree::SetCustomMaxNodeInQuad(int _maxCircleInQuad)
{
	this->maxCircleInQuad = _maxCircleInQuad;
}

void QuadTree::SetCustomMaxDepth(int _maxDepth)
{
	this->maxDepth = _maxDepth;
}

void QuadTree::PreBuildAllSubTrees()
{
	_buildSubTrees();
	for (int i = 0; i < subTrees.size(); i++)
	{
		QuadTree& tree = subTrees[i];
		tree.PreBuildAllSubTrees();
	}
}

void QuadTree::OnBuildFinish(QuadTree* parentTree)
{
	if (parentTree && parentTree != this)
	{
		this->isRoot = false;
		this->parentTree = parentTree;
		this->rootTree = parentTree->rootTree;
		this->SetCustomMaxNodeInQuad(rootTree->maxCircleInQuad);
		this->SetCustomMaxDepth(rootTree->maxDepth);
	}
	else
	{
		this->isRoot = true;
	}

	treeIdx = this->GetCurrTreeId();
	rootNode.OnAttachTree(this);
	rootTree->mapTrees.emplace(treeIdx, this);
}

void QuadTree::AddCircleNode(BaseCircleNode* node)
{
	if (subTrees.size() == 0)
	{
		rootNode.InsertCircleNode(node);
		if (rootNode.subNodes.size() > maxCircleInQuad) {
			_splitTree();
		}
	}
	else {
		QuadType quadType = GetQuadType(node);
		if (quadType == QuadType::All)
		{
			rootNode.InsertCircleNode(node);
		}
		else
		{
			QuadTree& subTree = subTrees[quadType];
			subTree.AddCircleNode(node);
		}
	}
}

void QuadTree::RemoveCircleNode(BaseCircleNode* node)
{
	if (!node)
	{
		return;
	}
	QuadTree* quadTree = FindTree(node);
	if (quadTree)
	{
		quadTree->rootNode.RemoveCircleNode(node);
	}
}

void QuadTree::UpdateCircleNode(BaseCircleNode* node)
{
	if (!isRoot)
	{
		// _ASSERT("invalid tree call UpdateBaseCircleNode");
		return;
	}
	//version1:先这么简单的移除再插入吧。还有优化空间
	RemoveCircleNode(node);
	AddCircleNode(node);
}


QuadTree*  QuadTree::FindTree(BaseCircleNode* node)
{
	if (!node)
	{
		return NULL;
	}
	unsigned int treeIndex = node->treeIndex;
	if (mapTrees.size() > 0)
	{
		if (mapTrees.find(treeIndex) != mapTrees.end())
		{
			return mapTrees[treeIndex];
		}
	}
	else if (rootTree)
	{
		return rootTree->FindTree(node);
	}

	return NULL;
}

void QuadTree::GetAllHitCircleNodeIdxs(std::vector<int>& idxs)
{
	_getAllParentNodeIdx(idxs);
	_getAllSelfChildNodeIdx(idxs);	
}

void QuadTree::GetAllHitCircleNodeIdxs(int treeIndex, std::vector<int>& idxs)
{
	if (this->rootTree->mapTrees.find(treeIndex) == this->rootTree->mapTrees.end())
	{
		return;
	}
	QuadTree* tree = this->rootTree->mapTrees[treeIndex];
	tree->GetAllHitCircleNodeIdxs(idxs);
}

void QuadTree::GetAllHitCircleNodeIdxs(const BBRect& rect, std::vector<int>& idxs)
{
	if (subTrees.size() == 0)
	{
		GetAllHitCircleNodeIdxs(idxs);
	}
	else 
	{
		QuadType quadType = GetQuadType(rect);
		if (quadType == QuadType::All)
		{
			GetAllHitCircleNodeIdxs(idxs);
		}
		else
		{
			QuadTree& subTree = subTrees[quadType];
			subTree.GetAllHitCircleNodeIdxs(rect, idxs);
		}
	}
}

unsigned int QuadTree::GetCurrTreeId()
{
	if (isRoot)
	{
		return 1;
	}
	else {
		return 4 * parentTree->treeIdx - 2 + treeType;
	}
}


void QuadTree::_getAllTreeNodeIdxs(const QuadTree* tree, std::vector<int>& idxs)
{
	if (!tree)
	{
		return;
	}
	const QuadNode& node = tree->rootNode;
	for (BaseCircleNode* BaseCircleNode : node.subNodes)
	{
		idxs.emplace_back(BaseCircleNode->Idx);
	}
}

//所有父节点
void QuadTree::_getAllParentNodeIdx(std::vector<int>& idxs)
{
	QuadTree* pTree = parentTree;
	while (pTree)
	{
		_getAllTreeNodeIdxs(pTree, idxs);
		pTree = pTree->parentTree;
	}
}

//当前节点和所有子树节点
void QuadTree::_getAllSelfChildNodeIdx(std::vector<int>& idxs)
{
	_getAllTreeNodeIdxs(this, idxs);
	for (int i = 0; i < this->subTrees.size(); i++)
	{
		this->subTrees[i]._getAllSelfChildNodeIdx(idxs);
	}
}

void QuadTree::_splitTree()
{
	bool success = _buildSubTrees();
	if (success)
	{
		//将这个节点的内容分到子节点
		std::vector<BaseCircleNode*> tempNodes = std::move(rootNode.subNodes);
		rootNode.subNodes.clear();
		for (int i = 0; i < tempNodes.size(); i++)
		{
			BaseCircleNode* node = tempNodes[i];
			AddCircleNode(node);
		}
	}
}

bool QuadTree::_buildSubTrees()
{
	if (depth >= maxDepth)
	{
		return false;
	}
	
	subTrees.resize(QuadType::MAX);
	for (int i = 0; i < QuadType::MAX; i++)
	{
		QuadType type = (QuadType)i;
		QuadTree& tree = subTrees[i];
		tree.SetTreeRect(depth + 1, _getRect(type));
		tree.treeType = type;
		tree.OnBuildFinish(this);
	}
	return true;
}

QuadType QuadTree::GetQuadType(BaseCircleNode* node)
{
	if (subTrees.size() != QuadType::MAX)
	{
		return QuadType::All;
	}
	int centerX = rect.centerX;
	int centerY = rect.centerY;
	BBRect rect = node->rect;
	if (rect.minY > centerY)
	{
		if (rect.minX >= centerX) {
			return QuadType::UR;
		}
		else if (rect.maxX < centerX) {
			return QuadType::UL;
		}
	}
	else if (rect.maxY < centerY) {
		if (rect.maxX <= centerX) {
			return QuadType::BL;
		}
		else if (rect.minX > centerX) {
			return QuadType::BR;
		}
	}
	
	return QuadType::All;

}

QuadType QuadTree::GetQuadType(const BBRect& checkHit)
{
	if (subTrees.size() != QuadType::MAX)
	{
		return QuadType::All;
	}
	int centerX = rect.centerX;
	int centerY = rect.centerY;
	if (checkHit.minY > centerY)
	{
		if (checkHit.minX >= centerX) {
			return QuadType::UR;
		}
		else if (checkHit.maxX < centerX) {
			return QuadType::UL;
		}
	}
	else if (checkHit.maxY < centerY) {
		if (checkHit.maxX <= centerX) {
			return QuadType::BL;
		}
		else if (checkHit.minX > centerX) {
			return QuadType::BR;
		}
	}
	
	return QuadType::All;
}

BBRect QuadTree::_getRect(QuadType type)
{
	BBRect returnRect(rect);
	switch (type)
	{
	case UL:
		returnRect.setRect(rect.minX, rect.centerY, rect.centerX, rect.maxY);
		break;
	case UR:
		returnRect.setRect(rect.centerX, rect.centerY, rect.maxX, rect.maxY);
		break;
	case BL:
		returnRect.setRect(rect.minX, rect.minY, rect.centerX, rect.centerY);
		break;
	case BR:
		returnRect.setRect(rect.centerX, rect.minY, rect.maxX, rect.centerY);
		break;
	default:
		break;
	}
	return returnRect;
}