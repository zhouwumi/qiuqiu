local BBGameMathUtils = import('logic.dialog.ballsbattle_cc_new.game.math.bb_game_math_utils').BBGameMathUtils
local BBGameBallDeltaData = import('logic.dialog.ballsbattle_cc_new.game.data.bb_game_frame_data').BBGameBallDeltaData
local BBGameMoveCommandList = import('logic.dialog.ballsbattle_cc_new.game.move.bb_game_move_command').BBGameMoveCommandList
local BBGameMoveCommand = import('logic.dialog.ballsbattle_cc_new.game.move.bb_game_move_command').BBGameMoveCommand
local BBGamePredictionData = import('logic.dialog.ballsbattle_cc_new.game.data.bb_game_prediction_data').BBGamePredictionData
local BBGameSimplePrediction = import('logic.dialog.ballsbattle_cc_new.game.data.bb_game_prediction_data').BBGameSimplePrediction
local BBGameRect = import('logic.dialog.ballsbattle_cc_new.game.base.bb_game_rect').BBGameRect
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

local local_bb_int_to_float = BBGameMathUtils.bb_int_to_float

BBGamePlayerObject = CreateClass()

function BBGamePlayerObject:__init__()
	self.mCommandList = BBGameMoveCommandList:New()
	self.gameManager = nil
	self.vecPlayerNodes = {}
	self.uid = 0
	self.Stopped = true
	self.lastMoveCommandIdx = -1
	self.NMass = 0
	self.isCatchingUp = false
	self.isShoot = false
	self.isSplit = false
	self.direction = {x = 0, y = 0}
	self.FinalPoint = {x = 0, y = 0}
	self.predictionData = BBGamePredictionData:New()
	self.predictionCommand = BBGameMoveCommand:New()
	self._mainPanel = g_panel_mgr.get_panel('ballsbattle_cc_new.dlg_ballsbattle_cc_main_panel')
	self.groupRect = BBGameRect:New()
	self._is_node_dirty = true
	self.__crc_table = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
end

