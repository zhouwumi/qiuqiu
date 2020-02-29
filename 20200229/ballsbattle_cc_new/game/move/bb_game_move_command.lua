BBGameMoveCommand = CreateClass()

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBGameMoveCommand:__init__()
	self:ClearData()
end

function BBGameMoveCommand:ClearData()
	self.isShoot = false
	self.isSplit = false
	self.angle = 0
	self.pressure = 0
	self.checkSum = 0
	self.idx = 0	
end

function BBGameMoveCommand:IsEqual(prediction)
	if self.isShoot or self.isSplit then
		return false
	end
	return self.angle == prediction.angle and self.pressure == prediction.pressure
end

BBGameMoveCommandList = CreateClass()

function BBGameMoveCommandList:__init__()
	self.mCommandVec = {}
	self.mLastCommandAck = -1
	self.mControlMismatch = false
end

function BBGameMoveCommandList:pushCommand(command)
	if self:IsBacklogged() then
		return
	end
	table.insert(self.mCommandVec, command)
end

function BBGameMoveCommandList:IsBacklogged()
	return #self.mCommandVec >= constant_ballsbattle_cc.BBConfigManager.maxCommandNoAck
end

function BBGameMoveCommandList:ClearCommands(count)
	if count <= 0 then return end
	if count > #self.mCommandVec then return end
	if count == #self.mCommandVec then 
		self.mCommandVec = {}
		return
	end
	for index = 1, count do
		table.remove(self.mCommandVec, 1)
	end
end

function BBGameMoveCommandList:ClientSyncServerCommandAck(serverAck)
	local clearCount = 0
	for _, command in ipairs(self.mCommandVec or {}) do
		if command.idx <= serverAck then
			clearCount = clearCount + 1
		else
			break
		end
	end
	self:ClearCommands(clearCount)
end