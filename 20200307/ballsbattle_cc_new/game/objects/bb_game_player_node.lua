local BBGameMoveBallObject = import('logic.dialog.ballsbattle_cc_new.game.objects.bb_game_move_ball').BBGameMoveBallObject
local BBGameMathUtils = import('logic.dialog.ballsbattle_cc_new.game.math.bb_game_math_utils').BBGameMathUtils
local BBGameBallDeltaData = import('logic.dialog.ballsbattle_cc_new.game.data.bb_game_frame_data').BBGameBallDeltaData
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

BBGamePlayerNodeObject, PlayerNodeSuper = CreateClass(BBGameMoveBallObject)

function BBGamePlayerNodeObject:__init__()
	PlayerNodeSuper.__init__(self)
	self.uid = 0
	self.cd = 0
	self.protect = 0
	self.initStopFrame = 0
	self.initSpeed = 0
	self.initDeltaSpeed = 0
	self.player = nil
	self.realX = 0
	self.realY = 0
end

function BBGamePlayerNodeObject:ChangeCd(delta)
	local cd = math.floor(delta * self.mass / 1000 + 0.5)
	self.cd = math.floor(cd / 5) * 5
end

function BBGamePlayerNodeObject:ChangeRenderPosition(newX, newY)
	newX = BBGameMathUtils.bb_fix_float(newX)
	newY = BBGameMathUtils.bb_fix_float(newY)
	self.mDeltaData.location.x = newX
	self.mDeltaData.location.y = newY
	self.realX = newX
	self.realY = newY
end


function BBGamePlayerNodeObject:InitMove()
	self.initSpeed = self.initSpeed - self.initDeltaSpeed
	local x, y = BBGameMathUtils.GetFixedVetor2(self.currentSpeedVec, self.initSpeed)
	self.currentSpeedVec.x = x
	self.currentSpeedVec.y = y
	BBGameMathUtils.bb_fix_bb_vector(self.currentSpeedVec)
end

function BBGamePlayerNodeObject:SetSpeedVec(targetX, targetY)
	local x, y = BBGameMathUtils.GetFixedVetor2ByXY(targetX, targetY, self.speed)
	self.currentSpeedVec.x = x
	self.currentSpeedVec.y = y
	BBGameMathUtils.bb_fix_bb_vector(self.currentSpeedVec)
end

function BBGamePlayerNodeObject:CalculateInitMoveParams(radius, frame, initSpeed)
	self.initStopFrame = frame
	local delta = math.abs(initSpeed / frame)
	delta = radius / (frame * (frame - 1) / 2) + delta
	local initDelta = delta
	local init = frame * initDelta

	self.initSpeed = init
	self.initDeltaSpeed = initDelta
end

function BBGamePlayerNodeObject:GetCrc()
	return BBGameMathUtils.GetCRC32({BBGameMathUtils.bb_get_crc_best_value(self.location.x), BBGameMathUtils.bb_get_crc_best_value(self.location.y)})
end

