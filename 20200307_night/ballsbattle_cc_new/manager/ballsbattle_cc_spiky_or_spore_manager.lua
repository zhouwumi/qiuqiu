BBCCSpikySporeManager = CreateClass()

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local winSize = g_director:getWinSize()
local ObjectPool = import('logic.dialog.ballsbattle_cc_new.bb_object_pool').ObjectPool
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBCCSpikySporeManager:__init__(mainPanel)
	self._mainPanel = mainPanel
	self.winW, self.winH = winSize.width, winSize.height
	self.gridManager = self._mainPanel.gridManager
	self:ReEnter()

	self.sporeObjectPool = ObjectPool:New(constant_ballsbattle_cc.BBObjectTypes.Spore, self._mainPanel)
	self.sporeObjectPool:CreateInitObjectCount(20)

	self.spikyObjectPool = ObjectPool:New(constant_ballsbattle_cc.BBObjectTypes.Spiky, self._mainPanel)
	self.spikyObjectPool:CreateInitObjectCount(30)
	self.nextSporeZOrder = 0
end

function BBCCSpikySporeManager:ReEnter()
	self.spikyGridMap = {}
	local gridCount = self.gridManager.gridCount
	for i = 1, gridCount do
		local iTable = {}
		self.spikyGridMap[i] = iTable
		for j = 1, gridCount do
			table.insert(iTable, {isVisbile = false, objects = {}})
		end
	end
	for _, spikyObject in pairs(self.spikyMap or {}) do
		spikyObject:Remove()
	end

	for _, sporeObject in pairs(self.sporeMap or {}) do
		sporeObject:Remove()
	end
	self.spikyMap = {}
	self.sporeMap = {}
	self.showingSporeCnt = 0
	self.movingMap = {}
	self.cacheVisibleSpore = {}
	self.lastSimulateSporeInfos = {}
	self.updateTime = nil
end

function BBCCSpikySporeManager:CacheSporeVisible(idx, visible)
	if visible then
		-- print("show idx ", idx)
		self.cacheVisibleSpore[idx] = true
		-- __G__TRACKBACK__('dddd')
	else
		-- print("hide idx ", idx)
		self.cacheVisibleSpore[idx] = nil
	end
end

function BBCCSpikySporeManager:CreateSpikyFromServer(spikyInfo)
	local x = spikyInfo.x
	local y = spikyInfo.y
	local mass = spikyInfo.mass
	local idx = spikyInfo.idx
	-- local spikyObject = SpikyObject:New(self._mainPanel)
	local spikyObject = self.spikyObjectPool:GetOrCreateObect()
    spikyObject:OnJoinGame(x, y, mass, idx)
    self.spikyMap[idx] = spikyObject
    self:AddSpikySporeObject(spikyObject)
end

function BBCCSpikySporeManager:RemoveSpikyFromServer(spikyIdx)
	local spiky = self.spikyMap[spikyIdx]
	if not spiky then
		error("RemoveSpiky no spiky idx: "..spikyIdx)
		return
	end
	self.spikyMap[spikyIdx] = nil
	spiky:Remove()
end

function BBCCSpikySporeManager:CreateSporeFromServer(idx, fromId, cd)
	-- local sporeObject = SporeObject:New(self._mainPanel)
	self.nextSporeZOrder = self.nextSporeZOrder + 1
	local sporeObject = self.sporeObjectPool:GetOrCreateObect()
	sporeObject.fromId = fromId
	sporeObject.cd = cd
	sporeObject:OnJoinGame(idx)
	self.sporeMap[idx] = sporeObject
	self.movingMap[idx] = sporeObject
	self:AddSpikySporeObject(sporeObject)
	return sporeObject
end

function BBCCSpikySporeManager:UpdateSpore()
	for idx, sporeObject in pairs(self.movingMap) do
		sporeObject:Update()
		if sporeObject.isMoveEnd then
			self.movingMap[idx] = nil
		end
	end
	self.hasSporeInVisible = false
	for _, _ in pairs(self.cacheVisibleSpore) do
		self.hasSporeInVisible = true
		break
	end
	-- local cnt = table.count(self.cacheVisibleSpore)
	-- local cnt = 0
	-- for idx, sporeObject in pairs(self.sporeMap) do
	-- 	if sporeObject.displayObject:isVisible() then
	-- 		cnt = cnt + 1
	-- 	end
	-- end
	-- self._mainPanel.labelShowSporeCount:SetString("当前孢子的数量:  "..cnt)
end

function BBCCSpikySporeManager:RemoveSpore(sporeIdx)
	local spore = self.sporeMap[sporeIdx]
	if not spore then
		error("RemoveSpore no spore idx: "..sporeIdx)
		return
	end
	self.sporeMap[sporeIdx] = nil
	self.movingMap[sporeIdx] = nil
	spore:Remove()
end

function BBCCSpikySporeManager:RemoveSporeFromPool(object)
	self.sporeObjectPool:ReturnObject(object)
end

function BBCCSpikySporeManager:SimulateEatSpore(sporeIdx)
	local spore = self.sporeMap[sporeIdx]
	if not spore then
		error("RemoveSpore no spore idx: "..sporeIdx)
		return
	end
	-- spore.displayObject:setVisible(false)
	spore:Hide()
