local BBBaseObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_base_object').BBBaseObject
SporeObject = CreateClass(BBBaseObject)

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils

function SporeObject:__init__(mainPanel)
	self._mainPanel = mainPanel
	local node = g_uisystem.create_item(mainPanel.sporeTemplate)
	self.displayObject = node
	self._mainPanel.sporeContainer:AddChild(nil, node)
	self.isDestory = false
	self.gridMap = {}
	self.mass = 10
	self:caculateRadius()
	self.displayObject:setScale(self.radius / 16)
	self.isSpore = true
end

function SporeObject:_canCancelUpdate(x, y)
	if not self.lastRenderX or not self.lastRenderY then
		return false
	end
	local minLocalPoint, maxLocalPoint = self._mainPanel.bgComponent:GetVisibleRect()
	local expandDistance = 400
	if x > maxLocalPoint.x + expandDistance or x < minLocalPoint.x - expandDistance or y > maxLocalPoint.y + expandDistance or y < minLocalPoint.y - expandDistance then
		return true
	end
	return false
end

function SporeObject:OnJoinGame(idx)
	self.isDestory = false
	self.isMoveEnd = false
	self.joinTime = self._mainPanel.frame
	self.objectIdx = idx
	self:_updatePosition(true)
	local x, y = self.displayObject:GetPosition()
	self._cancelUpdate = self:_canCancelUpdate(x, y)
	if self._cancelUpdate then
		--超过了一定的范围就可以到时候直接更新即可
		self.displayObject:DelayCall(1.2, function()
			local renderX,renderY = self._mainPanel.gameManager:GetSporeRenderPos(self.objectIdx)
			renderX = renderX  * 0.001
			renderY = renderY * 0.001
			self.displayObject:SetPosition(renderX, renderY)
			self.isMoveEnd = true
			self._mainPanel.spikySporeManager:ObjectPositionChange(self)
		end)
	end
end

function SporeObject:GetCurrentCd()
	if not self.cd then
		return 0
	end
	return self.cd - (self._mainPanel.frame - self.joinTime) / 2
end

function SporeObject:Update()
	if self.isMoveEnd then
		return
	end
	if self._cancelUpdate then
		return
	end
	self:_updatePosition()
end

function SporeObject:_updatePosition(isJump)
	local renderX,renderY = self._mainPanel.gameManager:GetSporeRenderPos(self.objectIdx)
	renderX = renderX  * 0.001
	renderY = renderY * 0.001
	local lastX, lastY = self.displayObject:GetPosition()

	if isJump or self._mainPanel.frameManager.isKeyFrame then
		self.displayObject:SetPosition(renderX, renderY)
		if self.lastRenderX == renderX and self.lastRenderY == renderY and self._mainPanel.frame - self.joinTime > 2 then
			self.isMoveEnd = true
			self._mainPanel.spikySporeManager:ObjectPositionChange(self)
		end
		self.lastRenderX = renderX
		self.lastRenderY = renderY
	else
		self.displayObject:SetPosition((renderX + lastX) / 2, (renderY + lastY) / 2)
	end
end

function SporeObject:Remove()
	self.isDestory = true
	self._mainPanel.spikySporeManager:RemoveSpikySporeObject(self)
	self._mainPanel.spikySporeManager:RemoveSporeFromPool(self)
	self.gridMap = {}
	self:Hide()
	self.displayObject:stopAllActions()
end

function SporeObject:Show()
	self.displayObject:setVisible(true)
	self._mainPanel.spikySporeManager:CacheSporeVisible(self.objectIdx, true)
end

function SporeObject:Hide()
	self.displayObject:setVisible(false)
	if self._mainPanel.spikySporeManager then
		self._mainPanel.spikySporeManager:CacheSporeVisible(self.objectIdx, false)
	end
end