function BBGamePlayerNodeObject:Move(gameManager)
	-- if not self.player:IsMe() then
	-- 	print("BBGamePlayerNodeObject Move before  ", self.idx, self.location.x, self.location.y, self.mDeltaData.location.x, self.mDeltaData.location.y)
	-- end
	
	local oldLocation = {x = self.location.x, y = self.location.y}
	local locVecX, locVecY = self:_stepMove()
	-- print("_stepMove ", locVecX, locVecY)
	local isFix, fixedX, fixedY = BBGameMathUtils.FixCircle(gameManager.mapRect, locVecX, locVecY, self:GetRadius())
	locVecX = fixedX
	locVecY = fixedY
	-- print("fixCirlce  ", locVecX, locVecY, self:GetRadius())
	if self.initStopFrame == 0 and #self.player.vecPlayerNodes > 1 then
		local isHit, newX, newY = self:_handleNodeHit(locVecX, locVecY)
		locVecX = newX
		locVecY = newY
		if isHit then
			-- print("handle hit position is: ", fixedX, fixedY)
		end
	end
	
	local isFix, fixedX, fixedY = BBGameMathUtils.FixCircle(gameManager.mapRect, locVecX, locVecY, self:GetRadius())

	locVecX = fixedX
	locVecY = fixedY
	
	self:ChangePosition(fixedX, fixedY)
	
	if not self.player.isCatchingUp then
		local deltaX = self.location.x - oldLocation.x
		local deltaY = self.location.y - oldLocation.y
		local newRenderX = self.mDeltaData.location.x + deltaX
		local newRenderY = self.mDeltaData.location.y + deltaY
		if self.mDeltaData.wrapTicks > 0 then
			newRenderX = newRenderX + self.mDeltaData.wrapLocationOffset.x
			newRenderY = newRenderY + self.mDeltaData.wrapLocationOffset.y
			self.mDeltaData.wrapTicks = self.mDeltaData.wrapTicks - 1
		end
		self:ChangeRenderPosition(newRenderX, newRenderY)
		-- print("client node current render position is: ", newRenderX, newRenderY)
		-- print("server node:  ", self.idx, self.location.x, self.location.y, self.mDeltaData.location.x, self.mDeltaData.location.y)
	end
	-- gameManager.NodeTree:UpdateCircleNode(self)
	if not self.player:IsMe() then
		-- print("BBGamePlayerNodeObject Move after ", self.idx,  self.location.x, self.location.y, self.mDeltaData.location.x, self.mDeltaData.location.y, self.mass)
		-- print("wrapTicks info: ", self.mDeltaData.wrapTicks,self.mDeltaData.wrapLocationOffset.x,self.mDeltaData.wrapLocationOffset.y)
	end
end

function BBGamePlayerNodeObject:MoveBack(gameManager)
	if self.player:IsMe() then
		return
	end
	if self.mDeltaData.wrapTicks <= 0 then
		return
	end
	self.mDeltaData.location.x = self.mDeltaData.location.x + self.mDeltaData.wrapLocationOffset.x
	self.mDeltaData.location.y = self.mDeltaData.location.y + self.mDeltaData.wrapLocationOffset.y
	self.mDeltaData.wrapTicks = self.mDeltaData.wrapTicks - 1
	self:ChangeRenderPosition(self.mDeltaData.location.x, self.mDeltaData.location.y)
	-- gameManager.NodeTree:UpdateCircleNode(self)

end

function BBGamePlayerNodeObject:_stepMove()
	local finalPoint = {x = self.player.FinalPoint.x, y = self.player.FinalPoint.y}
	local lastX = self.location.x
	local lastY = self.location.y
	if self.initStopFrame > 0 then
		self:InitMove()
	end
	local locVec = {x = self.location.x, y = self.location.y}
	if self.currentSpeedVec.x ~= 0 or self.currentSpeedVec.y ~= 0 then
		locVec.x = locVec.x + self.currentSpeedVec.x
		locVec.y = locVec.y + self.currentSpeedVec.y
		-- print("move speed:  ", self.currentSpeedVec.x, self.currentSpeedVec.y)
		if self.initStopFrame > 0 then
			return locVec.x, locVec.y
		end

		if lastX <= finalPoint.x and locVec.x >= finalPoint.x then
			locVec.x = finalPoint.x
		end
		if lastX >= finalPoint.x and locVec.x <= finalPoint.x then
			locVec.x = finalPoint.x
		end

		if lastY <= finalPoint.y and locVec.y >= finalPoint.y then
			locVec.y = finalPoint.y
		end
		if lastY >= finalPoint.y and locVec.y <= finalPoint.y then
			locVec.y = finalPoint.y
		end
	end
	return locVec.x, locVec.y
end

