BBCCRobotComponent = CreateClass()

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBCCRobotComponent:__init__(mainPanel, roboxIdx)
	self._mainPanel = mainPanel
	local randomTime = math.random() * 1
	self._serverComponent = self._mainPanel.serverComponent
	self.frame = 0
	self.lastChangeDirection = -250
	self.uid = g_user_info.get_user_info().uid + roboxIdx
	self.nextIdx = 0
	-- self.curAngle = 2700
	-- self.curPressure = 100
	self.curAngle = 0
	self.curPressure = 0
	self.times = 0
	self._mainPanel:get_layer():DelayCall(randomTime, function()
		if not self._mainPanel.playerManager:GetMyPlayerObject() then
			randomTime = math.random() * 2
			return randomTime
		end
		self._serverComponent:JoinPlayer(self.uid)
		if roboxIdx == constant_ballsbattle_cc.RobotCnt then
			message("创建完成")
		end
		local lastTime = utils_get_tick()
		delay_call(1 / 15, function()
			-- local nowTime = utils_get_tick()
			-- local times = math.floor((nowTime - lastTime) / (1 / 15))
			-- local ret = 1 / 15
			-- if times > 0 then
			-- 	print("机器人  ", times)
			-- 	for index = 1, times do
			-- 		ret = self:RunLoop()
			-- 	end
			-- 	lastTime = lastTime + times * 1 / 15
			-- end
			ret = self:RunLoop()
			return 1 / 15
		end)
	end)
end

function BBCCRobotComponent:RunLoop()
	if not self._mainPanel:is_valid() then
		return
	end
	if constant_ballsbattle_cc.DebugStopRobot then
		return
	end
	self.frame = self.frame + 1
	self.nextIdx = self.nextIdx + 1

	local isSplit = false
	local isShoot = false
	if self.frame - self.lastChangeDirection > 150 then
		self.times = self.times + 1
		-- if changeDirection then
			self.lastChangeDirection = self.frame
			local newAngle = math.ceil(math.random() * 360) * 10
			local newPressure = math.ceil(math.random() * 15) * 100
			self.curAngle = newAngle
			self.curPressure = newPressure
			if self.times == 1 then
				self.curAngle = 0
			end
			print("机器人改变了方向")
		-- end
		-- if self.times % 5 == 0 then
		-- 	self.curAngle = 0
		-- 	self.curPressure = 0
		-- 	message("暂停了")
		-- elseif self.times > 1 then
		-- 	-- message("分裂了")
		-- 	-- isSplit = true
		-- 	isShoot = true
		-- end

		-- if self.times % 10 == 0 then
			
		-- end
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
	
	-- if self.frame < 200 then
	-- 	self.curAngle = 2700
	-- 	self.curPressure = 100
	-- end

	-- isSplit = math.random() < 0.005
	-- isShoot = math.random() < 0.01


	local isShake = math.random() < 0 and self.frame > 130
	delay_call(constant_ballsbattle_cc.RobotDelay / 1000, function()
		if not self._mainPanel:is_valid() then
			return
		end
		
		self._mainPanel.serverComponent:AddPlayerCommand(self.uid, self.curAngle, self.curPressure, isSplit, isShoot, self.nextIdx, 0)
	end)
	
	local shakeDelay = 0
	if isShake then
		shakeDelay = 0
		-- message("出现了一次网络波动")
	end
	return 1 / 15 + shakeDelay / 1000
end