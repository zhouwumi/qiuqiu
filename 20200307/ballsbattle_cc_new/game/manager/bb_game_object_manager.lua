BBGameObjectManager = CreateClass()

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
local BBGameMathUtils = import('logic.dialog.ballsbattle_cc_new.game.math.bb_game_math_utils').BBGameMathUtils
local BBGameSporeObject = import('logic.dialog.ballsbattle_cc_new.game.objects.bb_game_move_ball').BBGameSporeObject
local BBGameFoodObject = import('logic.dialog.ballsbattle_cc_new.game.objects.bb_game_static_object').BBGameFoodObject
local BBSpikyBallObject = import('logic.dialog.ballsbattle_cc_new.game.objects.bb_game_static_object').BBSpikyBallObject
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBGameObjectManager:__init__(gameManager)
	self.gameManager = gameManager
	self.curObjIdx = 0
end

function BBGameObjectManager:GetNextObjIdx()
	self.curObjIdx = self.curObjIdx + 1
	return self.curObjIdx
end

function BBGameObjectManager:CreateFood(x, y)
	local idx = BBCCUtils.ClientPackLocation(x, y);
	local newFood = BBGameFoodObject:New()
	newFood.idx = idx;
	newFood:SetRadius(0);
	newFood:ChangePosition(x, y);
	return newFood;
end

function BBGameObjectManager:CreateSpikyBall(x, y, mass, idx)
	local newSpikyBall = BBSpikyBallObject:New()
    newSpikyBall:SetBallMass(mass);

	newSpikyBall:ChangePosition(x, y);
	newSpikyBall.idx = idx;
	return newSpikyBall;
end