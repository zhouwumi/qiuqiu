SpikyObject = CreateClass()

function SpikyObject:__init__(mainPanel)
	self._mainPanel = mainPanel
	self._bbGameManager = mainPanel.gameManager

	local node = g_uisystem.create_item(mainPanel.spikyTemplate)
	self.displayObject = node

	self.isDestory = false
	node:setVisible(false)
    self.gridMap = {}
end

function SpikyObject:SetPosition(x, y, radius, idx)
	self.displayObject:SetPosition(x, y)
    self.displayObject:setScale(radius / 50)
    self._mainPanel.spikyContainer:AddChild(nil, self.displayObject)
    self.displayObject.nodeIdx = idx
    self.displayObject.mask.OnClick = function()
        print("cick spliky ", idx, radius)
        self.displayObject.label:setVisible(not self.displayObject.label:isVisible())
    end
    self.displayObject.label:SetString(tostring(idx))

    self.objectIdx = idx
    self.radius = radius
    self.positionX = x
    self.positionY = y
end

function SpikyObject:Remove()
	self.isDestory = true
	self.displayObject:removeFromParent()
	self._mainPanel.spikySporeManager:RemoveSpikyObject(self)
end