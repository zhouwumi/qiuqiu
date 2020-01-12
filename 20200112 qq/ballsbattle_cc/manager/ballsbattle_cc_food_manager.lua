BBCCFoodManager = CreateClass()

local BBCCUtils = import("logic.dialog.ballsbattle_cc.ballsbattle_cc_utils").BBCCUtils
local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local winSize = g_director:getWinSize()

function BBCCFoodManager:__init__(mainPanel)
	self._mainPanel = mainPanel
	self.winW, self.winH = winSize.width, winSize.height
	self.foodGridMap = {}
	self.foodGridSize = 100 --食物大格子的w
	self.foodGridCount = constant_ballsbattle.BG_SIZE[1] / self.foodGridSize --食物格子的横竖数量
	for i = 1, self.foodGridCount do
		local iTable = {}
		self.foodGridMap[i] = iTable
		for j = 1, self.foodGridCount do
			table.insert(iTable, {})
		end
	end
	self.nowRect = {}
end

function BBCCFoodManager:AddFood(foodObject)
    local x, y = foodObject.positionX, foodObject.positionY
    local gridX = math.ceil(x / self.foodGridSize)
    local gridY = math.ceil(y / self.foodGridSize)
    gridX = math.max(gridX, 1)
    gridY = math.max(gridY, 1)
    if not self.foodGridMap[gridX] or not self.foodGridMap[gridX][gridY] then
    	print(gridX, gridY)
    	error("invalid gridX, gridY", gridX, gridY)
    end
    local map = self.foodGridMap[gridX][gridY]
    map[foodObject.objectIdx] = foodObject
    foodObject.gridMap = map
    if self:_isFoodInVisibleRect(foodObject)  then
    	foodObject.displayObject:setVisible(true)
    end
end

function BBCCFoodManager:RemoveFood(foodObject)
	local map = foodObject.gridMap
	if not map then
		return
	end
	map[foodObject.objectIdx] = nil
end

function BBCCFoodManager:_isFoodInVisibleRect(foodObject)
	if #self.nowRect > 0 then
		local x, y = foodObject.positionX, foodObject.positionY
		local gridX = math.ceil(x / self.foodGridSize)
		local gridY = math.ceil(y / self.foodGridSize)
		gridX = math.max(gridX, 1)
		gridY = math.max(gridY, 1)
		return gridX <= self.nowRect[2] and gridX >= self.nowRect[1] and gridY <= self.nowRect[4] and gridY >= self.nowRect[3]
	end
	return false	
end

function BBCCFoodManager:OnUpdateFood(force)
	if not force and self.updateTime and utils_get_tick() - self.updateTime <= 2 then
		return
	end
	self.updateTime = utils_get_tick()
	
	local minLocalPoint, maxLocalPoint = self._mainPanel.bgComponent:GetVisibleRect()
	local minGridX = math.floor(minLocalPoint.x / self.foodGridSize) - 1
	minGridX = math.max(minGridX, 1)
	local maxGridX = math.ceil(maxLocalPoint.x  / self.foodGridSize) + 1
	maxGridX = math.min(maxGridX, self.foodGridCount)

	local minGridY = math.floor(minLocalPoint.y / self.foodGridSize) - 1
	minGridY = math.max(minGridY, 1)
	local maxGridY = math.ceil(maxLocalPoint.y / self.foodGridSize) + 1
	maxGridY = math.min(maxGridY, self.foodGridCount)

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
		removeList, addList = BBCCUtils.GetChangeIndexTable(self.nowRect, self.oldRect, self.foodGridSize)
		self.oldRect[1] = minGridX
		self.oldRect[2] = maxGridX
		self.oldRect[3] = minGridY
		self.oldRect[4] = maxGridY
	end

	if removeList then
		for i = 1, #removeList, 2 do
			local x, y = removeList[i], removeList[i + 1]
			local gridMap = self.foodGridMap[x][y]
			for idx, obj in pairs(gridMap) do
				if not obj.isDestory then
					obj.displayObject:setVisible(false)
				end
			end
		end
	end
	
	if addList then
		for i = 1, #addList, 2 do
			local x, y = addList[i], addList[i + 1]
			if not self.foodGridMap[x] or not self.foodGridMap[x][y] then
				print(x, y)
				error('invalid x, y')
			end
			local gridMap = self.foodGridMap[x][y]
			for idx, obj in pairs(gridMap) do
				if not obj.isDestory then
					obj.displayObject:setVisible(true)
				end
			end
		end
	end
end

