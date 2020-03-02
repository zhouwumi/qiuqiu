local GridNode = CreateClass()

function GridNode:__init__()
	self.minX = 0
	self.minY = 0
	self.maxX = 0
	self.maxY = 0
	self.mapIdx2Obj = {}
end

GridMap = CreateClass()

function GridMap:__init__(w, h, depth)
	self.gridNodes = {}
	self.mapAllObjs = {}
	self:SetMap(w, h, depth)
end

function GridMap:SetMap(w, h, depth)
	self.mapW = w
	self.mapH = h
	local totalSize = math.pow(2, 2 * depth - 2)
	local sideSizeLen = math.pow(2, depth - 1)
	local sideLen = w / sideSizeLen
	for i = 1, sideSizeLen do
		for j = 1, sideSizeLen do
			local gridIdx = (j - 1) * sideSizeLen + i
			local gridNode = GridNode:New()
			gridNode.minX = (i - 1) * sideLen
			gridNode.maxX = i * sideLen
			gridNode.minY = (j - 1) * sideLen
			gridNode.minY = j * sideLen
			self.gridNodes[gridIdx] = gridNode
		end
	end
	self.sideLen = sideLen
	self.sideSizeLen = sideSizeLen
	print("GridMap ", depth, self.sideLen, self.sideSizeLen)
end

--node:BBGameBaseObject
function GridMap:AddObjectMapKey(node, x, y)
	local gridX = math.ceil(x / self.sideLen)
	local gridY = math.ceil(y / self.sideLen)
	local gridIdx = (gridY - 1) * self.sideSizeLen + gridX
	local gridNode = self.gridNodes[gridIdx]
	if not gridNode then
		error("invalid gridIdx: "..gridIdx)
		return
	end
	gridNode.mapIdx2Obj[node.idx] = node
	self.mapAllObjs[node] = gridIdx
end

function GridMap:RemoveObject(node)
	local gridIdx = self.mapAllObjs[node]
	if not gridIdx then
		return
	end
	self.mapAllObjs[node] = nil
	gridNode = self.gridNodes[gridIdx]
	gridNode.mapIdx2Obj[node.idx] = nil
end

function GridMap:GetAllHitIdxs(rect)
	local idxs = {}
	local minGridX = math.ceil(rect.minX / self.sideLen)
	local minGridY = math.ceil(rect.minY / self.sideLen)
	local maxGridX = math.ceil(rect.maxX / self.sideLen)
	local maxGridY = math.ceil(rect.maxY / self.sideLen)
	for i = minGridX, maxGridX do
		for j = minGridY, maxGridY do
			local gridIdx = (j - 1) * self.sideSizeLen + i
			local gridNode = self.gridNodes[gridIdx]
			if gridNode then
				for idx, node in pairs(gridNode.mapIdx2Obj or {}) do
					table.insert(idxs, idx)
				end
			end
		end
	end
	return idxs
end