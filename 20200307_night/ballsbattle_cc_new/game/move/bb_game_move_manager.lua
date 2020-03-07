BBGameMoveManager = CreateClass()

function BBGameMoveManager:__init__()
	self:ResetManager()
end

function BBGameMoveManager:ResetManager()
	self.isSplit = false
	self.isShoot = false
	self.angle = 0
	self.pressure = 0
	self.nextIdx = 0
	self.checkSum = 0
end

function BBGameMoveManager:ResetFrame()
	self.isShoot = false
	self.isSplit = false
end

function BBGameMoveManager:IncreaseFrame()
	self.nextIdx = self.nextIdx + 1
end

function BBGameMoveManager:SetMove(angle, pressure)
	self.angle = angle
	self.pressure = pressure
end

function BBGameMoveManager:GetAngle()
	return self.angle
end

function BBGameMoveManager:Split()
	self.isSplit = true
end

function BBGameMoveManager:IsSplit()
	return self.isSplit
end

function BBGameMoveManager:Shoot()
	self.isShoot = true
end

function BBGameMoveManager:IsShoot()
	return self.isShoot
end

function BBGameMoveManager:GetPressure()
	return self.pressure
end

function BBGameMoveManager:GetNextIndex()
	return self.nextIdx
end

function BBGameMoveManager:SetCheckSum(checkSum)
	self.checkSum = checkSum
end

function BBGameMoveManager:GetCheckSum()
	return self.checkSum
end