local BBGameQuadNode = CreateClass()

local BBGameRect = import('logic.dialog.ballsbattle_cc_new.game.base.bb_game_rect').BBGameRect

local QuadType = {
	All = 0,
	UL = 1,-- up-left
	UR = 2,--up-right
	BL = 3,--buttom-left
	BR = 4,--buttom_right
	MAX = 4,
}

function BBGameQuadNode:__init__(minX, minY, maxX, maxY)
	self.rect = BBGameRect:New(minX, minY, maxX, maxY)
	self.subNodes = {}
end

function BBGameQuadNode:OnAttachTree(tree)
	self.tree = tree
end

function BBGameQuadNode:InsertCircleNode(node)
	self.subNodes[node] = true
	node:ChangeTreeIndex(self.tree.treeIdx)
end

function BBGameQuadNode:RemoveCircleNode(node)
	self.subNodes[node] = nil
	node:OnRemoveFromTreeNode()
end

BBGameQuadTree = CreateClass()

function BBGameQuadTree:__init__(w, h)
	if not w then w = 0 end
	if not h then h = 0 end
	self.rect = BBGameRect:New(0, 0, w, h)
	self.rootNode = BBGameQuadNode:New(0, 0, w, h)
	self.subTrees = {}
	self.mapTrees = {}
	self.rootTree = self
	self.maxCircleInQuad = 20
	self.maxDepth = 5
	self.depth = 1
	self.treeType = QuadType.All
	self.isRoot = true
end

function BBGameQuadTree:SetTreeRect(depth, rect)
	self.depth = depth
	self.rect:Copy(rect)
	self.rootNode.rect:Copy(rect)
end

function BBGameQuadTree:SetCustomMaxNodeInQuad(max)
	self.maxCircleInQuad = max
end

function BBGameQuadTree:SetCustomMaxDepth(maxDepth)
	self.maxDepth = maxDepth
end

function BBGameQuadTree:PreBuildAllSubTrees()
	self:_buildSubTrees()
	for _, subTree in ipairs(self.subTrees or {}) do
		subTree:PreBuildAllSubTrees()
	end
end

function BBGameQuadTree:OnBuildFinish(parentTree)
	if parentTree and parentTree ~= self then
		self.isRoot = false
		self.parentTree = parentTree
		self.rootTree = parentTree.rootTree
		self:SetCustomMaxNodeInQuad(parentTree.maxCircleInQuad)
		self:SetCustomMaxDepth(parentTree.maxDepth)
	else
		self.isRoot = true
	end
	self.treeIdx = self:GetCurrTreeId()
	self.rootNode:OnAttachTree(self)
	self.rootTree.mapTrees[self.treeIdx] = self
end

function BBGameQuadTree:AddCircleNode(node)
	if #self.subTrees == 0 then
		self.rootNode:InsertCircleNode(node)
		if table.count(self.rootNode.subNodes) > self.maxCircleInQuad then
			self:_splitTree()
		end
	else
		local quadType = self:GetQuadType(node)
		if quadType == QuadType.All then
			self.rootNode:InsertCircleNode(node)
		else
			local subTree = self.subTrees[quadType]
			subTree:AddCircleNode(node)
		end
	end
end

function BBGameQuadTree:RemoveCircleNode(node)
	if not node then
		return
	end
	local quadTree = self:FindTree(node)
	if quadTree then
		quadTree.rootNode:RemoveCircleNode(node)
	end
end

function BBGameQuadTree:UpdateCircleNode(node)
	if not self.isRoot then
		return
	end
	self:RemoveCircleNode(node)
	self:AddCircleNode(node)
end

function BBGameQuadTree:FindTree(node)
	if not node then
		return
	end
	local treeIdx = node.treeIdx
	if self.isRoot then
		return self.mapTrees[treeIdx]
	end
	if self.rootTree then
		return self.rootTree:FindTree(node)
	end
end

function BBGameQuadTree:_getAllHitCircleNode(idxs)
	self:_getAllParentNodeIdx(idxs)
	self:_getAllSelfChildNodeIdx(idxs)
