local BBBaseObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_base_object').BBBaseObject
BBFakeBallObject, Super = CreateClass(BBBaseObject)

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')
local TAG = 10000

function BBFakeBallObject:__init__(mainPanel, idx)
	self._mainPanel = mainPanel
	local node = g_uisystem.create_item(mainPanel.ballFakeTemplate)
	self.displayObject = node
	mainPanel.playerContainer:AddChild(nil, node)

	self.isDestory = false

	self._spriteWidth = 50
	self.objectIdx = idx
	self.displayObject.labelIdx:SetString(tostring(idx))
	self.displayObject:setVisible(constant_ballsbattle_cc.ShowShadow)
end


function BBFakeBallObject:ChangeInfo(x, y, mass)
	local renderX = x / 1000
	local renderY = y / 1000
	self:_updateMass(mass)
	self.displayObject:SetPosition(renderX, renderY)
end

function BBFakeBallObject:Remove()
	self.isDestory = true
	self.displayObject:removeFromParent()
end

function BBFakeBallObject:_updateMass(mass)
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

function BBFakeBallObject:changeScale(delay)
    if delay and delay > 0 then
        self.displayObject:stopActionByTag(TAG)
        local scaleTo = cc.ScaleTo:create(delay, self.radius / self._spriteWidth)
        scaleTo:setTag(TAG)
        self.displayObject:runAction(scaleTo)
    else
        Super.changeScale(self)
    end
end