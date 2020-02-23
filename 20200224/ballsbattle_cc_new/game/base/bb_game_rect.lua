BBGameRect = CreateClass()

function BBGameRect:__init__(minX, minY, maxX, maxY)
	if not minX then minX = 0 end
	if not minY then minY = 0 end
	if not maxX then maxX = 0 end
	if not maxY then maxY = 0 end
	self:SetRect(minX, minY, maxX, maxY)
end

function BBGameRect:SetRect(minX, minY, maxX, maxY)
	self.minX = minX
	self.minY = minY
	self.maxX = maxX
	self.maxY = maxY
	self.centerX = (minX + maxX) / 2
	self.centerY = (minY + maxY) / 2
end

function BBGameRect:Copy(rect)
	self.minX = rect.minX
	self.minY = rect.minY
	self.maxX = rect.maxX
	self.maxY = rect.maxY
	self.centerX = (rect.minX + rect.maxX) / 2
	self.centerY = (rect.minY + rect.maxY) / 2
end

function BBGameRect:UpdateCenter()
	self.centerX = (self.minX + self.maxX) / 2
	self.centerY = (self.minY + self.maxY) / 2
end

function BBGameRect:Expand(expandW, expandH)
	self.minX = self.minX - expandW
	self.minY = self.minY - expandH
	self.maxX = self.maxX + expandW
	self.maxY = self.maxY + expandH
	self.centerX = (self.minX + self.maxX) / 2
	self.centerY = (self.minY + self.maxY) / 2
end

function BBGameRect:IntersectsRect(otherRect)
	if self.maxX < otherRect.minX then
		return false
	end
	if self.minX > otherRect.maxX then
		return false
	end
	if self.maxY < otherRect.minY then
		return false
	end
	if self.minY > otherRect.maxY then
		return false
	end
	return true
end

function BBGameRect:IntersectsRectPoint(minX, maxX, minY, maxY)
	if self.maxX < minX then
		return false
	end
	if self.minX > maxX then
		return false
	end
	if self.maxY < minY then
		return false
	end
	if self.minY > maxY then
		return false
	end
	return true
end

function BBGameRect:SetZero()
	self:setRect(0, 0, 0, 0)
end

function BBGameRect:IsZero()
	return self.minX == 0 and self.minY == 0 and self.maxX == 0 and self.maxY == 0
end