end

function BBGameQuadTree:GetAllHitCircleNodeIdxs(treeIdx, idxs)
	local subTree = self.rootTree.mapTrees[treeIdx]
	if not subTree  then
		return
	end
	return subTree:_getAllHitCircleNode(idxs)
end

function BBGameQuadTree:GetAllHitCircleNodeIdxsByRect(rect, idxs)
	if #self.subTrees == 0 then
		self:_getAllHitCircleNode(idxs)
	else
		local quadType = self:GetQuadTypeByRect(rect)
		if quadType == QuadType.All then
			self:_getAllHitCircleNode(idxs)
		else
			local quadTree = self.subTrees[quadType]
			quadTree:GetAllHitCircleNodeIdxsByRect(rect, idxs)
		end
	end
end

function BBGameQuadTree:GetCurrTreeId()
	if self.isRoot then
		return 1
	end
	return 4 * self.parentTree.treeIdx - 2 + self.treeType
end

function BBGameQuadTree:_getAllTreeNodeIdxs(idxs)
	local node = self.rootNode
	for _, subNode in pairs(node.subNodes or {}) do
		table.insert(idxs, subNode.idx)
	end
end

function BBGameQuadTree:_getAllParentNodeIdx(idxs)
	local parentIter = self.parentTree
	while parentIter do
		parentIter:_getAllTreeNodeIdxs(idxs)
		parentIter = parentIter.parentTree
	end
end

function BBGameQuadTree:_getAllSelfChildNodeIdx(idxs)
	self:_getAllTreeNodeIdxs(idxs)
	for _, subTree in ipairs(self.subTrees or {}) do
		subTree:_getAllSelfChildNodeIdx(idxs)
	end
end

function BBGameQuadTree:_splitTree()
	local success = self:_buildSubTrees()
	if success then
		local tempNodes = self.rootNode.subNodes
		self.rootNode.subNodes = {}
		for node, _ in pairs(tempNodes or {}) do
			self:AddCircleNode(node)
		end
	end
end

function BBGameQuadTree:_buildSubTrees()
	if self.depth >= self.maxDepth then
		return false
	end
	self.subTrees = {}
	for index = 1, QuadType.MAX do
		local type = index
		local subTree = BBGameQuadTree:New()
		self.subTrees[type] = subTree
		subTree:SetTreeRect(self.depth + 1, self:_getRect(type))
		subTree.treeType = type
		subTree:OnBuildFinish(self)
	end
	return true
end

function BBGameQuadTree:GetQuadType(node)
	if not self.subTrees or #self.subTrees ~= QuadType.MAX then
		return
	end
	return self:GetQuadTypeByRect(node.rect)
end

function BBGameQuadTree:GetQuadTypeByRect(rect)
	local centerX = self.rect.centerX
	local centerY = self.rect.centerY
	if rect.minY > centerY then
		if rect.minX >= centerX then
			return QuadType.UR
		elseif rect.maxX < centerX then
			return QuadType.UL
		end
	elseif rect.maxY < centerY then
		if rect.maxX <= centerX then
			return QuadType.BL
		elseif rect.minX > centerX then
			return QuadType.BR
		end
	end
	return QuadType.All
end

function BBGameQuadTree:_getRect(type)
	local rect = BBGameRect:New()
	if type == QuadType.UL then
		rect:SetRect(self.rect.minX, self.rect.centerY, self.rect.centerX, self.rect.maxY)
	elseif type == QuadType.UR then
		rect:SetRect(self.rect.centerX, self.rect.centerY, self.rect.maxX, self.rect.maxY);
	elseif type == QuadType.BL then
		rect:SetRect(self.rect.minX, self.rect.minY, self.rect.centerX, self.rect.centerY);
	elseif type == QuadType.BR then
		rect:SetRect(self.rect.centerX, self.rect.minY, self.rect.maxX, self.rect.centerY);
	end
	return rect
end