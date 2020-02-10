local BBBaseObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_base_object').BBBaseObject
SporeObject = CreateClass(BBBaseObject)

function SporeObject:__init__(mainPanel)
	self._mainPanel = mainPanel
	self._bbGameManager = mainPanel.gameManager

	local node = g_uisystem.create_item(mainPanel.sporeTemplate)
	self.displayObject = node
	self._mainPanel.sporeContainer:AddChild(nil, node)

	self.isDestory = false
	node:setVisible(true)
	self.gridMap = {}
	self.mass = 10
	self:caculateRadius()
end

function SporeObject:SetBirthInfo(x, y, idx)
	x = x / 1000
	y = y / 1000
	-- print("孢子位置1  ", x, y)
	self.displayObject:SetPosition(x, y)
    self.displayObject:setScale(self.radius / 16)
    self.displayObject.nodeIdx = idx
    self.objectIdx = idx
    self.curX = x
    self.curY = y
end

function SporeObject:Update()
	local renderX = self._mainPanel.gameManager:GetSporeRenderX(self.objectIdx)
	renderX = renderX / 1000
	local renderY = self._mainPanel.gameManager:GetSporeRenderY(self.objectIdx)
	renderY = renderY / 1000 
	print("孢子位置  ", self.objectIdx, renderX, renderY)
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