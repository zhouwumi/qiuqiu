local BBBaseObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_base_object').BBBaseObject
PlayerNodeObject, Super = CreateClass(BBBaseObject)

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')
local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
local TAG = 10000
local TAG_ROTATE = 10001
local TAG_MOTION = 10002

function PlayerNodeObject:__init__(mainPanel)
	self._mainPanel = mainPanel
	local node = g_uisystem.create_item(mainPanel.playerTemplate)
	self.displayObject = node
	mainPanel.playerContainer:AddChild(nil, node)
	self.isDestory = false
	self._spriteWidth = 50
	self.displayObject.blue:setVisible(constant_ballsbattle_cc.ShowShadow)
	self:ResetData()
end

function PlayerNodeObject:OnJoinGame(idx)
	self:playMotion('breathe')
	if g_user_info.get_user_info().uid == self.uid then
	    self.displayObject.arrowSp:setVisible(true)
	    self.isMe = true
	else
	    self.displayObject.arrowSp:setVisible(false)
	    self.isMe = false
	end
	self.isDestory = false
	self:ResetData()
	self.objectIdx = idx
	self.displayObject.labelIdx:SetString(tostring(idx))
	self.displayObject:setVisible(true)
	self:_updatePosition(true)
	-- self.displayObject.labelDelay:setVisible(false)
end

function PlayerNodeObject:Update(isJump)
	if self.isDestory then
		return
	end
	if not self._hasInsertFrame then
		return
	end
	self:_updatePosition(isJump)
end

function PlayerNodeObject:_updatePosition(isFirst)
	if not self.isMe then
		if constant_ballsbattle_cc.IsNodeCatchUp then
			self:_updateOtherPositionVersion2(isFirst)
			-- self:_updateOtherPosition(isFirst)
		else
			self:_updateOtherPosition(isFirst)
		end

		local minLocalPoint, maxLocalPoint = self._mainPanel.bgComponent:GetVisibleRect()
		local x, y = self.displayObject:GetPosition()
		local selfMinX = x - self.radius
		local selfMaxX = x + self.radius
		local selfMinY = y - self.radius
		local selfMaxY = y + self.radius
		if selfMinX > maxLocalPoint.x or selfMaxX < minLocalPoint.x or selfMinY > maxLocalPoint.y or selfMaxY < minLocalPoint.y then
			self.displayObject:setVisible(false)
		else
			self.displayObject:setVisible(true)
			self:_trySimulateEat()
		end

	else
		self:_updateMePosition(isFirst)
		self.displayObject:setVisible(true)
		self:_trySimulateEat()
	end
end

function PlayerNodeObject:_trySimulateEat()
	local hasUpdateSimulateEatFood = self:_simulateEatFood()
	local hasUpdateSimulateEatSpore = self:_simulateEatSpore()
	if hasUpdateSimulateEatSpore or hasUpdateSimulateEatFood then
		self.lastSimulateFrame = self._mainPanel.frame
	end
end

function PlayerNodeObject:_simulateEatFood()
	if not self.lastSimulateFrame then
		self.lastSimulateFrame = -4
	end
	if self._mainPanel.frame - self.lastSimulateFrame < 4 then
		return false
	end
	
	local x, y = self.displayObject:GetPosition()
	local results = self._mainPanel.gameManager:SimulateEatFoods(x, y, self.radius)
	for _, idx in ipairs(results or {}) do
		local obj = self._mainPanel.foodManager:GetFoodObj(idx)
		if obj then
			obj:SimulateHide(self.idx)
		end
	end
	return true
end

function PlayerNodeObject:_simulateEatSpore()
	if not self.lastSimulateFrame then
		self.lastSimulateFrame = -4
	end
	if self._mainPanel.frame - self.lastSimulateFrame < 4 then
		return false
	end
	local x, y = self.displayObject:GetPosition()
	local results = self._mainPanel.gameManager:SimulateEatSpores(x, y, self.radius)
	for _, idx in ipairs(results or {}) do
		local obj = self._mainPanel.spikySporeManager:GetSpore(idx)
		if obj then
			if obj.fromId ~= self.idx or obj:GetCurrentCd() <= 0 then
				obj.displayObject:setVisible(false)
			end
			
		end
	end
	return true
