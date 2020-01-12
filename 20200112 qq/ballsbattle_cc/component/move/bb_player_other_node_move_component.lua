BBOtherNodeMoveComponent = CreateClass()

function BBOtherNodeMoveComponent:__init__(playerNode)
	self._playerNode = playerNode
	self._displayObject = playerNode.displayObject
end

function BBOtherNodeMoveComponent:SetInitInfo(x, y)
	self:_setNodePosition(x, y)
	self.speedRadio = 1
end

function BBOtherNodeMoveComponent:UpdateNewTargetPoint(x, y)
	if self.currentSimulateAcceFrame and self.currentSimulateAcceFrame < self.simulateAcceFrame then
		self:CatchTarget(x, y)
		return
	end
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
	print("球的移动： ", self._playerNode.objectIdx, deltaLen, self.lastX, self.lastY, x, y)

	
end

function BBOtherNodeMoveComponent:CatchTarget(x, y)
	local deltaMainX = x - self.lastMainX
	local deltaMainY = y - self.lastMainY

	self.lastMainX = x
	self.lastMainY = y

	-- local speedX = self._playerNode.speedX
	-- local speedY = self._playerNode.speedY

	-- local maxSpeed = math.sqrt(speedX * speedX + speedY * speedY)
	-- maxSpeed = maxSpeed * self.speedRadio

	-- -- local deltaLen = math.sqrt(deltaX * deltaX + deltaY * deltaY)
	-- local dirX = deltaMainX / deltaLen
	-- local dirY = deltaMainY / deltaLen
	
	-- local moveX = maxSpeed * dirX
	-- local moveY = maxSpeed * dirY

	-- local curMoveX = math.abs(deltaX) > math.abs(moveX) and moveX or deltaX
	-- local curMoveY = math.abs(deltaY) > math.abs(moveY) and moveY or deltaY

	local nextSimulateSpeed = self.currentSimulateSpeed + self.currentSimualteAcce
	if self.currentSimulateAcceFrame > self.halfSimulateAcceFrame then
		nextSimulateSpeed = self.currentSimulateSpeed - self.currentSimualteAcce
	end
	local accMoveX = (nextSimulateSpeed + self.currentSimulateSpeed) / 2 * self.simulateAccDirX
	local accMoveY = (nextSimulateSpeed + self.currentSimulateSpeed) / 2 * self.simulateAccDirY
	local newX = self.lastX + accMoveX + deltaMainX
	local newY = self.lastY + accMoveY + deltaMainY
	self:_setNodePosition(newX, newY)

	if curMoveX == deltaX and curMoveY == deltaY then
		self.speedRadio = 1
	end

	self.speedRadio = math.min(1.5,self.speedRadio + 0.01)
	self.currentSimulateAcceFrame = self.currentSimulateAcceFrame + 1
	self.currentSimulateSpeed = nextSimulateSpeed

	print("加速球速度: ", accMoveX + deltaMainX, accMoveY + deltaMainY, accMoveX, deltaMainX, accMoveY, deltaMainY)
	print("加速球的移动： ", self._playerNode.objectIdx, self.lastX, self.lastY, x, y)
end

function BBOtherNodeMoveComponent:SetDeltaPosition(mainX, mainY)
	self.simulateDeltaX = mainX - self.lastX
	self.simulateDeltaY = mainY - self.lastY

	if self.simulateDeltaX == 0 and self.simulateDeltaY == 0 then
		return
	end


	self.simulateAcceFrame = 10
	self.halfSimulateAcceFrame = self.simulateAcceFrame / 2
	local distance = math.sqrt(self.simulateDeltaX * self.simulateDeltaX + self.simulateDeltaY * self.simulateDeltaY)
	self.maxSimulateSpeed = distance / self.halfSimulateAcceFrame
	self.currentSimulateSpeed = 0
	self.currentSimualteAcce = self.maxSimulateSpeed / self.halfSimulateAcceFrame
	self.currentSimulateAcceFrame = 0
	self.simulateAccDirX = self.simulateDeltaX / distance
	self.simulateAccDirY = self.simulateDeltaY / distance
	self.lastMainX = mainX
	self.lastMainY = mainY
	print("计算差值  ", self.simulateDeltaX, self.simulateDeltaY, self.simulateAccDirX, self.simulateAccDirY, self.currentSimulateSpeed, mainX, mainY, self.lastX, self.lastY)
end


function BBOtherNodeMoveComponent:_setNodePosition(x, y)
	self.lastX = x
	self.lastY = y
	self._displayObject:SetPosition(x, y)
end