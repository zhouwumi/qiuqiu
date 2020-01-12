SporeObject = CreateClass()

function SporeObject:__init__(mainPanel)
	self._mainPanel = mainPanel
	self._bbGameManager = mainPanel.gameManager

	local node = g_uisystem.create_item(mainPanel.sporeTemplate)
	self.displayObject = node
	self._mainPanel.sporeContainer:AddChild(nil, node)

	self.isDestory = false
	node:setVisible(false)
	self.gridMap = {}
end

function SporeObject:SetPosition(x, y, radius, idx)
	self.displayObject:SetPosition(x, y)
    self.displayObject:setScale(radius / 16)
    
    self.displayObject.nodeIdx = idx

    self.objectIdx = idx
    self.radius = radius
    self.positionX = x
    self.positionY = y
end

function SporeObject:Remove()
	self.isDestory = true
	self.displayObject:removeFromParent()
	self._mainPanel.spikySporeManager:RemoveSpikyObject(self)
end