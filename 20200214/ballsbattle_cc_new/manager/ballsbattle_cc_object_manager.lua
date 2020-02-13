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

function BBCCObjectManager:GenerateSpore(idx)
	local sporeObject = SporeObject:New(self._mainPanel)
	sporeObject:SetBirthInfo(idx)
	self.sporeMap[idx] = sporeObject
	self._spikySporeManager:AddSpikyObject(sporeObject)
end

function BBCCObjectManager:CreateSpikyNew(spikyInfo)
	local x = spikyInfo.x
	local y = spikyInfo.y
	local mass = spikyInfo.mass
	local idx = spikyInfo.idx
	local spikyObject = SpikyObject:New(self._mainPanel)
    spikyObject:SetPosition(x, y, mass, idx)
    self.spikyMap[idx] = spikyObject
    self._spikySporeManager:AddSpikyObject(spikyObject)
end

function BBCCObjectManager:UpdateSpore()
	for idx, sporeObject in pairs(self.sporeMap) do
		sporeObject:Update()
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


function BBCCObjectManager:RemoveFood(foodIdx, nodeIdx)
	local food = self.foodMap[foodIdx]
	if not food then
		error("RemoveFood no food idx:  "..foodIdx)
		return
	end
	self.foodMap[foodIdx] = nil
	food:Remove(nodeIdx)
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

function BBCCObjectManager:RemoveSpiky(spikyIdx)
	local spiky = self.spikyMap[spikyIdx]
	if not spiky then
		error("RemoveSpiky no spiky idx: "..spikyIdx)
		return
	end
	self.spikyMap[spikyIdx] = nil
	spiky:Remove()
end

function BBCCObjectManager:SimulateEatFood(foodIdx, nodeIdx)
	local food = self.foodMap[foodIdx]
	if not food then
		error("SimulateEatFood no food idx:  "..foodIdx)
		return
	end
	-- food.displayObject:setVisible(false)
	food:SimulateHide(nodeIdx)
end

function BBCCObjectManager:SimulateEatSpore(sporeIdx)
	local spore = self.sporeMap[sporeIdx]
	if not spore then
		error("RemoveSpore no spore idx: "..sporeIdx)
		return
	end
	spore.displayObject:setVisible(false)
end