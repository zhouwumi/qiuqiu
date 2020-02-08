local BBBaseObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_base_object').BBBaseObject
PlayerNodeObject, Super = CreateClass(BBBaseObject)

local BBOtherNodeMoveComponent = import('logic.dialog.ballsbattle_cc_new.component.move.bb_player_other_node_move_component').BBOtherNodeMoveComponent
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
	x = x / 1000
	y = y / 1000
	self.curX = x
	self.curY = y
	self:_updateMass(mass)
	self.displayObject:SetPosition(x, y)
end

function PlayerNodeObject:Update()
	local playerId, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed = self._mainPanel.gameManager:GetPlayerNodeInfo(self.objectIdx)
	x = x / 1000
	y = y / 1000
	
	local renderX = self._mainPanel.gameManager:GetPlayerNodeRenderX(self.objectIdx)
	renderX = renderX / 1000
	local renderY = self._mainPanel.gameManager:GetPlayerNodeRenderY(self.objectIdx)
	renderY = renderY / 1000
	self.curX = renderX
	self.curY = renderY

	self:_updateMass(mass)
	local lastX, lastY = self.displayObject:GetPosition()
	print(x, y, mass, renderX, renderY, lastX, lastY)
	if self._mainPanel.isKeyFrame then
		self.displayObject:SetPosition(renderX, renderY)
	else
		self.displayObject:SetPosition((renderX + lastX) / 2, (renderY + lastY) / 2)
	end
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