end

function BBCCSpikySporeManager:GetSpore(sporeIdx)
	return self.sporeMap[sporeIdx]
end

function BBCCSpikySporeManager:AddSpikySporeObject(object)
	local gridSize = self.gridManager.gridSize
	local x, y = object:GetDisplayPosition()
	local minGridX, minGridY, maxGridX, maxGridY

	if object.isSpore then
		minGridX = math.ceil(x / gridSize)
		maxGridX = minGridX
		minGridY = math.ceil(y / gridSize)
		maxGridY = minGridY
	else
		minGridX = math.ceil((x - object.radius) / gridSize)
		minGridY = math.ceil((y - object.radius) / gridSize)
		maxGridX = math.ceil((x + object.radius) / gridSize)
		maxGridY = math.ceil((y + object.radius) / gridSize)
		
	end
	local gridCount = self.gridManager.gridCount
	minGridX = math.max(minGridX, 1)
	minGridY = math.max(minGridY, 1)
	maxGridX = math.min(maxGridX, gridCount)
	maxGridY = math.min(maxGridY, gridCount)

    object.minGridX = minGridX
    object.minGridY = minGridY
    object.maxGridX = maxGridX
    object.maxGridY = maxGridY

    -- print("计算范围 ", minGridX, minGridY, maxGridX, maxGridY, object.radius)
    for gridX = minGridX, maxGridX do
    	for gridY = minGridY, maxGridY do
    		if not self.spikyGridMap[gridX] or not self.spikyGridMap[gridX][gridY] then
    			print(gridX, gridY)
    			error("invalid gridX, gridY", gridX, gridY)
    		end
    		local map = self.spikyGridMap[gridX][gridY]
    		map.objects[object.objectIdx] = object
    		object.gridMap[map] = true
    		object.gridX = gridX
    		object.gridY = gridY
    		if object.isSpore then
    			-- print("孢子所在的格子  ", object.objectIdx, gridX, gridY, x, y)
    		end
    	end
    end

    if self:_isObjInVisibleRect(object) then
    	object:Show()
    else
    	object:Hide()
    end
end

function BBCCSpikySporeManager:ObjectPositionChange(object)
	self:RemoveSpikySporeObject(object)
	self:AddSpikySporeObject(object)
end

function BBCCSpikySporeManager:RemoveSpikySporeObject(object)
	local gridMap = object.gridMap
	if not gridMap then
		return
	end
	local objectIdx = object.objectIdx
	for map, _ in pairs(gridMap) do
		map.objects[objectIdx] = nil
	end
	object.gridMap = {}
end

function BBCCSpikySporeManager:RemoveSpikyFromPool(object)
	self.spikyObjectPool:ReturnObject(object)
end


function BBCCSpikySporeManager:SimulateEatSpores()
	if not self.cacheVisibleSpore then
		return
	end
    if not self.lastSimulateSporeInfos then
        self.lastSimulateSporeInfos = {}
    end
    local playerIds = {g_user_info.get_user_info().uid}
    local results = self._mainPanel.gameManager:SimulateEatSpores(playerIds)
    -- local curDelayFrame = self.gameManager:GetDelayFrame()

    for index = 1, #results, 2 do
        local nodeId = results[index]
        local sporeId = results[index + 1]
        local lastTime = self.lastSimulateSporeInfos[sporeId]
        if not lastTime then
            lastTime = self.frame
            self.lastSimulateSporeInfos[sporeId] = lastTime
        else
            if self.frame - lastTime > 2 then
                -- print("模拟吃bao子结果：  ", sporeId, nodeId)
                self:SimulateEatSpore(sporeId)
            end
        end
    end
end

function BBCCSpikySporeManager:_isObjInVisibleRect(object)
	local nowRect = self.gridManager.nowRect
	if nowRect and #nowRect > 0 then
		local isXInScope = object.maxGridX >= nowRect[1] and object.minGridX <= nowRect[2]
		local isYInScope = object.maxGridY >= nowRect[3] and object.minGridY <= nowRect[4]
		return isXInScope and isYInScope
	end
	return false
end

function BBCCSpikySporeManager:OnUpdateVisible(removeList, addList)
	local removeMap = {}
	if removeList then
		for i = 1, #removeList, 2 do
			local x, y = removeList[i], removeList[i + 1]
			
			if not self.spikyGridMap[x] or not self.spikyGridMap[x][y] then
				print(x, y)
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
				
				print('invalid x, y')
				print(minLocalPoint, maxLocalPoint)
				break
			else
				local gridMap = self.spikyGridMap[x][y]
				gridMap.isVisbile = true
				for _, object in pairs(gridMap.objects) do
					-- object.displayObject:setVisible(true)
					object:Show()
					removeMap[object] = nil
				end
			end
		end
	end

	for removeObj, _ in pairs(removeMap) do
		local isShow = false
		
		for map, _ in pairs(removeObj.gridMap) do
			if map.isVisbile then
				isShow = true
				break
			end
		end
		if isShow then
			removeObj:Show()
		else
			removeObj:Hide()
		end
	end
end