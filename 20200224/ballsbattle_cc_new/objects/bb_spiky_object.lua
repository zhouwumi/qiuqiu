local BBBaseObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_base_object').BBBaseObject
SpikyObject = CreateClass(BBBaseObject)

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
function SpikyObject:__init__(mainPanel)
	self._mainPanel = mainPanel

	local node = g_uisystem.create_item(mainPanel.spikyTemplate)
	self.displayObject = node
	node:setVisible(false)
    self.gridMap = {}
    self._mainPanel.spikyContainer:AddChild(nil, self.displayObject)
end

function SpikyObject:OnJoinGame(x, y, mass, idx)
    self.isDestory = false
    self.gridMap = {}
	self.displayObject:SetPosition(x, y)
    self.mass = mass
    self:caculateRadius()
    self.displayObject:setScale(self.radius / 50)
    self.displayObject.mask.OnClick = function()
        self.displayObject.label:setVisible(not self.displayObject.label:isVisible())
    end
    self.displayObject.label:SetString(tostring(idx))
    self.objectIdx = idx
end

function SpikyObject:Remove()
	self.isDestory = true
	-- self.displayObject:removeFromParent()
    self.displayObject:setVisible(false)
	self._mainPanel.spikySporeManager:RemoveSpikySporeObject(self)
    self._mainPanel.spikySporeManager:RemoveSpikyFromPool(self)
    self.gridMap = {}
end