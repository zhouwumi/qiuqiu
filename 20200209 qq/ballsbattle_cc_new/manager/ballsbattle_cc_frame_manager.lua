BBCCFrameManager = CreateClass()

local SporeObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_spore_object').SporeObject

function BBCCFrameManager:__init__(mainPanel)
	self._mainPanel = mainPanel
	self._gameManager = mainPanel.gameManager
	self.frameDatas = {}
	self.currentServerFrame = 1
	self.simulateFrame = -1
	self.stepFrame = 0

	self.timeMove = 0
	self.timeEat = 0
end

function BBCCFrameManager:OnGetFirstState(data)
	self.currentServerFrame = data.frame
	self._mainPanel.gameManager:SetGameFrame(self.currentServerFrame)
	print("BBCCFrameManager 初始化帧  ", self.currentServerFrame)
end

function BBCCFrameManager:AddServerFrame(frameData)
	table.insert(self.frameDatas, frameData)
	print("添加帧数据", #self.frameDatas,frameData)
end

--去除模拟版本
function BBCCFrameManager:OnUpdate()
	if self.stepFrame < 5 then
		self.stepFrame = self.stepFrame + 1
		self._gameManager:OnFrameUpdate()
		print("消耗的时间:  ", 
			self._mainPanel.playerManager:GetPlayerNodeCount(), 
			self._gameManager:GetTimeMove(), 
			self._gameManager:GetTimeEat(), 
			self._gameManager:GetTimeHit(), 
			self._gameManager:GetUpdateCircle(),
			self._gameManager:GetMovePlayer())
		self._mainPanel.objectManager:KeyFrameDoEat()
		self._mainPanel.playerManager:UpdateAllPlayers()
		self._mainPanel.objectManager:UpdateSpore()
	end
	if self.stepFrame == 5 then
		if #self.frameDatas > 0 then
			self:DoKeyFrame()
		end
	end
end

function BBCCFrameManager:OnUpdateWithSimulate()
	local runTimes = math.min(#self.frameDatas, 3)
	runTimes = math.max(runTimes, 1)
	for index = 1, runTimes do
		self:TryStepNextFrame()
	end
	-- if self.simulateFrame >= 0 and #self.frameDatas > 0 then
	-- 	print("结束模拟")
	-- 	self._gameManager:StopSimulate(self.currentServerFrame + 5)
	-- 	self._mainPanel.labelSimulateState:SetString("结束模拟")
	-- 	self._mainPanel.labelSimulateState:DelayCall(2, function()
	-- 		self._mainPanel.labelSimulateState:SetString("")
	-- 	end)
	-- 	local lastSimulateFrame = self.simulateFrame
	-- 	self.simulateFrame = -1
	-- 	local runTimes = math.min(lastSimulateFrame, 3) --每次最多模拟3帧
	-- 	if self.stepFrame == 5 then
	-- 		lastSimulateFrame = lastSimulateFrame + 1
	-- 	end
	-- 	for index = 1, runTimes do
	-- 		self:OnUpdateWithSimulate()
	-- 		lastSimulateFrame = lastSimulateFrame - 1
	-- 	end
	-- 	if self.simulateFrame ~= -1 then --防止在中间也多模拟了
	-- 		self.simulateFrame = lastSimulateFrame
	-- 		if lastSimulateFrame == 0 then
	-- 			self.simulateFrame = -1
	-- 		end
	-- 	end
	-- 	print("连续追帧  ", runTimes, "还剩下  ", lastSimulateFrame, "当前关键帧数量: ", #self.frameDatas)
	-- 	return
	-- end
	-- self:TryStepNextFrame()
end

function BBCCFrameManager:TryStepNextFrame()
	if self.stepFrame < 5 then
		self.stepFrame = self.stepFrame + 1
		print("step frame  ", self.stepFrame)
		self._gameManager:OnFrameUpdate()
		self._mainPanel.objectManager:KeyFrameDoEat()
	end
	if self.stepFrame == 5 then
		if #self.frameDatas > 0 then
			if self.simulateFrame >= 0 then --有关键帧数据,需要一步一步撤销模拟数据
				self._gameManager:StopSimulate(self.currentServerFrame + 5)
				self._mainPanel.labelSimulateState:SetString("结束模拟")
				self._mainPanel.labelSimulateState:DelayCall(2, function()
					self._mainPanel.labelSimulateState:SetString("")
				end)
				self:BallStopSimulate(self.currentServerFrame + 5 + self.simulateFrame)
				self.simulateFrame = -1
			else
				self:DoKeyFrame()
			end
		else
			if self.simulateFrame < 0 then
				self.simulateFrame = 0
				self._mainPanel.labelSimulateState:SetString("开始模拟")
				print("开始模拟")
				self._gameManager:StartSimulate(self.currentServerFrame + 5)
				self.isSimulating = true
				self:BallStartSimulate(self.currentServerFrame + 5)
			end
			self.simulateFrame = self.simulateFrame + 1
			print("模拟step ", self.simulateFrame)
			self._gameManager:OnSimulate(self.simulateFrame % 5 == 0)
			self._mainPanel.labelSimulateState:SetString("模拟中")
		end
	end
end

--开始模拟的帧数
function BBCCFrameManager:BallStartSimulate(startSimulateFrame)
	local allPlayers = self._mainPanel.playerManager.allPlayers
	for id, playerObject in pairs(allPlayers) do
	    playerObject:PlayerStartSimulate(startSimulateFrame)
	end 
end

--结束模拟的帧数
function BBCCFrameManager:BallStopSimulate(endSimulateFrame)
	local allPlayers = self._mainPanel.playerManager.allPlayers
	for id, playerObject in pairs(allPlayers) do
	    playerObject:PlayerStopSimulate(endSimulateFrame)
	end 
end

function BBCCFrameManager:DoKeyFrame()
	self._gameManager:ClientKeyFrameUpdateBefore()
	-- self._mainPanel.playerManager:KeyFrameCreateNewPlayerNodes()
	self._mainPanel.playerManager:KeyFrameRemovePlayerNodes()
	local frameData = table.remove(self.frameDatas, 1)

	self._mainPanel.labelCurrentFrame:SetString(tostring(frameData.frame))
	-- self._gameManager:DoPlayerSplit()
	self._gameManager:ClientKeyFrameUpdate()
	--创建玩家的分裂,在这里插入玩家分裂刺球,先默认这些球全都是玩家分裂的球吧
	-- if frameData.new_nodes and #frameData.new_nodes then
	-- 	self._mainPanel.gameManager:AddOperatePlayerSplit(g_user_info.get_user_info().uid)
	-- end
	self._gameManager:DoPlayerSplit()
	self._gameManager:AddNodesCd()
	local newNodeIdxsInPlayer = self:_handleNewPlayer(frameData)
	self:_handleNewNodes(frameData, newNodeIdxsInPlayer)
	self:_handleCreateNewFoods(frameData)
	self:_handleCreateNewSpores(frameData)
	self:_handleCreateNewSpiky(frameData)
	self:_increaseKeyFrame()
	
	self.stepFrame = 0
	print("处理了一条关键帧,还有", #self.frameDatas, "条数据", frameData.frame)
	self.isKeyFrameUpdate = true
	self._gameManager:EndUpdate()

	
	self._mainPanel.playerManager:PrintPlayerInfo()
	self:_checkCrc(frameData)
	self:_handleFrameData(frameData)
	if not self._lastTimeMove then
		self._lastTimeMove = 0
	end

	if not self._lastTimeEat then
		self._lastTimeEat = 0
	end

	self._lastTimeMove = self._gameManager:GetTimeMove()
	self._lastTimeEat = self._gameManager:GetTimeEat()
	self.currentServerFrame = frameData.frame
end

function BBCCFrameManager:_checkCrc(frameData)
	local nativeFoodCrc = self._gameManager:GetAllFoodCrc()
	local serverFoodCrc = frameData.food_crc
	local nativeSpikyCrc = self._gameManager:GetAllSpikyCrc()
	local serverSpikyCrc = frameData.spiky_crc
	local nativeSporeCrc = self._gameManager:GetAllSporeCrc()
	local serverSporeCrc = frameData.spore_crc
	local nativePlayerCrc = self._gameManager:GetAllPlayerCrc()
	local serverPlayerCrc = frameData.player_crc

	print("native food crc:  "..nativeFoodCrc.."  server food crc:  "..serverFoodCrc)
	print("native spiky crc:  "..nativeSpikyCrc.."  server spiky crc:  "..serverSpikyCrc)
	print("native spore crc:  "..nativeSporeCrc.."  server spore crc:  "..serverSporeCrc)
	print("native player crc: "..nativePlayerCrc.." server player crc: "..serverPlayerCrc)

	
	if nativeFoodCrc ~= serverFoodCrc then
		local msg = "native food crc:  "..nativeFoodCrc.."  server food crc:  "..serverFoodCrc
		__G__TRACKBACK__('food crc 不一致 '..msg)
	end
	

	
	if nativeSpikyCrc ~= serverSpikyCrc then
		local msg = "native spiky crc:  "..nativeSpikyCrc.."  server spiky crc:  "..serverSpikyCrc
		__G__TRACKBACK__('spiky crc 不一致 '..msg)
	end
	
	

	
	if nativeSporeCrc ~= serverSporeCrc then
		local msg = "native spore crc:  "..nativeSporeCrc.."  server spore crc:  "..serverSporeCrc
		__G__TRACKBACK__('spore crc 不一致 '..msg)
	end
	
	
	if nativePlayerCrc ~= serverPlayerCrc then
		local msg = "native player crc:  "..nativePlayerCrc.."  server player crc:  "..serverPlayerCrc
		__G__TRACKBACK__('player crc 不一致 '..msg)
	end

	
end


function BBCCFrameManager:QuickHandleFrame()
	
end

function BBCCFrameManager:_handleFrameData(frameData)
	local commands = frameData.command or {}
	for _, command in ipairs(commands) do
		local type = command[1]
		if type == 1 then
			self:_handleMoveCommand(command)
		elseif type == 2 then
			self:_handleSplitCommand(command)
		elseif type == 3 then
			self:_handleShootCommand(command)
		end
	end
end

function BBCCFrameManager:_handleNewPlayer(frameData)
	if frameData.new_player then
		local newNodesInPlayer = {}
		for _, playerInfo in ipairs(frameData.new_player) do
			self._mainPanel.objectManager:CreatePlayerWithServerInfo(playerInfo, newNodesInPlayer)
		end
		return newNodesInPlayer
	end
end

function BBCCFrameManager:_handleNewNodes(frameData, filterIdxs)
	local newNodes = frameData.new_nodes
	for _, node in ipairs(newNodes or {}) do
		if not filterIdxs or not filterIdxs[node.idx] then
			self._mainPanel.objectManager:CreateNodeWithServerInfo(node)
			print("新创建了一个玩家节点", node.idx)
		end
	end
end

function BBCCFrameManager:_handleCreateNewFoods(frameData)
	local newFoods = frameData.new_foods
	for _, food in ipairs(newFoods or {}) do
		local foodObj = self._mainPanel.objectManager:CreateFoodWithServerInfo(food)
		print('新创建了一个食物', foodObj.positionX, foodObj.positionY)
	end
end

function BBCCFrameManager:_handleCreateNewSpores(frameData)
	local newSpores = frameData.new_spore or {}
	for _, newSpore in ipairs(newSpores) do
		self._mainPanel.objectManager:GenerateNewSporeWithServerInfo(newSpore)
		print('新创建了一个孢子')
	end
end

function BBCCFrameManager:_handleCreateNewSpiky(frameData)
	local newSpikys = frameData.new_spiky or {}
	for _, newSpiky in ipairs(newSpikys) do
		self._mainPanel.objectManager:CreateSpikyWithServerInfo(newSpiky)
		print('新创建了一个刺球')
	end
end

function BBCCFrameManager:_handleMoveCommand(command)
	local uid = command[2]
	local angle = command[3]
	local pressure = command[4]
	self._gameManager:AddOperateMove(uid, angle, pressure)
	print("处理玩家移动命令", angle, pressure, command)

	--记录上一次我的移动指令
	if uid == g_user_info.get_user_info().uid then
		self.lastAngle = angle
		self.lastPressure = pressure
	end

	local playerObject = self._mainPanel.playerManager.allPlayers[uid]
	if playerObject then
		playerObject.angle = angle
		playerObject.pressure = pressure
	end
end

function BBCCFrameManager:_handleShootCommand(command)
	print("吐了孢子", command)
	local uid = command[2]
	self._gameManager:AddOperatePlayerShoot(uid)
end

function BBCCFrameManager:_handleSplitCommand(command)
	print("分裂操作")
	local uid = command[2]
	self._gameManager:AddOperatePlayerSplit(uid)
end

function BBCCFrameManager:_increaseKeyFrame()
	self.currentServerFrame = self.currentServerFrame + 5
end