function BBGamePlayerNodeObject:_handleNodeHit(locVecX, locVecY)
	local isHit = false
	local allPlayerNodes = self.player.vecPlayerNodes
	for i = 1, #allPlayerNodes do
		local ballB = allPlayerNodes[i]
		if ballB.initStopFrame <= 0 and ballB ~= self then
			local deltaX = ballB.location.x - locVecX
			local deltaY = ballB.location.y - locVecY
			local distance = math.sqrt(deltaX * deltaX + deltaY *deltaY)
			local totalCircle = ballB:GetRadius() + self:GetRadius()
			if totalCircle > distance then
				local length = totalCircle - distance
				local deltaLen = constant_ballsbattle_cc.BBConfigManager.ballEatRate
				if self.cd > 0 or ballB.cd > 0 or BBGameMathUtils.NeedRollback(self, ballB, deltaLen) then
					local fixVecX = -1 * deltaX
					local fixVecY = -1 * deltaY
					local vecX, vecY = BBGameMathUtils.GetFixedVetor2ByXY(fixVecX, fixVecY, length)
					if locVecX <= ballB.location.x then
						locVecX = locVecX - math.abs(vecX)
					elseif locVecX > ballB.location.x then
						locVecX = locVecX + math.abs(vecX)
					end

					if locVecY <= ballB.location.y then
						locVecY = locVecY - math.abs(vecY)
					elseif locVecY > ballB.location.y then
						locVecY = locVecY + math.abs(vecY)
					end
				end
				isHit = true
			end
		end
	end
	return isHit, locVecX, locVecY
end

function BBGamePlayerNodeObject:ClientSyncServerPlayerNode(fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX, speedY)
	self.fromId = fromId
	self:SetBallMass(mass)
	self.cd = cd
	self.protect = protect
	self.initStopFrame = initStopFrame
	self.initSpeed = initSpeed
	self.initDeltaSpeed = initDeltaSpeed
	self.currentSpeedVec.x = BBGameMathUtils.bb_int_to_float(speedX)
	self.currentSpeedVec.y = BBGameMathUtils.bb_int_to_float(speedY)
	self:ChangePosition(BBGameMathUtils.bb_int_to_float(x), BBGameMathUtils.bb_int_to_float(y))
	self:CalcBallDelta()
	-- print("ClientSyncServerPlayerNode  ", self.currentSpeedVec.x, self.currentSpeedVec.y)
end

function BBGamePlayerNodeObject:ClientSyncRenderToLocation()
	if self.player:IsMe() then return end
	self.mDeltaData.location.x = self.location.x;
	self.mDeltaData.location.y = self.location.y;
	self.mDeltaData.wrapLocationOffset.x = 0
	self.mDeltaData.wrapLocationOffset.y = 0
	self.mDeltaData.wrapTicks = 0;
end

function BBGamePlayerNodeObject:CalcBallDelta()
	local ticks = 0
	if self.player:IsMe() then
		local commands = self.player.mCommandList.mCommandVec or {}
		ticks = #commands
		ticks = math.max(ticks, constant_ballsbattle_cc.BBConfigManager.minCatchUpTicks)
		ticks = math.min(ticks, constant_ballsbattle_cc.BBConfigManager.maxCatchUpTicks)
	else
		local deltaX = self.location.x - self.mDeltaData.location.x
		local deltaY = self.location.y - self.mDeltaData.location.y
		local distance = math.sqrt(deltaX * deltaX + deltaY * deltaY)
		-- local catchUpTicks = math.floor(distance / self.speed * 6)
		local catchUpTicks = math.floor(distance / self.speed * 6)
		catchUpTicks = math.max(catchUpTicks, constant_ballsbattle_cc.BBConfigManager.minOtherCatchUpTicks)
		catchUpTicks = math.min(catchUpTicks, constant_ballsbattle_cc.BBConfigManager.maxOtherCatchUpTicks) --2s钟追上
		ticks = catchUpTicks
		-- print("CalcBallDelta ", self.idx, deltaX, deltaY, ticks, self.speed, distance)
	end
	self.mDeltaData.wrapTicks = ticks
	self.mDeltaData.wrapLocationOffset.x = (self.location.x - self.mDeltaData.location.x) / ticks
	self.mDeltaData.wrapLocationOffset.y = (self.location.y - self.mDeltaData.location.y) / ticks
end

function BBGamePlayerNodeObject:GetRenderX()
	return BBGameMathUtils.bb_float_to_int(self.mDeltaData.location.x)
end

function BBGamePlayerNodeObject:GetRenderY()
	return BBGameMathUtils.bb_float_to_int(self.mDeltaData.location.y)
end