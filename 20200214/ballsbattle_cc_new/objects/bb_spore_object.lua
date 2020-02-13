local BBBaseObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_base_object').BBBaseObject
SporeObject = CreateClass(BBBaseObject)

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
function SporeObject:__init__(mainPanel)
	self._mainPanel = mainPanel
	self._bbGameManager = mainPanel.gameManager

	local node = g_uisystem.create_item(mainPanel.sporeTemplate)
	self.displayObject = node
	self._mainPanel.sporeContainer:AddChild(nil, node)

	self.isDestory = false
	node:setVisible(false)
	self.gridMap = {}
	self.mass = 10
	self:caculateRadius()
end

function SporeObject:SetBirthInfo(idx)
	self.objectIdx = idx
	local renderX,renderY = self._mainPanel.gameManager:GetSporeRenderPos(self.objectIdx)
	renderX = renderX / 1000
	renderY = renderY / 1000 
	-- print("孢子位置1  ", x, y)
	self.displayObject:SetPosition(renderX, renderY)
    self.displayObject:setScale(self.radius / 16)
    self.displayObject.nodeIdx = idx
    
    self.curX = x
    self.curY = y
end

function SporeObject:Update()
	local renderX,renderY = self._mainPanel.gameManager:GetSporeRenderPos(self.objectIdx)
	renderX = renderX / 1000
	renderY = renderY / 1000 
	-- print("孢子位置  ", self.objectIdx, renderX, renderY, self.displayObject:isVisible(), self.gridX, self.gridY)
	-- if self.lastVisible ~= nil and self.displayObject:isVisible() ~= self.lastVisible then
	-- 	if self.displayObject:isVisible() then
	-- 		message("显示出来了")
	-- 		print("显示出来了")
	-- 	else
	-- 		message("隐藏了")
	-- 		print("隐藏了")
	-- 	end
	-- end
	-- self.lastVisible = self.displayObject:isVisible()
	local lastX, lastY = self.displayObject:GetPosition()
	-- print(x, y, mass, renderX, renderY, lastX, lastY)
	if self._mainPanel.isKeyFrame then
		self.displayObject:SetPosition(renderX, renderY)
	else
		self.displayObject:SetPosition((renderX + lastX) / 2, (renderY + lastY) / 2)
	end
end

function SporeObject:Remove()
	self.isDestory = true
	self.displayObject:removeFromParent()
	self._mainPanel.spikySporeManager:RemoveSpikyObject(self)
end