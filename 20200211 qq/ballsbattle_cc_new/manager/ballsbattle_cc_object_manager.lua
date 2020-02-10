BBCCObjectManager = CreateClass()

local FoodObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_food_object').FoodObject
local SpikyObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_spiky_object').SpikyObject
local SporeObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_spore_object').SporeObject

local PlayerNodeObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_player_object_node').PlayerNodeObject
local PlayerObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_player_object').PlayerObject

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils

local Type_PlayerNode = 1
local Type_Food = 2
local Type_Spiky = 3
local Type_Spore = 4


function BBCCObjectManager:__init__(mainPanel)
	self._mainPanel = mainPanel
	self._gameManager = self._mainPanel.gameManager
	self._foodManager = self._mainPanel.foodManager
	self._spikySporeManager = self._mainPanel.spikySporeManager
	self.hasGenerateFood = false

	self.foodMap = {}
	self.spikyMap = {}
	self.sporeMap = {}
end

function BBCCObjectManager:CreateFoodWithServerInfo(foodInfo)
	local posKey = foodInfo
	local x, y = BBCCUtils.UnPackServerLocation(posKey)
	self._gameManager:AddNewFoodFromServer(posKey)
    local foodObj = FoodObject:New(self._mainPanel)
    foodObj:SetPosition(x, y, posKey)
    self.foodMap[posKey] = foodObj
    self._foodManager:AddFood(foodObj)
    return foodObj
end

function BBCCObjectManager:CreateNodeWithServerInfo(nodeInfo)
	local uid = nodeInfo.uid
	local playerObject = self._mainPanel.playerManager.allPlayers[uid]
	if not playerObject then
		return 
	end

	local current = nodeInfo.current
	local direction = nodeInfo.direction
	local fromId = nodeInfo.from_id or 0
	local idx = nodeInfo.idx
	local init = nodeInfo.init or 0
	local location = nodeInfo.location or 0
	local mass = nodeInfo.mass
	
	local x, y = BBCCUtils.UnPackServerLocation(location)
	local curSpeed = nodeInfo.init_speed or 0
	local deltaSpeed = nodeInfo.init_delta_speed or 0
	local cd = nodeInfo.cd or 0
	local protect = nodeInfo.protect or 0
	self._gameManager:AddNewBallFromServer(idx, fromId, uid, mass, x, y, direction[1], direction[2], current[1], current[2], curSpeed, deltaSpeed, init, cd, protect)
	local playerNode = PlayerNodeObject:New(self._mainPanel, idx)
	playerNode:SetBirthInfo(x, y, mass)
	playerObject:AddPlayerNode(playerNode)
	return playerNode
end

-- function BBCCObjectManager:GenerateNewSporeWithServerInfo(sporeInfo)
-- 	local cd = sporeInfo.cd or 0
-- 	local current = sporeInfo.current
-- 	local direction = sporeInfo.direction
-- 	local fromId = sporeInfo.from_id
-- 	local idx = sporeInfo.idx
-- 	local init = sporeInfo.init
-- 	local init_delta_speed = sporeInfo.init_delta_speed
-- 	local init_speed = sporeInfo.init_speed
-- 	local location = sporeInfo.location
-- 	local mass = sporeInfo.mass
-- 	local uid = sporeInfo.uid
-- 	local x, y = BBCCUtils.UnPackServerLocation(location)
-- 	self._gameManager:AddNewSporeFromServer(idx, fromId, uid, mass, x, y, direction[1], direction[2], current[1], current[2], init_speed, init_delta_speed, init, cd)

-- 	local sporeObject = SporeObject:New(self._mainPanel)
-- 	local testX, testY, radius = self._gameManager:GetSporeInfo(idx)
-- 	sporeObject:SetPosition(x, y, radius, idx)
-- 	self.sporeMap[idx] = sporeObject
-- 	self._spikySporeManager:AddSpikyObject(sporeObject)
-- end

function BBCCObjectManager:GenerateSpore(idx, x, y)
	local sporeObject = SporeObject:New(self._mainPanel)
	sporeObject:SetBirthInfo(x, y, idx)
	self.sporeMap[idx] = sporeObject
	self._spikySporeManager:AddSpikyObject(sporeObject)
end

function BBCCObjectManager:CreateSpikyWithServerInfo(spikyInfo)
	local idx = spikyInfo.idx
	local location = spikyInfo.location
	local serverX, serverY = BBCCUtils.UnPackServerLocation(location)
	local mass = spikyInfo.mass
	self._gameManager:AddNewSpikyFromServer(idx, location, mass)
	local x, y, radius = self._gameManager:GetSpikyInfo(idx)

	assert(serverX == x and serverY == y)
    local spikyObject = SpikyObject:New(self._mainPanel)
    spikyObject:SetPosition(x, y, radius, idx)
    self.spikyMap[idx] = spikyObject
    self._spikySporeManager:AddSpikyObject(spikyObject)
end

function BBCCObjectManager:CreatePlayerWithServerInfo(playerInfo, getNewNodeIdxs)
	local uid = playerInfo.uid
	local stopped = playerInfo.stopped
	local nmass = playerInfo.nmass or 0
	local direction = playerInfo.direction
	local final_point = playerInfo.final_point
	local isBalance = playerInfo.is_balance == 1
	local finalX, finalY = BBCCUtils.UnPackServerLocation(final_point)
	self._gameManager:CreatePlayerFromServer(uid, direction[1], direction[2], finalX, finalY, stopped, nmass)
	local playerObject = PlayerObject:New(self._mainPanel, uid)
    self._mainPanel.playerManager.allPlayers[uid] = playerObject
    if uid == self._mainPanel.currentPlayerIdx then
    	self._mainPanel.foceUpdate = true
    end
	local nodes = playerInfo.nodes or {}
	for _, node in ipairs(nodes) do
		self:CreateNodeWithServerInfo(node)
		if getNewNodeIdxs then
			getNewNodeIdxs[node.idx] = true
		end
    end
    self._gameManager:SetPlayerProperty(uid, isBalance)
