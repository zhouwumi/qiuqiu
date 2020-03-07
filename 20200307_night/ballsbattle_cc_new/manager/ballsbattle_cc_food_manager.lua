BBCCFoodManager = CreateClass()

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local winSize = g_director:getWinSize()
local ObjectPool = import('logic.dialog.ballsbattle_cc_new.bb_object_pool').ObjectPool
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBCCFoodManager:__init__(mainPanel)
	self._mainPanel = mainPanel
	self.winW, self.winH = winSize.width, winSize.height
	
	self.gridManager = self._mainPanel.gridManager
	self.foodGridSize = 450 --食物大格子的w
	self.foodGridCount = constant_ballsbattle.BG_SIZE[1] / self.foodGridSize --食物格子的横竖数量
	self.objectPool = ObjectPool:New(constant_ballsbattle_cc.BBObjectTypes.Food, self._mainPanel)
	self.objectPool:CreateInitObjectCount(600)
	self:ReEnter()
end

function BBCCFoodManager:ReEnter()
	for _, food in pairs(self.foodMap or {}) do
		food:Remove(nodeIdx)
	end
	self.foodMap = {}
	self.foodGridMap = {}
	for i = 1, self.foodGridCount do
		local iTable = {}
		self.foodGridMap[i] = iTable
		for j = 1, self.foodGridCount do
			table.insert(iTable, {})
		end
	end
end

function BBCCFoodManager:CreateFoodWithServerInfo(foodInfo)
	local posKey = foodInfo
	local x, y = BBCCUtils.UnPackServerLocation(posKey)
	self._mainPanel.gameManager:AddNewFoodFromServer(posKey)
    -- local foodObj = FoodObject:New(self._mainPanel)
    local foodObj = self.objectPool:GetOrCreateObect()
    foodObj:OnJoinGame(x, y, posKey)
    self.foodMap[posKey] = foodObj
    self:_AddFoodToMap(foodObj)
    return foodObj
end

function BBCCFoodManager:_AddFoodToMap(foodObject)
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
    foodObject.gridX = gridX
    foodObject.gridY = gridY
    if self:_isFoodInVisibleRect(foodObject)  then
    	-- foodObject.displayObject:setVisible(true)
    	foodObject:Show()
    else
    	foodObject:Hide()
    	-- foodObject.displayObject:setVisible(false)
    end
end

function BBCCFoodManager:RemoveFoodFromServer(foodIdx, nodeIdx)
	local food = self.foodMap[foodIdx]
	if not food then
		error("RemoveFood no food idx:  "..foodIdx)
		return
	end
	self.foodMap[foodIdx] = nil
	food:Remove(nodeIdx)
end

function BBCCFoodManager:RemoveFoodFromMap(foodObject)
	local map = foodObject.gridMap
	if not map then
		return
	end
	map[foodObject.objectIdx] = nil
end

function BBCCFoodManager:RemoveFromPool(foodObject)
	-- foodObject.displayObject:setVisible(false)
	foodObject:Hide()
	self.objectPool:ReturnObject(foodObject)
end

function BBCCFoodManager:GetFoodObj(idx)
	return self.foodMap[idx]
end

function BBCCFoodManager:SimulateEatFood()
    local playerIds = {g_user_info.get_user_info().uid}
    local results = self._mainPanel.gameManager:SimulateEatFoods(playerIds)

    for index = 1, #results, 2 do
        local nodeIdx = results[index]
        local foodIdx = results[index + 1]
        local food = self.foodMap[foodIdx]
		if not food then
			error("SimulateEatFood no food idx:  "..foodIdx)
			return
		end
		food:SimulateHide(nodeIdx)
    end
end

function BBCCFoodManager:_isFoodInVisibleRect(foodObject)
	local nowRect = self.gridManager.nowRect
	if nowRect and #nowRect > 0 then
		local x, y = foodObject.positionX, foodObject.positionY
		local gridX = math.ceil(x / self.gridManager.gridSize)
		local gridY = math.ceil(y / self.gridManager.gridSize)
		gridX = math.max(gridX, 1)
		gridY = math.max(gridY, 1)
		return gridX <= nowRect[2] and gridX >= nowRect[1] and gridY <= nowRect[4] and gridY >= nowRect[3]
	end
	return false	
end

function BBCCFoodManager:OnUpdateFood(removeList, addList)
	if removeList then
		for i = 1, #removeList, 2 do
			local x, y = removeList[i], removeList[i + 1]
			if not self.foodGridMap[x] or not self.foodGridMap[x][y] then
				print(x, y)
				error('invalid x, y')
			else
				local gridMap = self.foodGridMap[x][y]
				for idx, obj in pairs(gridMap) do
					if not obj.isDestory then
						obj:Hide()
					end
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
			else
				local gridMap = self.foodGridMap[x][y]
				for idx, obj in pairs(gridMap) do
					if not obj.isDestory then
						obj:Show()
					end
				end
			end
		end
	end
end

