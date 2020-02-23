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
	local x, y, mass, cd, protect, speedX, speedY = self._mainPanel.gameManager:GetPlayerSimpleNodeInfo(self.objectIdx)
	if not x then --被移除了
		return
	end

	self.realX = x
	self.realY = y
	self.protect = protect
	if not protect then
		print(self.objectIdx, x, y, mass, cd, protect, speedX, speedY)
	end
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
	if not self.isMe then
		-- print(self.objectIdx, self._mainPanel.frame, x, y, mass, renderX, renderY, lastX, lastY)
	end

	self:_updateDisplayModel()
	if self._mainPanel.frameManager.isKeyFrame or isFirst then
		self.displayObject:SetPosition(renderX, renderY)
	else
		self.displayObject:SetPosition((renderX + lastX) / 2, (renderY + lastY) / 2)
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
	self.displayObject:stopAllActions()
	self._mainPanel.playerManager:RemoveFromPool(self)
end

function PlayerNodeObject:TryRemove(callback)
	local eatNodeId = self._mainPanel.infoManager.beEatNodeCache[self.objectIdx]
	local function realRemove()
		self:Remove()
		if callback then
			callback()
		end
	end
	if not eatNodeId then
		realRemove()
	else
		local nodeObj = self._mainPanel.playerManager:GetPlayerNodeObject(eatNodeId)
		if not nodeObj then
			realRemove()
		else
			if self.uid == nodeObj.uid then
				realRemove()
				return
			end
			local curX, curY = self:GetDisplayPosition()
			local otherX, otherY = nodeObj:GetDisplayPosition()
			local distance = math.sqrt(math.pow(otherX - curX, 2) + math.pow(otherY - curY, 2))
			local targetDistance = math.sqrt(math.pow(otherX - self.realX / 1000, 2) + math.pow(otherY - self.realY / 1000, 2))
			local eatCircle = math.abs(self.radius - nodeObj.radius)
			print("待确定消失的节点  ", distance, targetDistance, self.radius, nodeObj.radius, eatCircle)
			if distance > targetDistance  then
				self:MoveToTarget(realRemove)
			-- elseif targetDistance < eatCircle then
			-- 	self:MoveToTarget(calback)
			-- elseif distance < eatCircle then
			-- 	realRemove()
			else
				if distance < eatCircle then
					realRemove()
				else
					self.displayObject:DelayCall(1 / 30 * 2, function()
						realRemove()
					end)
				end
			end
		end
	end
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