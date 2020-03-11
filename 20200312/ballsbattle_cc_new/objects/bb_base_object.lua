BBBaseObject = CreateClass()

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils

function BBBaseObject:__init__()
end

function BBBaseObject:changeZOrder()
	if not self.mass then
		return
	end
    self.displayObject:setLocalZOrder(self.mass + 100)
end

function BBBaseObject:ChangeMass(newMass)
end

function BBBaseObject:caculateRadius()
    self.radius = 4 + math.sqrt(self.mass) * 6
    return self.radius
end

function BBBaseObject:changeScale()
    self.displayObject:setScale(self.radius / self._spriteWidth)
end

function BBBaseObject:GetDisplayObject()
	return self.displayObject
end

function BBBaseObject:GetDisplayPosition()
	return self.displayObject:GetPosition()
end

function BBBaseObject:Show()
	self.displayObject:setVisible(true)
end

function BBBaseObject:Hide()
	self.displayObject:setVisible(false)
end

function BBBaseObject:IsValid()
	if tolua_is_obj(self.displayObject) then
		return true
	end
	return false
end

function BBBaseObject:_isPointInVisible(x, y)
	local minLocalPoint, maxLocalPoint = self._mainPanel.bgComponent:GetVisibleRect()
	local selfMinX = x - self.radius
	local selfMaxX = x + self.radius
	local selfMinY = y - self.radius
	local selfMaxY = y + self.radius
	if selfMinX > maxLocalPoint.x or selfMaxX < minLocalPoint.x or selfMinY > maxLocalPoint.y or selfMaxY < minLocalPoint.y then
		return false
	end
	return true
end