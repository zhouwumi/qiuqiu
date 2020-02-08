BBBaseObject = CreateClass()

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
end

function BBBaseObject:calculateSpeed()
	self.speed = math.floor(8 * 2 * math.sqrt(20 / (self.mass + 10)))
end

function BBBaseObject:changeScale()
    self.displayObject:setScale(self.radius / self._spriteWidth)
end

function BBBaseObject:GetDisplayObject()
	return self.displayObject
end

