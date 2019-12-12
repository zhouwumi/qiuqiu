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
		_ASSERT("invalid tree call UpdateBaseCircleNode");
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

void QuadTree::GetAllHitCircleNodeIdxs(QuadTree* tree, std::vector<int>& idxs)
{
	
	if (tree)
	{
		_getAllTreeNodeIdxs(tree, idxs);
		if (tree->subTrees.size() > 0)
		{
			for (int i = 0; i < tree->subTrees.size(); i++)
			{
				GetAllHitCircleNodeIdxs(&tree->subTrees[i], idxs);
			}
		}
		else
		{
			QuadType treeType = tree->treeType;
			switch (treeType)
			{
			case TOP:
				GetAllHitCircleNodeIdxs(&tree->parentTree->subTrees[QuadType::UL], idxs);
				GetAllHitCircleNodeIdxs(&tree->parentTree->subTrees[QuadType::UR], idxs);
				break;
			case BUTTOM:
				GetAllHitCircleNodeIdxs(&tree->parentTree->subTrees[QuadType::BL], idxs);
				GetAllHitCircleNodeIdxs(&tree->parentTree->subTrees[QuadType::BR], idxs);
				break;
			case LEFT:
				GetAllHitCircleNodeIdxs(&tree->parentTree->subTrees[QuadType::UL], idxs);
				GetAllHitCircleNodeIdxs(&tree->parentTree->subTrees[QuadType::BL], idxs);
				break;
			case RIGHT:
				GetAllHitCircleNodeIdxs(&tree->parentTree->subTrees[QuadType::UR], idxs);
				GetAllHitCircleNodeIdxs(&tree->parentTree->subTrees[QuadType::BR], idxs);
				break;
			default:
				break;
			}
		}
	}
}

void QuadTree::GetAllHitCircleNodeIdxs(int treeIndex, std::vector<int>& idxs)
{
	if (this->rootTree->mapTrees.find(treeIndex) == this->rootTree->mapTrees.end())
	{
		return;
	}
	QuadTree* tree = this->rootTree->mapTrees[treeIndex];
	GetAllHitCircleNodeIdxs(tree, idxs);
}

void QuadTree::GetAllHitCircleNodeIdxs(const BBRect& rect, std::vector<int>& idxs)
{
	if (subTrees.size() == 0)
	{
		GetAllHitCircleNodeIdxs(treeIdx, idxs);
	}
	else {
		QuadType quadType = GetQuadType(rect);
		if (quadType == QuadType::All)
		{
			GetAllHitCircleNodeIdxs(treeIdx, idxs);
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
		return 8 * parentTree->treeIdx - 6 + treeType;
	}
}


void QuadTree::_getAllTreeNodeIdxs(const QuadTree* tree, std::vector<int>& idxs)
{
	if (!tree)
	{
		return;
	}
	const QuadNode& node = tree->rootNode;
	for each (BaseCircleNode* BaseCircleNode in node.subNodes)
	{
		idxs.emplace_back(BaseCircleNode->Idx);
	}
}

void QuadTree::_splitTree()
{
	_buildSubTrees();
	//将这个节点的内容分到子节点
	std::vector<BaseCircleNode*> tempNodes = std::move(rootNode.subNodes);
	rootNode.subNodes.clear();
	for (int i = 0; i < tempNodes.size(); i++)
	{
		BaseCircleNode* node = tempNodes[i];
		AddCircleNode(node);
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
	if (node->minY > centerY)
	{
		if (node->minX >= centerX) {
			return QuadType::UR;
		}
		else if (node->maxX < centerX) {
			return QuadType::UL;
		}
		else {
			return QuadType::TOP;
		}
	}
	else if (node->maxY < centerY) {
		if (node->maxX <= centerX) {
			return QuadType::BL;
		}
		else if (node->minX > centerX) {
			return QuadType::BR;
		}
		else {
			return QuadType::BUTTOM;
		}
	}
	else
	{
		if (node->maxX <= centerX) {
			return QuadType::LEFT;
		}
		else if (node->minX >= centerX) {
			return QuadType::RIGHT;
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
		else {
			return QuadType::TOP;
		}
	}
	else if (checkHit.maxY < centerY) {
		if (checkHit.maxX <= centerX) {
			return QuadType::BL;
		}
		else if (checkHit.minX > centerX) {
			return QuadType::BR;
		}
		else {
			return QuadType::BUTTOM;
		}
	}
	else
	{
		if (checkHit.maxX <= centerX) {
			return QuadType::LEFT;
		}
		else if (checkHit.minX >= centerX) {
			return QuadType::RIGHT;
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
	case TOP:
		returnRect.setRect(rect.minX, rect.centerY, rect.maxX, rect.maxY);
		break;
	case BUTTOM:
		returnRect.setRect(rect.minX, rect.minY, rect.maxX, rect.centerY);
		break;
	case LEFT:
		returnRect.setRect(rect.minX, rect.minY, rect.centerX, rect.maxY);
		break;
	case RIGHT:
		returnRect.setRect(rect.centerX, rect.minY, rect.maxX, rect.maxY);
		break;
	default:
		break;
	}
	return returnRect;
}