end

function BBCCObjectManager:UpdateSpore()
	for idx, sporeObject in pairs(self.sporeMap) do
		sporeObject:Update()
	end
end

function BBCCObjectManager:OnGetFirstState(data)
	print("收到服务器的信息  ", data)
	local allFoods = data.foods
	local allSpikys = data.spiky
	local players = data.players

	for _, food in ipairs(allFoods) do
		self:CreateFoodWithServerInfo(food)
	end

	for _, spiky in ipairs(allSpikys) do
		self:CreateSpikyWithServerInfo(spiky)
	end

	for _, player in ipairs(players) do
		self:CreatePlayerWithServerInfo(player)
	end
end

function BBCCObjectManager:GenerateFoodAndSpiky(foodInfos, spikyInfos)
	for _, value  in ipairs(foodInfos or {}) do
		self:CreateFoodWithServerInfo(value)
	end

	self.spikyMap = {}
	for _, spikyInfo in ipairs(spikyInfos or {}) do
	    local idx, x, y, mass, radius = unpack(spikyInfo)
	    local spikyObject = SpikyObject:New(self._mainPanel)
	    spikyObject:SetPosition(x, y, mass, idx)
	    self.spikyMap[idx] = spikyObject
	    self._spikySporeManager:AddSpikyObject(spikyObject)
	end
	self.hasGenerateFood = true
end



function BBCCObjectManager:KeyFrameDoEat()
	local results = self._gameManager:GetFrameEatResult()
	for i = 1, #results, 2 do
	    local removeObjType, removeObjIdx = results[i],results[i+1]
	    self:OnEatObject(removeObjType, removeObjIdx)
	end
end

function BBCCObjectManager:UpdateSporeObjects()
	for sporeIdx, sporeObject in pairs(self.sporeMap) do
		local x, y, radius = self._gameManager:GetSporeInfo(sporeIdx)
		sporeObject:SetPosition(x, y, radius, sporeIdx)
		self._spikySporeManager:ObjectPositionChange(sporeObject)
	end
end

function BBCCObjectManager:KeyFrameCreateNewSpores()
	local allNewSporeIdxs = self._gameManager:GetFrameNewSpore()
	if #allNewSporeIdxs > 0 then
		print("新的孢子", allNewSporeIdxs)
		for _, idx in ipairs(allNewSporeIdxs) do
			local x, y, radius = self._gameManager:GetSporeInfo(idx)
			local sporeObject = SporeObject:New(self._mainPanel)
			sporeObject:SetPosition(x, y, radius, idx)
			self.sporeMap[idx] = sporeObject
			self._spikySporeManager:AddSpikyObject(sporeObject)
		end
	end
end



function BBCCObjectManager:OnEatObject(removeObjType, removeObjIdx)
	if removeObjType == Type_Food  then
		print("#########吃掉了食物  ", removeObjIdx)
	    if not self.foodMap[removeObjIdx] then
	        -- error("eat food but lua has not this food idx:  ", removeObjIdx)
	    else
	        local food = self.foodMap[removeObjIdx]
	        self.foodMap[removeObjIdx] = nil
	        food:Remove()
	    end
	elseif removeObjType == Type_Spiky then
	    if not self.spikyMap[removeObjIdx] then
	        -- error("eat spiky but lua has not this spiky idx:  ", removeObjIdx)
	    else
	        local spiky = self.spikyMap[removeObjIdx]
	        self.spikyMap[removeObjIdx] = nil
	        spiky:Remove()
	        print("刺球被吃了 ", removeObjIdx)
	    end
	elseif removeObjType == Type_Spore then

		if not self.sporeMap[removeObjIdx] then
	        -- error("eat spiky but lua has not this spiky idx:  ", removeObjIdx)
	    else
	        local spore = self.sporeMap[removeObjIdx]
	        self.sporeMap[removeObjIdx] = nil
	        spore:Remove()
	        print("孢子被吃了 ", removeObjIdx)
	    end
	end
end

function BBCCObjectManager:RemoveFood(foodIdx)
	local food = self.foodMap[foodIdx]
	if not food then
		error("RemoveFood no food idx:  "..foodIdx)
		return
	end
	self.foodMap[foodIdx] = nil
	food:Remove()
end

function BBCCObjectManager:RemoveSpore(sporeIdx)
	local spore = self.sporeMap[sporeIdx]
	if not spore then
		error("RemoveSpore no spore idx: "..sporeIdx)
		return
	end
	self.sporeMap[sporeIdx] = nil
	spore:Remove()
end

function BBCCObjectManager:SimulateEatFood(foodIdx)
	local food = self.foodMap[foodIdx]
	if not food then
		error("SimulateEatFood no food idx:  "..foodIdx)
		return
	end
	food.displayObject:setVisible(false)
end

function BBCCObjectManager:SimulateEatSpore(sporeIdx)
	local spore = self.sporeMap[sporeIdx]
	if not spore then
		error("RemoveSpore no spore idx: "..sporeIdx)
		return
	end
	spore.displayObject:setVisible(false)
end