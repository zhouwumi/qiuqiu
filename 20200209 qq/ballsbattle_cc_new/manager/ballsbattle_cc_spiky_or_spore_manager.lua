BBCCSpikySporeManager = CreateClass()

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local winSize = g_director:getWinSize()

function BBCCSpikySporeManager:__init__(mainPanel)
	self._mainPanel = mainPanel
	self.winW, self.winH = winSize.width, winSize.height
	self.spikyGridMap = {}
	self.spikyGridSize = 450 --食物大格子的w
	self.spikyGridCount = constant_ballsbattle.BG_SIZE[1] / self.spikyGridSize --刺球格子的横竖数量
	for i = 1, self.spikyGridCount do
		local iTable = {}
		self.spikyGridMap[i] = iTable
		for j = 1, self.spikyGridCount do
			table.insert(iTable, {isVisbile = false, objects = {}})
		end
	end
	self.nowRect = {}
end

function BBCCSpikySporeManager:AddSpikyObject(spikyObject)
	local x, y = spikyObject.positionX, spikyObject.positionY
    local minGridX = math.floor((x - spikyObject.radius) / self.spikyGridSize)
    minGridX = math.max(minGridX, 1)
    local minGridY = math.floor((y - spikyObject.radius) / self.spikyGridSize)
    minGridY = math.max(minGridY, 1)

    local maxGridX = math.ceil((x + spikyObject.radius) / self.spikyGridSize)
    maxGridX = math.min(maxGridX, self.spikyGridCount)
    local maxGridY = math.ceil((y + spikyObject.radius) / self.spikyGridSize)
    maxGridY = math.min(maxGridY, self.spikyGridCount)

    spikyObject.minGridX = minGridX
    spikyObject.minGridY = minGridY
    spikyObject.maxGridX = maxGridX
    spikyObject.maxGridY = maxGridY

    for gridX = minGridX, maxGridX do
    	for gridY = minGridY, maxGridY do
    		if not self.spikyGridMap[gridX] or not self.spikyGridMap[gridX][gridY] then
    			print(gridX, gridY)
    			error("invalid gridX, gridY", gridX, gridY)
    		end
    		local map = self.spikyGridMap[gridX][gridY]
    		map.objects[spikyObject.objectIdx] = spikyObject
    		spikyObject.gridMap[map] = true
    	end
    end    

    if self:_isSpikyInVisibleRect(spikyObject) then
    	spikyObject.displayObject:setVisible(true)
    else
    	spikyObject.displayObject:setVisible(false)
    end
end

function BBCCSpikySporeManager:ObjectPositionChange(sporeObject)
	self:RemoveSpikyObject(sporeObject)
	self:AddSpikyObject(sporeObject)
end

function BBCCSpikySporeManager:RemoveSpikyObject(spikyObject)
	local gridMap = spikyObject.gridMap
	if not gridMap then
		return
	end
	local objectIdx = spikyObject.objectIdx
	for map, _ in pairs(gridMap) do
		map.objects[objectIdx] = nil
	end
end

function BBCCSpikySporeManager:_isSpikyInVisibleRect(spikyObject)
	if #self.nowRect > 0 then
		local isXInScope = spikyObject.maxGridX >= self.nowRect[1] and spikyObject.minGridX <= self.nowRect[2]
		local isYInScope = spikyObject.maxGridY >= self.nowRect[3] and spikyObject.minGridY <= self.nowRect[4]
		return isXInScope and isYInScope
	end
	return false
end

function BBCCSpikySporeManager:OnUpdateSpiky(force)
	if not force and self.updateTime and utils_get_tick() - self.updateTime <= 3 then
		return
	end
	self.updateTime = utils_get_tick()

	local minLocalPoint, maxLocalPoint = self._mainPanel.bgComponent:GetVisibleRect()

	local minGridX = math.floor(minLocalPoint.x / self.spikyGridSize) - 1
	minGridX = math.max(minGridX, 1)
	local maxGridX = math.ceil(maxLocalPoint.x  / self.spikyGridSize) + 1
	maxGridX = math.min(maxGridX, self.spikyGridCount)

	local minGridY = math.floor(minLocalPoint.y / self.spikyGridSize) - 1
	minGridY = math.max(minGridY, 1)
	local maxGridY = math.ceil(maxLocalPoint.y / self.spikyGridSize) + 1
	maxGridY = math.min(maxGridY, self.spikyGridCount)

	self.nowRect[1] = minGridX
	self.nowRect[2] = maxGridX
	self.nowRect[3] = minGridY
	self.nowRect[4] = maxGridY

	local removeList, addList
	if not self.oldRect then
		addList  = {}
		for i = minGridX, maxGridX do
			for j = minGridY, maxGridY do
				table.insert(addList,i)
				table.insert(addList,j)
			end
		end
		self.oldRect = {minGridX, maxGridX, minGridY, maxGridY}
	else
		removeList, addList = BBCCUtils.GetChangeIndexTable(self.nowRect, self.oldRect)
		-- print(removeList, addList, self.nowRect, self.oldRect)
		self.oldRect[1] = minGridX
		self.oldRect[2] = maxGridX
		self.oldRect[3] = minGridY
		self.oldRect[4] = maxGridY
	end

	
	local removeMap = {}
	if removeList then
		for i = 1, #removeList, 2 do
			local x, y = removeList[i], removeList[i + 1]
			
			if not self.spikyGridMap[x] or not self.spikyGridMap[x][y] then
				print(x, y)
				-- print("addList  ", addList)
				-- print("removeList  ", removeList)
				print("change rect  ", self.nowRect, self.oldRect)
				print('invalid x, y')
				print(minLocalPoint, maxLocalPoint)
				break
			else
				local gridMap = self.spikyGridMap[x][y]
				gridMap.isVisbile = false
				for index, object in pairs(gridMap.objects) do
					removeMap[object] = true
				end
			end
		end
	end
	
	if addList then
		for i = 1, #addList, 2 do
			local x, y = addList[i], addList[i + 1]
			if not self.spikyGridMap[x] or not self.spikyGridMap[x][y] then
				print(x, y)
				
				-- print("addList  ", addList)
				-- print("removeList  ", removeList)
				print("change rect  ", self.nowRect, self.oldRect)
				print('invalid x, y')
				print(minLocalPoint, maxLocalPoint)
				break
			else
				local gridMap = self.spikyGridMap[x][y]
				gridMap.isVisbile = true
				for _, object in pairs(gridMap.objects) do
					object.displayObject:setVisible(true)
					removeMap[object] = nil
				end
			end
			
		end
	end

	for removeObj, _ in pairs(removeMap) do
		removeObj.displayObject:setVisible(false)
		for map, _ in pairs(removeObj.gridMap) do
			if map.isVisbile then
				removeObj.displayObject:setVisible(true)
				break
			end
		end
	end
end