end

function PlayerNodeObject:AddNewFrame()
	self._hasInsertFrame = true
	if not self.isMe then
		self.lastUpdateTime = utils_get_tick() - 1 / 30
	end
	local renderX, renderY = self._mainPanel.gameManager:GetPlayerNodeRenderPos(self.objectIdx)
	renderX = renderX / 1000
	renderY = renderY / 1000
	table.insert(self.renderXYCache, {(self.lastFrameX + renderX) / 2, (self.lastFrameY + renderY) / 2})
	table.insert(self.renderXYCache, {renderX, renderY})
	
	
	print("add new frame  ", renderX, renderY)
	self.lastFrameX = renderX
	self.lastFrameY = renderY
	self.lastKeyFrameTime = utils_get_tick()
end

function PlayerNodeObject:_updateBluePosition()
	local globalPoint = self._mainPanel.playerContainer:ConvertToWorldSpace(self.currentPositionX / 1000, self.currentPositionY / 1000)
	local localPoint = self.displayObject.blue:getParent():ConvertToNodeSpace(globalPoint.x, globalPoint.y)
	self.displayObject.blue:SetPosition(localPoint.x, localPoint.y)
end

function PlayerNodeObject:_updateBaseInfo()
	local x, y, mass, cd, protect, speedX, speedY, initStopFrame = self._mainPanel.gameManager:GetPlayerSimpleNodeInfo(self.objectIdx)
	self.speedX = speedX
	self.speedY = speedY
	self.initStopFrame = initStopFrame
	self.currentPositionX = x
	self.currentPositionY = y
	self.realX = x
	self.realY = y
	self.protect = protect
	if protect > 0 then
		self.displayObject.spProtect:setVisible(true)
	else
		self.displayObject.spProtect:setVisible(false)
	end
	self:_updateMass(mass)
	self:_updateDisplayModel()
end

function PlayerNodeObject:_updateOtherPositionVersion2(isFirst)
	self:_updateBaseInfo()
	local renderX, renderY = self._mainPanel.gameManager:GetPlayerNodeRenderPos(self.objectIdx)
	renderX = renderX / 1000
	renderY = renderY / 1000
	
	local lastX, lastY = self.displayObject:GetPosition()
	if isFirst then
		self.displayObject:SetPosition(renderX, renderY)
		self.lastFrameX = renderX
		self.lastFrameY = renderY
		self.lastUpdateTime = utils_get_tick()
	else
		-- if self._mainPanel.frameManager.isKeyFrame then
		-- 	table.insert(self.renderXYCache, {(self.lastFrameX + renderX) / 2, (self.lastFrameY + renderY) / 2})
		-- 	table.insert(self.renderXYCache, {renderX, renderY})
		-- 	self.lastFrameX = renderX
		-- 	self.lastFrameY = renderY

		-- 	local deltaX = (self.lastFrameX + renderX) / 2 - lastX
		-- 	local deltaY = (self.lastFrameY + renderY) / 2 - lastY
		-- 	-- print("计算距离 ", self._mainPanel.frame, deltaX * deltaX + deltaY * deltaY)
		-- 	if deltaX * deltaX + deltaY * deltaY > 100 * 100 then
		-- 		for index = 1, #self.renderXYCache - 2 do
		-- 			table.remove(self.renderXYCache, 1)
		-- 		end
		-- 		local firstPoint = self.renderXYCache[1]
		-- 		self.displayObject:SetPosition(firstPoint[1], firstPoint[2])
		-- 		table.remove(self.renderXYCache, 1)
		-- 		self:_updateBluePosition()
		-- 		self.currentCathFrame = 0
		-- 		self.catchOffsetX = 0
		-- 		self.catchOffsetY = 0
		-- 		self.lastUpdateTime = utils_get_tick()
		-- 		-- print("直接拉扯位置 ")
		-- 		return
		-- 	end
		-- end
		-- if #self.renderXYCache <= 0 then
		-- 	self.lastUpdateTime = utils_get_tick()
		-- 	return
		-- end
		local newTime = utils_get_tick()
		local costTimes = (newTime - self.lastUpdateTime) / (1 / 30)
		local oldUpdateTime = self.lastUpdateTime
		local oldTime = newTime - self.lastUpdateTime
		local x, y = self.displayObject:GetPosition()
		self:_tryCostOtherPosition(costTimes)
		local newX, newY = self.displayObject:GetPosition()
		print("move info ", self.objectIdx, self._mainPanel.frame, oldTime, newX - x, newY - y, (newX - x) / (newY - y), newTime, oldUpdateTime)
	end
	self:_updateBluePosition()
