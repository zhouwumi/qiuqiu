local BBBaseObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_base_object').BBBaseObject
SporeObject = CreateClass(BBBaseObject)

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils

function SporeObject:__init__(mainPanel)
	self._mainPanel = mainPanel
	local node = g_uisystem.create_item(mainPanel.sporeTemplate)
	self.displayObject = node
	self._mainPanel.sporeContainer:AddChild(nil, node)
	self.isDestory = false
	node:setVisible(false)
	self.gridMap = {}
	self.mass = 10
	self:caculateRadius()
	self.displayObject:setScale(self.radius / 16)
end

function SporeObject:OnJoinGame(idx)
	self.isDestory = false
	self.isMoveEnd = false
	self.objectIdx = idx
	self:_updatePosition(true)
end

function SporeObject:Update()
	if self.isMoveEnd then
		return
	end
	self:_updatePosition()
end

function SporeObject:_updatePosition(isJump)
	local renderX,renderY = self._mainPanel.gameManager:GetSporeRenderPos(self.objectIdx)
	renderX = renderX / 1000
	renderY = renderY / 1000 
	local lastX, lastY = self.displayObject:GetPosition()
	if self._mainPanel.isKeyFrame or isJump then
		self.displayObject:SetPosition(renderX, renderY)
		if self.lastRenderX and self.lastRenderY then
			local deltaX = math.abs(self.lastRenderX - renderX)
			local deltaY = math.abs(self.lastRenderY - renderY)
			if deltaX < 1 and deltaY < 1 then
				self.isMoveEnd = true
				self._mainPanel.spikySporeManager:ObjectPositionChange(self)
			end
		end
		self.lastRenderX = renderX
		self.lastRenderY = renderY
	else
		self.displayObject:SetPosition((renderX + lastX) / 2, (renderY + lastY) / 2)
	end
end

function SporeObject:Remove()
	self.isDestory = true
	-- self.displayObject:removeFromParent()
	self._mainPanel.spikySporeManager:RemoveSpikySporeObject(self)
	self._mainPanel.spikySporeManager:RemoveSporeFromPool(self)
	self.gridMap = {}
	self.displayObject:setVisible(false)
end