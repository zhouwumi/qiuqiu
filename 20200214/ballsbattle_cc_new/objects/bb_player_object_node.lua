local BBBaseObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_base_object').BBBaseObject
PlayerNodeObject, Super = CreateClass(BBBaseObject)

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
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
end

function PlayerNodeObject:SetBirthInfo(x, y, mass)
	local x, y, mass, cd, protect = self._mainPanel.gameManager:GetPlayerSimpleNodeInfo(self.objectIdx)
	self.protect = protect
	if protect > 0 then
		self.displayObject.spProtect:setVisible(true)
	else
		self.displayObject.spProtect:setVisible(false)
	end
	
	local renderX, renderY = self._mainPanel.gameManager:GetPlayerNodeRenderPos(self.objectIdx)
	renderX = renderX / 1000
	renderY = renderY / 1000
	self:_updateMass(mass)
	self.displayObject:SetPosition(renderX, renderY)

	local globalPoint = self._mainPanel.playerContainer:ConvertToWorldSpace(x / 1000, y / 1000)
	local localPoint = self.displayObject.blue:getParent():ConvertToNodeSpace(globalPoint.x, globalPoint.y)
	self.displayObject.blue:SetPosition(localPoint.x, localPoint.y)
end

function PlayerNodeObject:Update()
	local x, y, mass, cd, protect = self._mainPanel.gameManager:GetPlayerSimpleNodeInfo(self.objectIdx)
	-- print("Update  ", protect, self.objectIdx)
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

	self:_updateMass(mass)
	local lastX, lastY = self.displayObject:GetPosition()
	-- if math.abs(lastX - renderX) < 0.1 and math.abs(lastY - renderY) < 0.1 then
	-- 	return
	-- end
	local distance = 0
	if self.lastRenderX ~= nil then
		distance =  math.sqrt((self.lastRenderX - renderX) * (self.lastRenderX - renderX) + (self.lastRenderY - renderY) * (self.lastRenderY - renderY))
	end
	if not self.isMe then
		print(self.objectIdx, self._mainPanel.frame, distance, x, y, mass, renderX, renderY, lastX, lastY)
	end
	
	-- self.displayObject.labelIdx:SetString(x..":"..y)
	if self._mainPanel.isKeyFrame then
		self.lastRenderX = renderX
		self.lastRenderY = renderY
		self.displayObject:SetPosition(renderX, renderY)
	else
		self.displayObject:SetPosition((renderX + lastX) / 2, (renderY + lastY) / 2)
	end
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