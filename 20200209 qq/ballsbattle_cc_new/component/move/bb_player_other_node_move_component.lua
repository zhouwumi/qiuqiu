BBOtherNodeMoveComponent = CreateClass()

function BBOtherNodeMoveComponent:__init__(playerNode)
	self._playerNode = playerNode
	self._displayObject = playerNode.displayObject
end

function BBOtherNodeMoveComponent:SetInitInfo(x, y)
	self:_setNodePosition(x, y)
	self.speedRadio = 1
end

-- function BBOtherNodeMoveComponent:MoveToTarget(x, y)
-- 	local speedX = self._playerNode.speedX
-- 	local speedY = self._playerNode.speedY

-- 	local totalSpeed = math.sqrt(speedX * speedX + speedY * speedY)
-- 	local deltaX = x - self.lastX
-- 	local deltaY = y - self.lastY
-- 	local deltaLen = math.sqrt(deltaX * deltaX + deltaY * deltaY)

-- 	local maxSpeed = math.ceil(deltaLen / 6)
-- 	totalSpeed = math.max(totalSpeed, maxSpeed)

-- 	if self.hasMoveToTarget and self.playerObject and self.playerObject.pressure == 0 then
-- 		if deltaLen < 3 then
-- 			return
-- 		else
-- 			self.hasMoveToTarget = false
-- 		end
-- 	end

-- 	local dirX = deltaX / deltaLen
-- 	local dirY = deltaY / deltaLen
	
-- 	local moveX = totalSpeed * dirX
-- 	local moveY = totalSpeed * dirY
-- 	local curMoveX = math.abs(deltaX) > math.abs(moveX) and moveX or deltaX
-- 	local curMoveY = math.abs(deltaY) > math.abs(moveY) and moveY or deltaY

-- 	local newX = self.lastX + curMoveX
-- 	local newY = self.lastY + curMoveY
-- 	self:_setNodePosition(newX, newY)
-- 	if curMoveX == deltaX and curMoveY == deltaY then
-- 		self.hasMoveToTarget = true
-- 	end
-- 	print("球的移动: ", self._playerNode.objectIdx, curMoveX, curMoveY, deltaLen, self.lastX, self.lastY, x, y)
-- end

function BBOtherNodeMoveComponent:UpdateNewTargetPoint(x, y)
	local deltaX = x - self.lastX
	local deltaY = y - self.lastY

	local speedX = self._playerNode.speedX
	local speedY = self._playerNode.speedY
	local maxSpeed = math.sqrt(speedX * speedX + speedY * speedY)
	maxSpeed = maxSpeed * self.speedRadio

	local deltaLen = math.sqrt(deltaX * deltaX + deltaY * deltaY)
	local dirX = deltaX / deltaLen
	local dirY = deltaY / deltaLen
	
	local moveX = maxSpeed * dirX
	local moveY = maxSpeed * dirY

	local curMoveX = math.abs(deltaX) > math.abs(moveX) and moveX or deltaX
	local curMoveY = math.abs(deltaY) > math.abs(moveY) and moveY or deltaY

	local newX = self.lastX + curMoveX
	local newY = self.lastY + curMoveY
	self:_setNodePosition(newX, newY)

	if curMoveX == deltaX and curMoveY == deltaY then
		self.speedRadio = 1
	end

	self.speedRadio = math.min(1.5,self.speedRadio + 0.01)
	print("球速度: ", self._playerNode.objectIdx, maxSpeed, speedX, speedY, curMoveX, curMoveY)
	print("球的移动: ", self._playerNode.objectIdx, deltaLen, self.lastX, self.lastY, x, y)
end

function BBOtherNodeMoveComponent:_setNodePosition(x, y)
	self.lastX = x
	self.lastY = y
	self._displayObject:SetPosition(x, y)
end