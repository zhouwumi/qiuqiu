local BBGameBaseObject = import('logic.dialog.ballsbattle_cc_new.game.base.bb_game_base_object').BBGameBaseObject
local BBGameMathUtils = import('logic.dialog.ballsbattle_cc_new.game.math.bb_game_math_utils').BBGameMathUtils

BBGameFoodObject, FoodSuper = CreateClass(BBGameBaseObject)

function BBGameFoodObject:__init__()
	FoodSuper.__init__(self)
end

BBGameBaseBallObject, BaseBallSuper = CreateClass(BBGameBaseObject)

function BBGameBaseBallObject:__init__()
	BaseBallSuper.__init__(self)
	self.mass = 0
end

function BBGameBaseBallObject:SetBallMass(mass)
	self.mass = mass
	self:SetRadius(BBGameMathUtils.Mass2Radius(mass))
end

function BBGameBaseBallObject:GetBallMass()	
	return self.mass
end

BBSpikyBallObject, SpikySuper = CreateClass(BBGameBaseBallObject)

function BBSpikyBallObject:__init__()
	SpikySuper.__init__(self)
end