end

function PlayerNodeObject:_tryCostOtherPosition(costTime)
	local lastX, lastY = self.displayObject:GetPosition()
	if #self.renderXYCache <= 0 then
		__G__TRACKBACK__("dddd")
		-- print("no renderXYCache")
		-- message("没有目标位置")
		print("move1  ", self.catchOffsetX * costTime, self.catchOffsetY * costTime, costTime)
		self.displayObject:SetPosition(lastX + self.catchOffsetX * costTime, lastY + self.catchOffsetY * costTime)
		self.lastUpdateTime = self.lastUpdateTime + costTime * 1 / 30
		return
	end
	-- print("renderXYCache  ", #self.renderXYCache)
	
	if self.currentCathFrame == nil or self.currentCathFrame <= 0 then
		local playerDelay = self._mainPanel.gameManager:GetOtherPlayerDelay(self.uid) - 1
		if not self.isMe then
			-- self.displayObject.labelDelay:setVisible(true)
			-- self.displayObject.labelDelay:SetString(tostring(playerDelay + 1))
		end
		-- playerDelay = math.min(playerDelay, 4)
		-- playerDelay = math.max(playerDelay, 1)
		-- print("当前帧信息  ", self._mainPanel.frame, playerDelay, #self.renderXYCache)
		if self.initStopFrame > 0 or playerDelay <= 2 then --正常消耗
			print("正常消耗-----------")
			if #self.renderXYCache > 2 then
				self.currentCathFrame = #self.renderXYCache - 1
				self.targetFrame = #self.renderXYCache
			else
				self.currentCathFrame = 1
				self.targetFrame = 1
			end
		else --需要快速消耗
			if #self.renderXYCache > playerDelay * 2 then
				self.currentCathFrame = #self.renderXYCache - 1
				self.targetFrame = #self.renderXYCache
			else
				self.currentCathFrame = #self.renderXYCache + 1
				self.targetFrame = #self.renderXYCache
			end
		end
		-- self.currentCathFrame = 1
		-- self.targetFrame = 1
		local next_pos = self.renderXYCache[self.targetFrame]
		self.catchOffsetX = (next_pos[1] - lastX) / self.currentCathFrame
		self.catchOffsetY = (next_pos[2] - lastY) / self.currentCathFrame
		print('calc offsetXY  ', self.catchOffsetX, self.catchOffsetY, self.currentCathFrame, next_pos[1], lastX, next_pos[2], lastY)
	end
	if self.currentCathFrame and self.currentCathFrame > 0 then
		local moveTime = math.min(costTime, self.currentCathFrame)
		costTime = costTime - moveTime
		self.currentCathFrame = self.currentCathFrame - moveTime
		self.displayObject:SetPosition(lastX + self.catchOffsetX * moveTime, lastY + self.catchOffsetY * moveTime)
		print("move2  ", self.catchOffsetX * moveTime, self.catchOffsetY * moveTime, costTime, moveTime, self.currentCathFrame, self.catchOffsetX, self.catchOffsetY)
		local x, y = self.displayObject:GetPosition()
		if self.currentCathFrame <= 0 then
			for index = 1, self.targetFrame do
				table.remove(self.renderXYCache, 1)
			end
		end
		self.lastUpdateTime = self.lastUpdateTime + moveTime * 1 / 30
		if costTime > 0 then
			self:_tryCostOtherPosition(costTime)
		end
	end
end

function PlayerNodeObject:_updateOtherPosition(isFirst)
	self:_updateBaseInfo()
	local renderX, renderY = self._mainPanel.gameManager:GetPlayerNodeRenderPos(self.objectIdx)
	renderX = renderX / 1000
	renderY = renderY / 1000
	
	local lastX, lastY = self.displayObject:GetPosition()
	if isFirst then
		self.displayObject:SetPosition(renderX, renderY)
	elseif self._mainPanel.frameManager.isKeyFrame then
		self.displayObject:SetPosition((renderX + lastX) / 2, (renderY + lastY) / 2)
	else
		self.displayObject:SetPosition(renderX, renderY)
	end
	local x, y = self.displayObject:GetPosition()
	if not self.isMe then
		-- print("_updateOtherPosition ", self.idx, self._mainPanel.frame, x, y)
	end
	self:_updateBluePosition()
end

function PlayerNodeObject:_updateMePosition(isFirst)
	self:_updateBaseInfo()
	
	local renderX, renderY = self._mainPanel.gameManager:GetPlayerNodeRenderPos(self.objectIdx)
	renderX = renderX / 1000
	renderY = renderY / 1000
	local lastX, lastY = self.displayObject:GetPosition()

	if isFirst then
		self.displayObject:SetPosition(renderX, renderY)
		self.lastFrameX = renderX
		self.lastFrameY = renderY
		self.lastUpdateTime = utils_get_tick()
	else
		-- if self._mainPanel.frameManager.isKeyFrame then
		-- 	table.insert(self.renderXYCache, {(self.lastFrameX + renderX) / 2, (self.lastFrameY + renderY) / 2})
		-- 	table.insert(self.renderXYCache, {renderX, renderY})
		-- 	self.lastFrameX = renderX
		-- 	self.lastFrameY = renderY
		-- 	-- self.lastUpdateTime = utils_get_tick()
		-- end
		local newTime = utils_get_tick()
		local costTime = (newTime - self.lastUpdateTime) / (1 / 30)
		self:_tryCostMePosition(costTime)
		self.lastUpdateTime = newTime
	end

	if self.isMe then
		self:_updateArrow(self.speedX, self.speedY)
		self:_updateMotion()
	end
	self:_updateBluePosition()
end

function PlayerNodeObject:_tryCostMePosition(costTime)
	if #self.renderXYCache <= 0 then
		return
	end

	local lastX, lastY = self.displayObject:GetPosition()
	if self.currentCathFrame == nil or self.currentCathFrame <= 0 then
		local playerDelay = self._mainPanel.gameManager:GetPlayerDelay(g_user_info.get_user_info().uid) - 1
		playerDelay = math.min(playerDelay, 4)
		playerDelay = math.max(playerDelay, 1)

		local targetFrame = #self.renderXYCache - playerDelay * 2 - 2
		if self.initStopFrame > 0 or (targetFrame <= 1 and targetFrame >= -1) then --正常消耗
			self.currentCathFrame = 1
			self.targetFrame = 1
		elseif targetFrame < -1 then
			self.currentCathFrame = #self.renderXYCache + 1
			self.targetFrame = #self.renderXYCache
		else --需要快速消耗
			self.currentCathFrame = #self.renderXYCache - 1
			self.targetFrame = #self.renderXYCache
		end

		if not constant_ballsbattle_cc.IsNodeCatchUp then
			self.currentCathFrame = 1
			self.targetFrame = 1
		end
		local next_pos = self.renderXYCache[self.targetFrame]
		self.catchOffsetX = (next_pos[1] - lastX) / self.currentCathFrame
		self.catchOffsetY = (next_pos[2] - lastY) / self.currentCathFrame
	end

	if self.currentCathFrame and self.currentCathFrame > 0 then
		local moveTime = math.min(costTime, self.currentCathFrame)
		costTime = costTime - moveTime
		self.currentCathFrame = self.currentCathFrame - moveTime
		self.displayObject:SetPosition(lastX + self.catchOffsetX * moveTime, lastY + self.catchOffsetY * moveTime)
		if self.currentCathFrame == 0 then
			for index = 1, self.targetFrame do
				table.remove(self.renderXYCache, 1)
			end
		end
		self.lastUpdateTime = self.lastUpdateTime + moveTime * 1 / 30
		if costTime > 0 then
			self:_tryCostMePosition(costTime)
		end
	end
end

function PlayerNodeObject:_updateDisplayModel()
	local mode
	local ratio = self._mainPanel.bgComponent.curScale / self._mainPanel.bgComponent.begainScale
	if self.radius < 20 / ratio then
	    mode = 1
	else
	    mode = 2
	end

	if self.displayMode ~= mode then
	    -- if mode == 1 then
	    --     self.ballSp.userName:setVisible(false)
	    -- else 
	    --     self.ballSp.userName:setVisible(true)
	    -- end
	    -- self.ballInterpolation.avatorSp:ChangeMode(mode)
	    self.displayMode = mode
	end
end

function PlayerNodeObject:_updateArrow(speedX, speedY)
	local radian = BBCCUtils.get_radian_by_vector(speedX, speedY)
    local targetRotation = 360 - math.deg(radian)
	self.displayObject.arrowSp:setRotation(targetRotation)
end

function PlayerNodeObject:_updateMotion()
	if self.displayMode == 1 then
		return
	end
	local ratio = self._mainPanel.bgComponent.curScale / self._mainPanel.bgComponent.begainScale
	local isFear = false
	local isHappy = false
	local nodeIdxInRounds = self._mainPanel.gameManager:GetNodeInRound(self.objectIdx, 50 / ratio)
	for _, otherIdx in ipairs(nodeIdxInRounds or {}) do
		local otherNode = self._mainPanel.playerManager:GetPlayerNodeObject(otherIdx)
		local isCircleMatch = math.abs(self.radius - otherNode.radius) > math.min(self.radius, otherNode.radius) * 0.05
		if otherNode and otherNode.displayMode == 2 and isCircleMatch then
			local otherX, otherY = otherNode:GetDisplayPosition()
			local myX, myY = self:GetDisplayPosition()
			local distancePow = math.pow(myX - otherX,2) + math.pow(myY - otherY,2)
			if distancePow < math.pow(otherNode.radius + self.radius + 50 / ratio, 2) then
			    if otherNode.radius > self.radius then
					isFear = true
					otherNode:playMotion('happy')
				elseif otherNode.radius < self.radius then
					isHappy = true
					otherNode:playMotion('fear')
				end
			end
		end
	end
	if isFear then
		self:playMotion('fear')
	elseif isHappy then
		self:playMotion('happy')
	end
end

function PlayerNodeObject:playMotion(motion)
	if self.displayMode == 1 then
		return
	end
	if self.motion == motion then
		return
	end
    self.motion = motion
    self.isPlayingMovie = true
    self.displayObject.sp:PlayMotion(motion,true)

    self.displayObject:stopActionByTag(TAG_MOTION)
    local act = cc.Sequence:create(
        cc.DelayTime:create(2),
        cc.CallFunc:create(function() 
            self.isPlayingMovie = false
            -- self.avatorSp:StopAllMotions() 
            self.motion = 'breathe'
            self.displayObject.sp:PlayMotion(self.motion,true)
        end)
    )
    act:setTag(TAG_MOTION)
    self.displayObject:runAction(act)
end

function PlayerNodeObject:_updateMass(mass)
	local lastMass = self.mass
	self.mass = mass
	if mass ~= lastMass then
		self:caculateRadius()
		self:calculateSpeed()
		self:changeScale(self.delayTime)
		self:changeZOrder()
		self.delayTime = 0.3
	end
end

function PlayerNodeObject:ResetData()
	self.renderXYCache = {}
	self.lastSimulateFrame = -1000
	self.delayTime = 0
	self.motion = 'breathe'
	self.currentCathFrame = 0
	self.catchOffsetX = 0
	self.catchOffsetY = 0
	self._hasInsertFrame = false
end

function PlayerNodeObject:Remove()
	self.isDestory = true
	self:ResetData()
	self.displayObject:stopAllActions()
	self._mainPanel.playerManager:RemoveFromPool(self)
end


function PlayerNodeObject:IsValid()
	if self.isDestory then
		return false
	end
	if not tolua_is_obj(self.displayObject) then
		return false
	end
	return true
end

function PlayerNodeObject:changeScale(delay)
    if delay and delay > 0 then
        self.displayObject:stopActionByTag(TAG)
        local scaleTo = cc.ScaleTo:create(delay, self.radius / self._spriteWidth)
        scaleTo:setTag(TAG)
        self.displayObject:runAction(scaleTo)
    else
        Super.changeScale(self)
    end
end