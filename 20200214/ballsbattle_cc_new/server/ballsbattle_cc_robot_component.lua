BBCCRobotComponent = CreateClass()

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBCCRobotComponent:__init__(mainPanel, roboxIdx)
	self._mainPanel = mainPanel
	local randomTime = math.random() * 1
	self._serverComponent = self._mainPanel.serverComponent
	self.frame = 0
	self.lastChangeDirection = 0
	self.uid = g_user_info.get_user_info().uid + roboxIdx
	self.nextIdx = 0
	self.curAngle = 0
	self.curPressure = 200
	self.times = 0
	self._mainPanel:get_layer():DelayCall(randomTime, function()
		self._serverComponent:JoinPlayer(self.uid)
		delay_call(1 / 15, function()
			return self:RunLoop()
		end)
	end)
end

function BBCCRobotComponent:RunLoop()
	if not self._mainPanel:is_valid() then
		return
	end
	self.frame = self.frame + 1
	self.nextIdx = self.nextIdx + 1

	if self.frame - self.lastChangeDirection > 100 then
		self.times = self.times + 1
		local changeDirection = math.random() < 0.1
		-- if changeDirection then
			self.lastChangeDirection = self.frame
			local newAngle = math.ceil(math.random() * 360) * 10
			local newPressure = math.ceil(math.random() * 15) * 100
			self.curAngle = newAngle
			self.curPressure = newPressure
		-- end
		if self.times % 5 == 0 then
			self.curAngle = 0
			self.curPressure = 0
			message("暂停了")
		end
		-- if self.frame > 1000 then
		-- 	self.curAngle = 0
		-- 	self.curPressure = 0
		-- 	if not self.hasMsg then
		-- 		self.hasMsg = true
		-- 		message("停止了")
		-- 	end
		-- end
		-- print("机器人发送命令  ", self.uid)
		
	end
	if self.frame < 30 then
		self.curAngle = 0
		self.curPressure = 0
	end

	delay_call(constant_ballsbattle_cc.RobotDelay / 1000, function()
		if not self._mainPanel:is_valid() then
			return
		end
		self._mainPanel.serverComponent:AddPlayerCommand(self.uid, self.curAngle, self.curPressure, false, false, self.nextIdx, 0)
	end)
	
	-- if self.frame == 75 then
	-- 	self.curAngle = 0
	-- 	self.curPressure = 0
	-- 	print("改变方向")
	-- elseif self.frame == 500 then
	-- 	self.curAngle = 180
	-- 	self.curPressure = 200
	-- 	print("改变方向")
	-- elseif self.frame == 600 then
	-- 	self.curAngle = 0
	-- 	self.curPressure = 0
	-- 	print("改变方向")
	-- elseif self.frame == 1000 then
	-- 	self.curAngle = 0
	-- 	self.curPressure = 200
	-- 	self.frame = 0
	-- 	print("改变方向")
	-- end
	return 1 / 15
end