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
	self.delayTime = 0
	-- self.displayObject.blue:setVisible(constant_ballsbattle_cc.ShowShadow)
	self.motion = 'breathe'
end

function PlayerNodeObject:OnJoinGame(idx)
	self.renderXYCache = {}
	self:playMotion('breathe')
	self.motion = 'breathe'
	if g_user_info.get_user_info().uid == self.uid then
	    self.displayObject.arrowSp:setVisible(true)
	    self.isMe = true
	else
	    self.displayObject.arrowSp:setVisible(false)
	    self.isMe = false
	end
	self.isDestory = false
	self.delayTime = 0
	self.objectIdx = idx
	self.displayObject.labelIdx:SetString(tostring(idx))
	self.displayObject:setVisible(true)
	self:_updatePosition(true)
end

function PlayerNodeObject:Update()
	if self.isDestory then
		return
	end
	local lastX, lastY = self.displayObject:GetPosition()
	self:_updatePosition()

	local newX, newY = self.displayObject:GetPosition()
	self.lastMoveX = newX - lastX
	self.lastMoveY = newY - lastY
end

function PlayerNodeObject:_updatePosition(isFirst)
	if not self.isMe then
		self:_updateOtherPosition(isFirst)
	else
		self:_updateMePosition(isFirst)
	end
end

function PlayerNodeObject:_updateOtherPosition(isFirst)
	local x, y, mass, cd, protect, speedX, speedY, initStopFrame = self._mainPanel.gameManager:GetPlayerSimpleNodeInfo(self.objectIdx)
	if not x then --被移除了
		return
	end

	self.realX = x
	self.realY = y
	self.protect = protect
	if protect > 0 then
		self.displayObject.spProtect:setVisible(true)
	else
		self.displayObject.spProtect:setVisible(false)
	end
	x = x / 1000
	y = y / 1000
	local globalPoint = self._mainPanel.playerContainer:ConvertToWorldSpace(x, y)
	local localPoint = self.displayObject.blue:getParent():ConvertToNodeSpace(globalPoint.x, globalPoint.y)
	self.displayObject.blue:SetPosition(localPoint.x, localPoint.y)
	
	local renderX, renderY = self._mainPanel.gameManager:GetPlayerNodeRenderPos(self.objectIdx)
	renderX = renderX / 1000
	renderY = renderY / 1000
	if not self.isMe then
		-- print("client node:  ", self._mainPanel.frame, self.objectIdx, mass, renderX, renderY, x, y)
	end
	
	self:_updateMass(mass)
	local lastX, lastY = self.displayObject:GetPosition()
	-- if not self.isMe then
	-- 	local deltaDistance = 0
	-- 	if self.lastRenderX and self.lastRenderY then
	-- 		deltaDistance = math.sqrt((renderX - self.lastRenderX) * (renderX - self.lastRenderX) + (renderY - self.lastRenderY) * (renderY - self.lastRenderY))
	-- 	end
		-- print(self.objectIdx, self._mainPanel.frame, lastX, lastY, renderX, renderY, deltaDistance, initStopFrame)
	-- end

	self:_updateDisplayModel()
	if isFirst then
		self.displayObject:SetPosition(renderX, renderY)
		self.lastRenderX = nil
		self.lastRenderY = nil
		self.renderXYCache = {}
		self.slowDownSpeed = false
		if initStopFrame > 0 and self.isMe then
			self.displayObject:setVisible(false)
		end
	elseif self._mainPanel.frameManager.isKeyFrame then
		if self.lastRenderX and self.lastRenderY then
			self.displayObject:SetPosition(self.lastRenderX, self.lastRenderY)
		end
	else
		self.nextRenderX = renderX
		self.nextRenderY = renderY
		if self.nextRenderX and self.nextRenderY then
			table.insert(self.renderXYCache, {self.nextRenderX, self.nextRenderY})
		end
		local playerDelay = self._mainPanel.gameManager:GetPlayerDelay(g_user_info.get_user_info().uid) - 1
		playerDelay = math.min(playerDelay, 4)
		playerDelay = math.max(playerDelay, 1)
		-- playerDelay = 1
		if not self.isMe then
			playerDelay = 0
		end
		
		if #self.renderXYCache > playerDelay then
			local lastRenderXY = self.renderXYCache[1]
			self.lastRenderX = lastRenderXY[1]
			self.lastRenderY = lastRenderXY[2]
			table.remove(self.renderXYCache, 1)
			self.displayObject:setVisible(true)
		end
		if self.lastRenderX and self.lastRenderY then
			self.displayObject:SetPosition((self.lastRenderX + lastX) / 2, (self.lastRenderY + lastY) / 2)
		end
	end
	

	if self.isMe then
		self:_updateArrow(speedX, speedY)
		self:_updateMotion()
	end
