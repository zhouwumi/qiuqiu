local BBGameBaseBallObject = import('logic.dialog.ballsbattle_cc_new.game.objects.bb_game_static_object').BBGameBaseBallObject
local BBGameMathUtils = import('logic.dialog.ballsbattle_cc_new.game.math.bb_game_math_utils').BBGameMathUtils
local BBGameBallDeltaData = import('logic.dialog.ballsbattle_cc_new.game.data.bb_game_frame_data').BBGameBallDeltaData

BBGameMoveBallObject, MoveBallSuper = CreateClass(BBGameBaseBallObject)

local local_bb_fix_float = BBGameMathUtils.bb_fix_float
local local_bb_float_to_int = BBGameMathUtils.bb_float_to_int

function BBGameMoveBallObject:__init__()
	MoveBallSuper.__init__(self)
	self.fromId = 0
	self.currentSpeedVec = {x = 0, y = 0}
	self.speed = 0
	self.mDeltaData = BBGameBallDeltaData:New()
end

function BBGameMoveBallObject:SetSpeedVec(targetX, targetY)
	local x, y = BBGameMathUtils.GetFixedVetor2ByXY(targetX, targetY, self.speed)
	self.currentSpeedVec.x = x
	self.currentSpeedVec.y = y
	BBGameMathUtils.bb_fix_bb_vector(self.currentSpeedVec)
end

function BBGameMoveBallObject:SetBallMass(mass)
	self.mass = mass
	self:SetRadius(BBGameMathUtils.Mass2Radius(mass))
	self.speed = BBGameMathUtils.Mass2Speed(mass)
end

function BBGameMoveBallObject:ChangeDeltaMass(delta)
	local oldSpeed = self.speed
	self:SetBallMass(self.mass + delta)
	local newSpeed = self.speed
	local rate = newSpeed / oldSpeed
	self.currentSpeedVec.x = self.currentSpeedVec.x * rate
	self.currentSpeedVec.y = self.currentSpeedVec.y * rate
	BBGameMathUtils.bb_fix_bb_vector(self.currentSpeedVec)
end

function BBGameMoveBallObject:GetIntSpeedX()
	return local_bb_float_to_int(self.currentSpeedVec.x)
end

function BBGameMoveBallObject:GetIntSpeedY()
	return local_bb_float_to_int(self.currentSpeedVec.y)
end

function BBGameMoveBallObject:GetIntLocationX()
	return local_bb_float_to_int(self.location.x)
end

function BBGameMoveBallObject:GetIntLocationY()
	return local_bb_float_to_int(self.location.y)
end

function BBGameMoveBallObject:GetRenderX()
	return local_bb_float_to_int(self.mDeltaData.fixedLocation.x)
end

function BBGameMoveBallObject:GetRenderY()
	return local_bb_float_to_int(self.mDeltaData.fixedLocation.y)
end

function BBGameMoveBallObject:ChangePosition(x, y)
	local tempLocation = self.location
	local radius = self.radius
	tempLocation.x = local_bb_fix_float(x)
	tempLocation.y = local_bb_fix_float(y)
	self.rect:SetRect(tempLocation.x - radius, tempLocation.y - radius, tempLocation.x + radius, tempLocation.y + radius)
	-- print("ChangePosition  ", self.location.x, self.location.y)
end

function BBGameMoveBallObject:ChangeRenderPosition(newX, newY)
	newX = local_bb_fix_float(newX)
	newY = local_bb_fix_float(newY)
	self.mDeltaData.location.x = newX
	self.mDeltaData.location.y = newY
	-- print("ChangeRenderPosition  ", newX, newY)
end

BBGameSporeObject, SporeSuperObject = CreateClass(BBGameMoveBallObject)

function BBGameSporeObject:__init__()
	SporeSuperObject.__init__(self)
	self.uid = 0
	self.cd = 0
	self.initSpeed = 0
	self.initDeltaSpeed = 0
	self.initStopFrame = 0
end

function BBGameSporeObject:InitMove()
	local x, y = BBGameMathUtils.GetFixedVetor2(self.currentSpeedVec, self.initSpeed)
	self.currentSpeedVec.x = x
	self.currentSpeedVec.y = y
	BBGameMathUtils.bb_fix_bb_vector(self.currentSpeedVec)
	-- print('BBGameSporeObject  ', self.currentSpeedVec.x, self.currentSpeedVec.y, self.initSpeed)
	self:ChangePosition(self.location.x + self.currentSpeedVec.x, self.location.y + self.currentSpeedVec.y)
	self.initStopFrame = self.initStopFrame - 1
	self.initSpeed = self.initSpeed - self.initDeltaSpeed

end

function BBGameSporeObject:CalculateInitMoveParams(radius, frame, initSpeed)
	self.initStopFrame = frame
	local delta = math.abs(initSpeed / frame)
	local initDelta = delta
	local init = frame * initDelta

	self.initSpeed = init
	self.initDeltaSpeed = initDelta
	-- print("BBGameSporeObject CalculateInitMoveParams", radius, frame, initDelta)
end

function BBGameSporeObject:ChangeRenderPosition(newX, newY)
	newX = local_bb_fix_float(newX)
	newY = local_bb_fix_float(newY)
	local tempDeltaData = self.mDeltaData
	tempDeltaData.location.x = newX
	tempDeltaData.location.y = newY

	local isFix, fixedX, fixedY = BBGameMathUtils.FixCircleDefaultRect(newX, newY, self:GetRadius())
	tempDeltaData.fixedLocation.x = fixedX
	tempDeltaData.fixedLocation.y = fixedY
end

function BBGameSporeObject:CalcBallDelta()
	local tempDeltaData = self.mDeltaData
	local ticks = self.initStopFrame
	tempDeltaData.wrapTicks = ticks
	tempDeltaData.wrapLocationOffset.x = (self.location.x - tempDeltaData.location.x) / ticks
	tempDeltaData.wrapLocationOffset.y = (self.location.y - tempDeltaData.location.y) / ticks
end