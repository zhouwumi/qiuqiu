local BBBaseObject = import('logic.dialog.ballsbattle_cc.objects.bb_base_object').BBBaseObject
PlayerNodeObject, Super = CreateClass(BBBaseObject)

local BBOtherNodeMoveComponent = import('logic.dialog.ballsbattle_cc.component.move.bb_player_other_node_move_component').BBOtherNodeMoveComponent
local TAG = 10000
local TAG_ROTATE = 10001

function PlayerNodeObject:__init__(mainPanel, idx)
	self._mainPanel = mainPanel
	local node = g_uisystem.create_item(mainPanel.playerTemplate)
	self.displayObject = node
	mainPanel.playerContainer:AddChild(nil, node)

	self.isDestory = false

	self._spriteWidth = 50
	self.objectIdx = idx
	self.delayTime = 0
	self.displayObject.labelIdx:SetString(tostring(idx))
	self.moveComponent = BBOtherNodeMoveComponent:New(self)
end

function PlayerNodeObject:SetBirthInfo(x, y, mass)
	self:_updateMass(mass)
	self.moveComponent:SetInitInfo(x, y)
end

function PlayerNodeObject:SetDeltaPosition(mainX, mainY)
	self.moveComponent:SetDeltaPosition(mainX, mainY)
end

function PlayerNodeObject:Update()
	local gameManager = self._mainPanel.frameManager:GetCurrentGameManager()
	local x, y, mass, init, cd, isHit, curSpeedX, curSpeedY = gameManager:GetPlayerNodeInfo(self.objectIdx)
	local isSimulating = self._mainPanel.gameManager ~= gameManager
	if not x and self._mainPanel.gameManager ~= gameManager then
		x, y, mass, init, cd, isHit, curSpeedX, curSpeedY = self._mainPanel.gameManager:GetPlayerNodeInfo(self.objectIdx)
		isSimulating = false
	end
	print("当前player节点位置  ", x, y,isSimulating)
	self.curX = x
	self.curY = y
	self.isHit = isHit
	self.speedX = curSpeedX
	self.speedY = curSpeedY
	self:_updateMass(mass)
	
	self.moveComponent:UpdateNewTargetPoint(x, y)
	--[[if self.isSimulating then
		self.moveComponent:UpdateNewTargetPoint(x, y)
		self:RecordSimulatePosition()
	else
		if self._mainPanel.frameManager:IsCatchingSimulateEndFrame() then
			self.moveComponent:KeepSpeedMove()
		else
			local curFrame = self._mainPanel.gameManager:GetGameFrame()
			local deltaX, deltaY = self:GetSimulatePosition(curFrame)
			if deltaX and deltaY then
				print("delta info:  ", deltaX, deltaY)
				-- self.moveComponent:UpdateNewTargetPoint(x + deltaX, y + deltaY, true)
				self.moveComponent:UpdateNewTargetPoint(x + deltaX, y + deltaY, true)
			else
				self.moveComponent:UpdateNewTargetPoint(x, y)
			end
		end

		
	end--]]
end


function PlayerNodeObject:PrintPostionInfo()
	-- local x, y, mass, init, cd = self._mainPanel.gameManager:GetPlayerNodeInfo(self.objectIdx)
	-- print("更新玩家球的位置  ",self.objectIdx,  x,  y, self.mass, cd)
 --    if init > 0 then
 --    	print("球的Init ", self.objectIdx, init)
 --    end
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
	self.displayObject:removeFromParent()
end

function PlayerNodeObject:GetDisplayPosition()
	return self.displayObject:GetPosition()
end

function PlayerNodeObject:changeScale(delay)
    if delay then
        self.displayObject:stopActionByTag(TAG)
        local scaleTo = cc.ScaleTo:create(delay, self.radius / self._spriteWidth)
        scaleTo:setTag(TAG)
        self.displayObject:runAction(scaleTo)
    else
        Super.changeScale(self)
    end
end

function PlayerNodeObject:NodeStartSimulate(startSimulateFrame)
	self.isSimulating = true
	self.startSimulateFrame = startSimulateFrame
	self.startSimulateX = self.curX
	self.startSimulateY = self.curY
	if not self.cacheSimulateInfo then
		self.cacheSimulateInfo = {}
	end
	local newCache = {}
	newCache.positions = {}
	self.cacheSimulateInfo[startSimulateFrame] = newCache
	print("球开始模拟  ", self.objectIdx)
end

--在模拟中记录模拟的位置
function PlayerNodeObject:RecordSimulatePosition()
	if not self.isSimulating then
		return
	end
	local curCache = self.cacheSimulateInfo[self.startSimulateFrame]
	local positions = curCache.positions
	table.insert(positions, ccp(self.curX, self.curY))
	print("球插入模拟数据  ", self.objectIdx, self.curX, self.curY)
end

function PlayerNodeObject:NodeStopSimulate(endSimulateFrame)
	-- self:RecordSimulatePosition()
	self.endSimulateFrame = endSimulateFrame
	self.isSimulating = false
	self.endSimulateX = self.curX
	self.endSimulateY = self.curY
	local curCache = self.cacheSimulateInfo[self.startSimulateFrame]
	curCache.endFrame = endSimulateFrame
	print("球结束模拟  ", self.objectIdx)
end

function PlayerNodeObject:GetSimulatePosition(frame)
	for startSimulateFrame, caches in pairs(self.cacheSimulateInfo or {}) do
		if frame > startSimulateFrame and frame < caches.endFrame then
			local delta = frame - startSimulateFrame
			local position = caches.positions[delta]
			local finalPosition = caches.positions[#caches.positions]
			local deltaX = finalPosition.x - position.x
			local deltaY = finalPosition.y - position.y
			return deltaX, deltaY
		end
	end
end