end

function PlayerNodeObject:_updateMePosition(isFirst)
	local x, y, mass, cd, protect, speedX, speedY, initStopFrame = self._mainPanel.gameManager:GetPlayerSimpleNodeInfo(self.objectIdx)
	if not x then --被移除了
		return
	end

	self.realX = x
	self.realY = y
	self.protect = protect
	if protect > 0 then
		self.displayObject.spProtect:setVisible(true)
	else
		self.displayObject.spProtect:setVisible(false)
	end
	x = x / 1000
	y = y / 1000
	local globalPoint = self._mainPanel.playerContainer:ConvertToWorldSpace(x, y)
	local localPoint = self.displayObject.blue:getParent():ConvertToNodeSpace(globalPoint.x, globalPoint.y)
	self.displayObject.blue:SetPosition(localPoint.x, localPoint.y)
	
	local renderX, renderY = self._mainPanel.gameManager:GetPlayerNodeRenderPos(self.objectIdx)
	renderX = renderX / 1000
	renderY = renderY / 1000
	if not self.isMe then
		-- print("client node:  ", self._mainPanel.frame, self.objectIdx, mass, renderX, renderY, x, y)
	end
	
	self:_updateMass(mass)
	local lastX, lastY = self.displayObject:GetPosition()
	-- if not self.isMe then
	-- 	local deltaDistance = 0
	-- 	if self.lastRenderX and self.lastRenderY then
	-- 		deltaDistance = math.sqrt((renderX - self.lastRenderX) * (renderX - self.lastRenderX) + (renderY - self.lastRenderY) * (renderY - self.lastRenderY))
	-- 	end
		-- print(self.objectIdx, self._mainPanel.frame, lastX, lastY, renderX, renderY, deltaDistance, initStopFrame)
	-- end

	self:_updateDisplayModel()
	if isFirst then
		self.displayObject:SetPosition(renderX, renderY)
		-- self.renderXYCache = {{renderX, renderY}}
		self.lastFrameX = renderX
		self.lastFrameY = renderY
		if initStopFrame > 0 and self.isMe then
			self.displayObject:setVisible(false)
		end
	else
		if not self._mainPanel.frameManager.isKeyFrame then
			table.insert(self.renderXYCache, {(self.lastFrameX + renderX) / 2, (self.lastFrameY + renderY) / 2})
			table.insert(self.renderXYCache, {renderX, renderY})
			self.lastFrameX = renderX
			self.lastFrameY = renderY
		end
		if #self.renderXYCache <= 0 then
			return
		end

		if self.currentCathFrame == nil or self.currentCathFrame <= 0 then
			local playerDelay = self._mainPanel.gameManager:GetPlayerDelay(g_user_info.get_user_info().uid) - 1
			-- playerDelay = math.min(playerDelay, 4)
			playerDelay = math.max(playerDelay, 1)

			local targetFrame = #self.renderXYCache - playerDelay * 2
			if targetFrame < -1 then --这个时候就需要等待了
				self.currentCathFrame = #self.renderXYCache + 2
				local next_pos = self.renderXYCache[#self.renderXYCache]
				self.catchOffsetX = (next_pos[1] - lastX) / self.currentCathFrame
				self.catchOffsetY = (next_pos[2] - lastY) / self.currentCathFrame
				self.targetFrame = #self.renderXYCache
			elseif targetFrame <= 1 then --正常消耗
				self.currentCathFrame = 1
				local next_pos = self.renderXYCache[1]
				self.catchOffsetX = (next_pos[1] - lastX) / self.currentCathFrame
				self.catchOffsetY = (next_pos[2] - lastY) / self.currentCathFrame
				self.targetFrame = 1
			else --需要快速消耗
				self.currentCathFrame = #self.renderXYCache - 2
				local next_pos = self.renderXYCache[#self.renderXYCache]
				self.catchOffsetX = (next_pos[1] - lastX) / self.currentCathFrame
				self.catchOffsetY = (next_pos[2] - lastY) / self.currentCathFrame
				self.targetFrame = #self.renderXYCache
			end
			print("_updateMePosition  ", #self.renderXYCache, playerDelay, targetFrame, self.targetFrame, self.currentCathFrame, self._mainPanel.frame)
		end
		if self.currentCathFrame and self.currentCathFrame > 0 then
			self.currentCathFrame = self.currentCathFrame - 1
			self.displayObject:SetPosition(lastX + self.catchOffsetX, lastY + self.catchOffsetY)
			self.displayObject:setVisible(true)
			print("movemovemove ", self._mainPanel.frame, self.catchOffsetX, self.catchOffsetY)
			if self.currentCathFrame == 0 then
				for index = 1, self.targetFrame do
					table.remove(self.renderXYCache, 1)
				end
			end
		end

		
		-- local nextFrame = self.renderXYCache[]

		-- local next_pos = self.renderXYCache[1]
		-- self.displayObject:SetPosition(next_pos[1], next_pos[2])
		-- table.remove(self.renderXYCache, 1)
	end


	if self.isMe then
		self:_updateArrow(speedX, speedY)
		self:_updateMotion()
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

function PlayerNodeObject:Remove()
	self.isDestory = true
	self.renderXYCache = {}
	self.displayObject:stopAllActions()
	self._mainPanel.playerManager:RemoveFromPool(self)
end

function PlayerNodeObject:TryRemove(callback)
	self:Remove()
	if callback then
		callback()
	end
	-- local eatNodeId = self._mainPanel.infoManager.beEatNodeCache[self.objectIdx]
	-- local function realRemove()
	-- 	self:Remove()
	-- 	if callback then
	-- 		callback()
	-- 	end
	-- end
	-- if not eatNodeId then
	-- 	realRemove()
	-- else
	-- 	local nodeObj = self._mainPanel.playerManager:GetPlayerNodeObject(eatNodeId)
	-- 	if not nodeObj then
	-- 		realRemove()
	-- 	else
	-- 		if self.uid == nodeObj.uid then
	-- 			realRemove()
	-- 			return
	-- 		end
	-- 		local curX, curY = self:GetDisplayPosition()
	-- 		local otherX, otherY = nodeObj:GetDisplayPosition()
	-- 		local distance = math.sqrt(math.pow(otherX - curX, 2) + math.pow(otherY - curY, 2))
	-- 		local targetDistance = math.sqrt(math.pow(otherX - self.realX / 1000, 2) + math.pow(otherY - self.realY / 1000, 2))
	-- 		local eatCircle = math.abs(self.radius - nodeObj.radius)
	-- 		print("待确定消失的节点  ", distance, targetDistance, self.radius, nodeObj.radius, eatCircle)
	-- 		if distance > targetDistance  then
	-- 			self:MoveToTarget(realRemove)
	-- 		-- elseif targetDistance < eatCircle then
	-- 		-- 	self:MoveToTarget(calback)
	-- 		-- elseif distance < eatCircle then
	-- 		-- 	realRemove()
	-- 		else
	-- 			if distance < eatCircle then
	-- 				realRemove()
	-- 			else
	-- 				self.displayObject:DelayCall(1 / 30 * 2, function()
	-- 					realRemove()
	-- 				end)
	-- 			end
	-- 		end
	-- 	end
	-- end
end

function PlayerNodeObject:MoveToTarget(calback)
	local curX, curY = self:GetDisplayPosition() 
	local times = 0
	self.displayObject:DelayCall(1 / 30, function()
		local eatNodeId = self._mainPanel.infoManager.beEatNodeCache[self.objectIdx]
		local nodeObj = self._mainPanel.playerManager:GetPlayerNodeObject(eatNodeId)
		if not nodeObj then
			if calback then
				calback()
			end
			return
		end

		local otherX, otherY = nodeObj:GetDisplayPosition()
		local newDeltaX = otherX - curX
		local newDeltaY = otherY - curY
		local distance = math.sqrt(math.pow(newDeltaX, 2) + math.pow(newDeltaY, 2))
		local eatCircle = math.abs(self.radius - nodeObj.radius)
		if distance < eatCircle then
			if calback then
				calback()
			end
			return
		end

		if times >= 5 then
			if newDeltaX * self.lastMoveX + newDeltaY * self.lastMoveY <= 0 then
				if calback then
					calback()
				end
				return
			end
		end

		curX = curX + self.lastMoveX
		curY = curY + self.lastMoveY
		self.displayObject:SetPosition(curX, curY)
		-- print("MoveToTarget  ", self.objectIdx, curX, curY, otherX, otherY)
		-- message("达到了目的地")
		times = times + 1
		return 1 / 30
	end)
end

function PlayerNodeObject:GetDisplayPosition()
	return self.displayObject:GetPosition()
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