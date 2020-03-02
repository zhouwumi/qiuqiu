BBGameFoodSpikyManager = CreateClass()

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
local BBGameMathUtils = import('logic.dialog.ballsbattle_cc_new.game.math.bb_game_math_utils').BBGameMathUtils
local GridMap = import('logic.dialog.ballsbattle_cc_new.game.base.bb_game_grid_map').GridMap
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBGameFoodSpikyManager:__init__(gameManager)
	self.gameManager = gameManager
	self.mapFoodIdxs = {}
	self.mapSpikyBalls = {}

	local w = constant_ballsbattle_cc.BBConfigManager.gameWidth
	local h = constant_ballsbattle_cc.BBConfigManager.gameHeight
	self.FoodGridMap = GridMap:New(w, h, 5)
	self.SpikyGridMap = GridMap:New(w, h, 4)
end

function BBGameFoodSpikyManager:RemoveFoodByIdx(idx)
	local food = self.mapFoodIdxs[idx]
	if not food then
		return
	end
	self.mapFoodIdxs[idx] = nil
	self.FoodGridMap:RemoveObject(food)
end

function BBGameFoodSpikyManager:RemoveSpikyByIdx(idx)
	local spiky = self.mapSpikyBalls[idx]
	if not spiky then
		return
	end
	self.mapSpikyBalls[idx] = nil
	self.SpikyGridMap:RemoveObject(spiky)
end

function BBGameFoodSpikyManager:ClientAddNewFoodFromServer(posKey)
	local x, y = BBCCUtils.UnPackServerLocation(posKey)
	local food = self.gameManager.objectManager:CreateFood(x, y)
	self.mapFoodIdxs[posKey] = food
	self.FoodGridMap:AddObjectMapKey(food, x, y)
end

function BBGameFoodSpikyManager:ClientAddNewSpikyFromServer(idx, posKey, mass)
	local x, y = BBCCUtils.UnPackServerLocation(posKey)
	local newSpikyBall = self.gameManager.objectManager:CreateSpikyBall(x, y, mass, idx);
	self.mapSpikyBalls[newSpikyBall.idx] = newSpikyBall
	self.SpikyGridMap:AddObjectMapKey(newSpikyBall, x, y);
end

function BBGameFoodSpikyManager:GetSpikyInfo(idx)
	return self.mapSpikyBalls[idx]
end


function BBGameFoodSpikyManager:GetFood(idx)
	return self.mapFoodIdxs[idx]
end