function BBGamePlayerObject:GetCrc()
	for idx, playerNode in ipairs(self.vecPlayerNodes or {}) do
		self.__crc_table[idx] = playerNode:GetCrc()
	end
	return BBGameMathUtils.GetCRC32(self.__crc_table, #self.vecPlayerNodes)
end

function BBGamePlayerObject:IsMe()
	return self.uid == g_user_info.get_user_info().uid
end

function BBGamePlayerObject:ResetCommands()
	self.lastMoveCommandIdx = -1
	self.mCommandList.mCommandVec = {}
	self.predictionData:ClearData()
end

function BBGamePlayerObject:IsNeedSyncState()
	if self:IsMe() then
		return false
	end
	return not self.predictionData.mPredictionRight
end

function BBGamePlayerObject:CanSkipSyncState()
	if self:IsMe() then
		return true
	end
	return self.predictionData.canSkipSync
end

function BBGamePlayerObject:UpdateFinalPoint(x, y)
	self.FinalPoint.x = BBGameMathUtils.bb_fix_float(x)
	self.FinalPoint.y = BBGameMathUtils.bb_fix_float(y)
end

function BBGamePlayerObject:GetFinalPointX()
	return BBGameMathUtils.bb_float_to_int(self.FinalPoint.x)
end

function BBGamePlayerObject:GetFinalPointY()
	return BBGameMathUtils.bb_float_to_int(self.FinalPoint.y)
end

function BBGamePlayerObject:GetGroupRect()
	if self._is_node_dirty then
		self:CalcGroupRect()
		self._is_node_dirty = false
	end
	return self.groupRect
end

function BBGamePlayerObject:CalcGroupRect()
	if #self.vecPlayerNodes == 0 then
		self.groupRect:SetZero()
		return
	end
	local ret = self.groupRect
	local node1 = self.vecPlayerNodes[1]
	ret:Copy(node1.rect)
	for i = 2, #self.vecPlayerNodes do
		local node = self.vecPlayerNodes[i]
		local stepRect = node.rect
		if ret.minX > stepRect.minX then
			ret.minX = stepRect.minX;
		end
		if ret.maxX < stepRect.maxX then
			ret.maxX = stepRect.maxX;
		end
		if ret.minY > stepRect.minY then
			ret.minY = stepRect.minY;
		end
		if ret.maxY < stepRect.maxY then
			ret.maxY = stepRect.maxY
		end
	end
	ret:UpdateCenter()
end

function BBGamePlayerObject:AddPlayerNode(node)
	node.player = self
	table.insert(self.vecPlayerNodes, node)
	table.sort(self.vecPlayerNodes, function(a, b)
		return a.idx < b.idx
	end)
	if self.Stopped and #self.vecPlayerNodes == 1 then
		local node1 = self.vecPlayerNodes[1]
		self:UpdateFinalPoint(node1.location.x, node1.location.y)
	end
	self._is_node_dirty = true
end

function BBGamePlayerObject:RemovePlayerNode(node)
	for i, playerNode in ipairs(self.vecPlayerNodes or {}) do
		if playerNode == node then
			self._is_node_dirty = true
			table.remove(self.vecPlayerNodes, i)
			return
		end
	end
end

function BBGamePlayerObject:GetPlayerNode(idx)
	for _, node in ipairs(self.vecPlayerNodes or {}) do
		if node.idx == idx then
			return node
		end
	end
end

function BBGamePlayerObject:RemoveMass()
	if #self.vecPlayerNodes == 0 then
		return
	end
	local maxMass = -1;
	local maxMassIdx = -1;
	local maxPlayerNode = nil;
	for i = 1, #self.vecPlayerNodes do
		local node = self.vecPlayerNodes[i];
		local mass = node:GetBallMass();
		if maxMass < mass then
			maxMass = mass;
			maxMassIdx = node.idx;
			maxPlayerNode = node;
		elseif maxMass == mass and node.idx < maxMassIdx then
			maxMassIdx = node.idx;
			maxPlayerNode = node;
		end
	end
	local massToRemove = maxMass;
	local delta = math.floor((massToRemove + self.NMass) / 1000)
	if delta > 0 then
		self.NMass = (massToRemove + self.NMass) % 1000;
		if maxMass - delta >= constant_ballsbattle_cc.BBConfigManager.initBallMass then
			maxPlayerNode:ChangeDeltaMass(-delta);
		end
	else
		self.NMass = self.NMass + massToRemove;
	end
end

function BBGamePlayerObject:_clientUpdateHandleMe()
	local commands = self.mCommandList.mCommandVec or {}
	local cnt = #commands
	if cnt <= 0 then
		return
	end
	local command = commands[cnt];
	if command.idx > self.lastMoveCommandIdx then
		self:processTick(command);
		self.lastMoveCommandIdx = command.idx;
	end
end

function BBGamePlayerObject:_clientUpdateHandleOther()
	self.predictionData.mPredictionRight = true;
	self.predictionData.canSkipSync = true;
	local commands = self.mCommandList.mCommandVec or {}
	local cnt = #commands
	-- print("其他玩家执行命令数量  ", cnt)
	if cnt > 0 then
		self.predictionData.lastRunGameFrame = self.gameManager:GetGameFrame();
		cnt = math.min(cnt, constant_ballsbattle_cc.BBConfigManager.maxOtherStepCommandCnt)
		if cnt > 1 then
			self.isCatchingUp = true;
		end

		for i = 1, cnt do
			if cnt > 1 and i == cnt then
				self.isCatchingUp = false
				self:CalcBallDelta()
			end

			local command = commands[i];
			self.predictionData.lastAngle = command.angle;
			self.predictionData.lastPressure = command.pressure;

			local predictionCommands = self.predictionData.predictionCommands
			if #predictionCommands > 0 then
				local firstCommand = predictionCommands[1];
				table.remove(predictionCommands, 1)
				-- predictionData.predictionCommands.pop_front();
				-- //预测结果和服务器的结果如果出现不一致的情况,需要进行一次状态同步
				if command.checkSum ~= firstCommand.crc then
					-- print("模拟的和预测的不同", command.checkSum, firstCommand.crc, command.angle, command.pressure, firstCommand.angle, firstCommand.pressure)
					-- //BBMathUtils::BBLOG("prediction not same  %u", command.checkSum);
					self.predictionData.mPredictionRight = false;
					self.predictionData.canSkipSync = false;
					self:processTick(command);
				else 
					-- print("模拟的和预测的相同", command.checkSum, firstCommand.crc)
					-- //BBMathUtils::BBLOG("prediction same  %u", command.checkSum);
					if i == cnt then
						-- //这里不能因为crc一致就放弃移动,只有最后一帧的command才需要去移动
						self:_clientOtherPrediction(true);
					end
				end
				self.gameManager.clzManager:ReturnSimplePrediction(firstCommand)
			else 
				-- //计算结果和服务器的结果如果出现不一致的情况,需要进行一次状态同步
				local crc = self:processTick(command);
				if crc ~= command.checkSum then
					self.predictionData.mPredictionRight = false;
					self.predictionData.canSkipSync = true;
					-- print("其他玩家计算不一致 ", crc, command.checkSum)
					-- //BBMathUtils::BBLOG("calc not same  %u-%u", crc, command.checkSum);
				else
					self.predictionData.mPredictionRight = true;
					-- print("其他玩家计算一致 ")
					-- //BBMathUtils::BBLOG("calc same  %u-%u", crc, command.checkSum);
				end
			end
		end
		self.mCommandList:ClearCommands(cnt);
	else
		if self.gameManager:GetGameFrame() - self.predictionData.lastRunGameFrame >= constant_ballsbattle_cc.BBConfigManager.otherTooSlowTicks then
			self.predictionData.mPredictionRight = false;
			self.predictionData.canSkipSync = true;
			self:MoveBack();
		else 
			-- print("_clientOtherPrediction")
			-- //BBMathUtils::BBLOG("_clientOtherPrediction");
			self:_clientOtherPrediction()
		end
	end
end

function BBGamePlayerObject:MoveBack()
	for _, node in ipairs(self.vecPlayerNodes or {}) do
		node:MoveBack(self.gameManager)
	end
end

function BBGamePlayerObject:_clientOtherPrediction(forceUpdate)
	if forceUpdate == nil then forceUpdate = false end
	if self.predictionData.lastAngle == 0 and self.predictionData.lastPressure == 0 then
		self.predictionCommand:ClearData()
		-- local prediction = BBGameSimplePrediction:New()
		local prediction = self.gameManager.clzManager:GetSimplePrediction()
		prediction.angle = self.predictionData.lastAngle
		prediction.pressure = self.predictionData.lastPressure

		local crc = self:processTick(self.predictionCommand)
		prediction.crc = crc
		-- print("计算预测的crc结果2  ", crc)
		table.insert(self.predictionData.predictionCommands, prediction)
		return
	end
	-- //BBMathUtils::BBLOG("start prediction temp %i", predictionData.predictionCommands.size());
	if #self.predictionData.predictionCommands >= constant_ballsbattle_cc.BBConfigManager.maxPredictionCnt and not forceUpdate then
		return
	end

	-- //BBMathUtils::BBLOG("start prediction %i - %i ", predictionData.lastAngle, predictionData.lastPressure);
	self.predictionCommand:ClearData();
	self.predictionCommand.angle = self.predictionData.lastAngle;
	self.predictionCommand.pressure = self.predictionData.lastPressure;

	local prediction = self.gameManager.clzManager:GetSimplePrediction()
	prediction.angle = self.predictionData.lastAngle;
	prediction.pressure = self.predictionData.lastPressure;

	local crc = self:processTick(self.predictionCommand);
	prediction.crc = crc;
	table.insert(self.predictionData.predictionCommands, prediction)
	-- print("计算预测的crc结果1  ", crc)
	-- predictionData.predictionCommands.emplace_back(prediction);
end

function BBGamePlayerObject:_clientUpdate()
	if self:IsMe() then
		self:_clientUpdateHandleMe()
	else
		self:_clientUpdateHandleOther()
	end
end

function BBGamePlayerObject:Update()
	self.mCommandList.mControlMismatch = false;
	self:_clientUpdate()
end

function BBGamePlayerObject:Move()
	if #self.vecPlayerNodes == 0 then
		self.direction.x = 0
		self.direction.y = 0
		self.Stopped = true
		return
	end
	for _, node in ipairs(self.vecPlayerNodes or {}) do
		node:Move(self.gameManager)
	end
end

function BBGamePlayerObject:_tryUpdateFinalPoint()
	if not self.Stopped then
		local rect = self:GetGroupRect()
		local w = rect.maxX - rect.minX
		local h = rect.maxY - rect.minY
		local halfLen = math.floor(math.sqrt(w * w + h * h) / 2)
		local baseLen = halfLen * constant_ballsbattle_cc.BBConfigManager.ballMoveRate

		baseLen = math.max(baseLen, constant_ballsbattle_cc.BBConfigManager.fixLength)

		-- //BBMathUtils::BBLOG("rect info: %i-%i-%i ", w, h, halfLen);
		local extLen = baseLen * BBGameMathUtils.PressureToPercent(self.direction.y);
		local targetVecX, targetVecY = BBGameMathUtils.AngleToFixedVector(math.floor(self.direction.x / 10.0), extLen);
		-- print('_tryUpdateFinalPoint  ', rect.centerX,  rect.centerY, targetVecX, targetVecY, extLen, self.direction.x)
		if targetVecX == 0 and targetVecY == 0 and #self.vecPlayerNodes == 1 then
			for _, node in ipairs(self.vecPlayerNodes or {}) do
				node.currentSpeedVec.x = 0
				node.currentSpeedVec.y = 0
				self:UpdateFinalPoint(node.location.x, node.location.y)
			end
		else
			self:UpdateFinalPoint(rect.centerX + targetVecX, rect.centerY + targetVecY);
		end
	end

	-- //一定要在这里,不能放在上面,否则停不下来，因为放在上面finalPoint就不会更新了.
	if self.direction.y == 0 then
		self.Stopped = true
	else
		self.Stopped = false
	end
end

function BBGamePlayerObject:_changeInitEndNodeSpeed()
	for _, node in ipairs(self.vecPlayerNodes or {}) do
		if node.initStopFrame == 0 then
			node:SetSpeedVec(self.FinalPoint.x - node.location.x, self.FinalPoint.y - node.location.y)
			-- print("change vec ", node.currentSpeedVec.x, node.currentSpeedVec.y)
		end
	end
end

function BBGamePlayerObject:_updateNodeTime()
	for _, node in ipairs(self.vecPlayerNodes or {}) do
		if node.initStopFrame > 0 then
			node.initStopFrame = node.initStopFrame - 1
		end
		if node.cd > 0 then
			node.cd = node.cd - 1
		end
		if node.protect > 0 then
			node.protect = node.protect - 1
		end
	end
end


function BBGamePlayerObject:processTick(command)
	-- print("BBGamePlayerObject processTick")
	self.direction.x = command.angle;
	self.direction.y = command.pressure;
	self:RemoveMass();
	self:_tryUpdateFinalPoint();
	-- print("final point is  ", self.FinalPoint.x, self.FinalPoint.y)
	self:_changeInitEndNodeSpeed();
	self:_updateNodeTime();
	local time1 = utils_get_tick()
	self:Move();
	local time2 = utils_get_tick()
	
	local crc = self:GetCrc();
	local time3 = utils_get_tick()
	self._mainPanel.frameManager:CacheTime(time2 - time1, time3 - time2)
	if self:IsMe() then
		self.gameManager.moveManager:SetCheckSum(crc)
	end
	self._is_node_dirty = true
	return crc;
end

function BBGamePlayerObject:ClientSyncServerPlayerInfo(nMass, directionX, directionY, ackCommand, finalPointX, finalPointY, Stopped)
	self.NMass = nMass;
	self.Stopped = Stopped;
	self.FinalPoint.x = local_bb_int_to_float(finalPointX)
	self.FinalPoint.y = local_bb_int_to_float(finalPointY)
	if not self:IsMe() then
		-- //同步完了状态就没必要保存预测的数据
		self.direction.x = directionX;
		self.direction.y = directionY;
		self.predictionData:ClearData();
		self.predictionData.lastAngle = directionX;
		self.predictionData.lastPressure = directionY;
		-- /*int commandCnt = mCommandList.mCommandVec.size();
		-- if (commandCnt > 0)
		-- {
		-- 	BBMoveCommand& lastCommand = mCommandList.mCommandVec[commandCnt - 1];
		-- 	predictionData.lastAngle = lastCommand.angle;
		-- 	predictionData.lastPressure = lastCommand.pressure;
		-- }*/
		self.mCommandList.mCommandVec = {};
		-- //gameManager->RemovePlayerCommand(uid);
	else
		self.mCommandList:ClientSyncServerCommandAck(ackCommand);
	end
end

function BBGamePlayerObject:CalcBallDelta()
	for _, node in ipairs(self.vecPlayerNodes or {}) do
		node:CalcBallDelta()
	end
end

function BBGamePlayerObject:clientCatchup()
	if not self:IsMe() then
		return
	end
	-- print("clientCatchup ", #self.mCommandList.mCommandVec)
	self.isCatchingUp = true;
	local commands = self.mCommandList.mCommandVec or {}
	local maxCnt = #commands
	for index = 1, maxCnt do
		local command = commands[index]
		self:processTick(command)
	end
	self.isCatchingUp = false;
end