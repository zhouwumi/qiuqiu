BBCCServerComponent = CreateClass()

local BBCCPlayerInfo = import('logic.dialog.ballsbattle_cc_new.data.bb_player_info').BBCCPlayerInfo
local BBCCNodeInfo = import('logic.dialog.ballsbattle_cc_new.data.bb_node_info').BBCCNodeInfo
local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
local BBCCServerToClientComponent = import('logic.dialog.ballsbattle_cc_new.server.bb_cc_server_send_to_client_component').BBCCServerToClientComponent
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBCCServerComponent:__init__(mainPanel)
	self._mainPanel = mainPanel

	self.serverToClientComponent = BBCCServerToClientComponent:New(mainPanel)

	cc.BBConfigManager:SetLogState(constant_ballsbattle_cc.IsCOpenLog)
	cc.BBConfigManager:SetDebugOtherBallMas(100)
	cc.BBConfigManager:SetDebugMyBallMass(100)
	cc.BBConfigManager:SetMinSpikyMass(40)
	cc.BBConfigManager:SetMaxSpikyMass(60)
	-- if constant_ballsbattle_cc.RobotCnt == 1 then
	-- 	cc.BBConfigManager:SetClientDebug(true)
	-- end
	-- cc.BBConfigManager:SetClientDebug(true)
	-- cc.BBConfigManager:SetSpikyCount(0)

	self.gameManager = cc.BBServerGameManager:Create()
	self.gameManager:InitRoom()
	self.gameManager:SetServerState(true)
	self.gameManager:SetCanEatFood(true)
	self.gameManager:SetCanEatSpiky(true)
	self.gameManager:SetCanEatSpore(true)
	self.gameManager:SetCanEatNode(true)
	self.gameManager:SetUserId(g_user_info.get_user_info().uid)
	self.cacheSyncStateTime = {}
	self.gameManager:InitFoodSpiky()
	local idx = 0
	local lastTime = utils_get_tick()
	delay_call(0, function()
		if not mainPanel:is_valid() then
			return
		end
		local nowTime = utils_get_tick()
		local times = math.floor((nowTime - lastTime) / (1 / 15))
		if times > 0 then
			lastTime = lastTime + times * (1 / 15)
			for index = 1, times do
				self:FrameUpdate()
			end
		end
		return 1 / 30
	end)

	self.frameCommands = {}
	self.frameRequestData = {}
	self.diePlayerIds = {}
	self.playerLuaCache = {}
end

function BBCCServerComponent:JoinPlayer(uid, useLua)
	self.gameManager:AddOperatePlayerJoin(uid)
	if useLua == nil and self.playerLuaCache[uid] == nil then
		return
	end
	self.playerLuaCache[uid] = useLua
	self.gameManager:SetPlayerUseLua(uid, useLua)
end

function BBCCServerComponent:Disconnect()
	self.isConnect = false
	self.serverToClientComponent:Stop()
	-- self.gameManager:AddPlayerQuit(u)
end

function BBCCServerComponent:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, commandIdx, checkSum)
	
	-- print("server add command  ", uid, commandIdx, checkSum)
	if isShoot then
		-- message("接受到了吐孢子指令")
		-- print('接受到了吐孢子指令')
	end
	local uidCommands = self.frameCommands[uid]
	if not uidCommands  then
		uidCommands = {}
		self.frameCommands[uid] = uidCommands
	end
	-- print("插入一个玩家命令  ", uid)
	table.insert(uidCommands, {uid, angle, pressure, isSplit, isShoot, commandIdx, checkSum})
	self.gameManager:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, commandIdx, checkSum)
end

function BBCCServerComponent:FrameUpdate()
	if not self._times then  self._times = 0 end
	self._times = self._times + 1
	self.serverToClientComponent:StartNewFrame()
	-- print("服务器开始执行 ", self._mainPanel.frame, self._times)
	local time1 = utils_get_tick()
	self.gameManager:FrameUpdate()
	local time2 = utils_get_tick()
	if not self:SyncFirstData() and self.isConnect then
		self:SyncNewFood()
		self:SyncNewSpore()
		self:SyncNewSpiky()
		self:SyncNewPlayers()
		self:SyncEatFood()
		self:SyncEatSpiky()
		self:SyncEatSpore()
		self:SyncCommands()
		self:HandleAck()
		self:UpdateFakeNodeDisplay()
	end
	

	-- self.frameCommands = {}
	self.frameRequestData = {}
	local time3 = utils_get_tick()
	-- print("服务器执行完成  ", time2 - time1, time3 - time2, self._mainPanel.frame)
	self.serverToClientComponent:EndNewFrame()

	local playerIds = self.gameManager:GetFrameDiePlayerIds()
	for _, playerId in ipairs(playerIds or {}) do
		self.diePlayerIds[playerId] = true
		self._mainPanel:get_layer():DelayCall(5, function()
			self.diePlayerIds[playerId] = false
			self:JoinPlayer(playerId, self.playerLuaCache[playerId])
		end)
	end
	self._frame_generate_main_player = false
end

function BBCCServerComponent:IsPlayerDie(uid)
	return self.diePlayerIds[uid]
end

local function _getPackSporeInfo(idx, fromId, uid, speedX, speedY, x, y, initSpeed, initDeltaSpeed, initStopFrame, cd)
	local ret = {}
	ret.idx = idx
	ret.fromId = fromId
	ret.uid = uid
	ret.speedX = speedX
	ret.speedY = speedY
	ret.x = x
	ret.y = y
	ret.initSpeed = initSpeed
	ret.initDeltaSpeed = initDeltaSpeed
	ret.initStopFrame = initStopFrame
	ret.cd = cd
	return ret
end

function BBCCServerComponent:GetSyncCrcToPlayer(mainUid, otherUid, crc, luaCrc)
	if mainUid == otherUid then
		return crc
	end
	if self.playerLuaCache[mainUid] then
		return luaCrc
	end
	return crc
end

function BBCCServerComponent:SyncCommands()
	local send_commands = {}
	for uid, uidCommands in pairs(self.frameCommands) do
        if uid ~= g_user_info.get_user_info().uid then
        	local crcCnt = self.gameManager:GetCrcCnt(uid)
        	local commands = {}
        	send_commands[uid] = commands
        	for index = 1, crcCnt do
        		local crc = self.gameManager:GetCrcIdx(uid, index - 1)
        		local luaCrc = self.gameManager:GetLuaCrcIdx(uid, index - 1)
        		local command = uidCommands[1]
        		if not command then
        			print(uid, #uidCommands, crcCnt, self.frameCommands)
        		end
        		local sendCrc = self:GetSyncCrcToPlayer(g_user_info.get_user_info().uid, uid, crc, luaCrc)
        		command[7] = sendCrc or 0
        		table.remove(uidCommands, 1)
        		table.insert(commands, command)
        	end
        end
    end
    -- print("同步其他人的操作  ", self.frameCommands)
	self.serverToClientComponent:SyncCommands(send_commands)
end

function BBCCServerComponent:UpdateFakeNodeDisplay() 
	if not constant_ballsbattle_cc.ShowShadow then
		return
	end
	local playerIds = self.gameManager:GetAllPlayerIdxs()
	for _, playerId in ipairs(playerIds or {}) do
		local playerNodeIds = self.gameManager:GetAllPlayerNodeIdxs(playerId)
		for _, playerNodeId in ipairs(playerNodeIds or {}) do
			local uid, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX, speedY = self.gameManager:GetPlayerNodeInfo(playerNodeId)
			self.serverToClientComponent:ServerChangeFakeNode(playerNodeId, x, y, mass)
		end
	end
end

function BBCCServerComponent:SyncFirstData()
	if self.isConnect then
		return
	end
	local hasFind = false
	local newPlayerIds = self.gameManager:GetFrameNewPlayer()
	for _, playerId in ipairs(newPlayerIds or {}) do
		if playerId == g_user_info.get_user_info().uid then
			hasFind = true
		end
	end
	if not hasFind then
		return false
	end
	self.isConnect = true

	local ret = {}
	local foodInfos = self.gameManager:GetAllFoodInfos()
	local spikyInfoIds = self.gameManager:GetAllSpikyInfos()
	local spikyInfos = {}
	for _, idx in ipairs(spikyInfoIds or {}) do
		local x, y, mass, raidus = self.gameManager:GetSpikyInfo(idx)
		table.insert(spikyInfos, {idx, x, y, mass, raidus})
	end
	local sporeInfos = {}
	local sporeIds = self.gameManager:GetAllSporeInfos()
	for _, sporeId in ipairs(sporeIds or {}) do
		local fromId, uid, speedX, speedY, x, y, initSpeed, initDeltaSpeed, initStopFrame, cd = self.gameManager:GetSporeInfo(sporeId)
		local sporeInfo = _getPackSporeInfo(sporeId, fromId, uid, speedX, speedY, x, y, initSpeed, initDeltaSpeed, initStopFrame, cd)
		sporeInfos[sporeId] = sporeInfo
	end
	local playerInfos = {}
	local playerIds = self.gameManager:GetAllPlayerIdxs()
	for _, playerId in ipairs(playerIds or {}) do
		local playerInfo = self:_getPlayerFrameData(playerId)
		playerInfos[playerId] = playerInfo
	end

	ret.foodInfos = foodInfos
	ret.spikyInfos = spikyInfos
	ret.sporeInfos = sporeInfos
	ret.playerInfos = playerInfos

	self.serverToClientComponent:ReStart()
	self.serverToClientComponent:SyncFirstData(ret)
	return true
end


--只同步新的玩家
function BBCCServerComponent:SyncNewPlayers()
	local newPlayerIds = self.gameManager:GetFrameNewPlayer()
	local ret = {}
	ret.newPlayers = {}
	for _, playerId in ipairs(newPlayerIds or {}) do
		local playerInfo = self:_getPlayerFrameData(playerId)
		ret.newPlayers[playerId] = playerInfo
		if playerId == g_user_info.get_user_info().uid then
			self._frame_generate_main_player = true
		end
	end
	self.serverToClientComponent:SyncNewPlayers(ret)
end

function BBCCServerComponent:_getPlayerFrameData(playerId)
	-- local playerInfo = BBCCPlayerInfo:New()
	local playerInfo = {}
	local directionX, directionY, nmass,ackCommand,finalPointX, finalPointY, stopped = self.gameManager:GetPlayer(playerId)
	playerInfo.uid = playerId
	playerInfo.directionX = directionX
	playerInfo.directionY = directionY
	playerInfo.nmass = nmass
	playerInfo.ackCommand = ackCommand
	playerInfo.finalPointX = finalPointX
	playerInfo.finalPointY = finalPointY
	playerInfo.stopped = stopped
	playerInfo.nodes = {}
	local playerNodeIds = self.gameManager:GetAllPlayerNodeIdxs(playerId)
	for _, playerNodeId in ipairs(playerNodeIds or {}) do
		local uid, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX, speedY = self.gameManager:GetPlayerNodeInfo(playerNodeId)
		local node = {}
		node.uid = uid
		node.idx = idx
		node.mass = mass
		node.fromId = fromId
		node.x = x
		node.y = y
		node.cd = cd
		node.protect = protect
		node.initStopFrame = initStopFrame
		node.initSpeed = initSpeed
		node.initDeltaSpeed = initDeltaSpeed
		node.speedX = speedX
		node.speedY = speedY
		-- playerInfo:AddNodeInfo(node)
		table.insert(playerInfo.nodes, node)
	end
	return playerInfo
end

function BBCCServerComponent:Mass2Radius(mass)
	return 4 + math.ceil(math.sqrt(mass) * 6)
end

function BBCCServerComponent:getVisibleRect()
	local playerNodeIds = self.gameManager:GetAllPlayerNodeIdxs(g_user_info.get_user_info().uid)
	for _, playerNodeId in ipairs(playerNodeIds or {}) do
		local uid, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX, speedY = self.gameManager:GetPlayerNodeInfo(playerNodeId)
		local raidus = self:Mass2Radius(mass)
		x = x / 1000
		y = y / 1000
		local startx, starty, endx, endy = x - raidus, y - raidus, x + raidus, y + raidus
		return startx - 1280 - 640, starty - 960 - 480, endx + 1280 + 640, endy + 960 + 480
	end	
end

function BBCCServerComponent:_getCanUpdatePlayerIds()
	local minX, minY, maxX, maxY
	if self._frame_generate_main_player then
		minX, minY, maxX, maxY = self:getVisibleRect() 
	else
		minX, minY, maxX, maxY = self._mainPanel.bgComponent:GetCanSyncRect()
	end
    local playerIds = self.gameManager:GetAllPlayerIdxInRect(minX, maxX, minY, maxY)
    return playerIds
end

function BBCCServerComponent:HandleAck()
	-- if not self._mainPanel.playerManager:GetMyPlayerObject() then--主玩家还没准备好
	-- 	return
	-- end
	if not self.lastVisiblePlayerIds then
		self.lastVisiblePlayerIds = {}
	end
	local newVisiblePlayerIdMaps = {}
	local newVisiblePlayerIds = self:_getCanUpdatePlayerIds()
	for _, playerId in ipairs(newVisiblePlayerIds) do
		newVisiblePlayerIdMaps[playerId] = true
	end

	local ret = {}
	local sendAcks = {}
	local newPlayerInfos = {}
	local playerIds = self.gameManager:GetAllPlayerIdxs()
	for _, playerId in ipairs(playerIds) do
		local isMatch = self.gameManager:IsPlayerCommandMatch(playerId)
		local ackCommand = self.gameManager:GetAckCommand(playerId)
		sendAcks[playerId] = ackCommand

		local isNeedSyncState = self.gameManager:IsNeedSyncState(playerId)
		local canSkipSyncState = self.gameManager:CanSkipSyncState(playerId)
		-- if playerId ~= g_user_info.get_user_info().uid then
		-- 	isNeedSyncState = true
		-- 	canSkipSyncState = false
		-- end
		if not self.lastVisiblePlayerIds[playerId] and newVisiblePlayerIdMaps[playerId] and playerId ~= g_user_info.get_user_info().uid then
			isNeedSyncState = true
			canSkipSyncState = false
		-- 	-- message("进入了视野")
		-- elseif self.lastVisiblePlayerIds[playerId] and not newVisiblePlayerIdMaps[playerId] then
		-- 	-- message("逃离了视野")
		end
		local isTips = playerId ~= g_user_info.get_user_info().uid

		if self.frameRequestData[playerId] then
			isNeedSyncState = true
			canSkipSyncState = false
			if isTips then
				-- message("请求推送的数据")
			end
		end

		if isNeedSyncState then
			local playerNodeIds = self.gameManager:GetAllPlayerNodeIdxs(playerId) or {}
			local lastCacheInfo = self.cacheSyncStateTime[playerId]
			if not lastCacheInfo then
				newPlayerInfos[playerId] = self:_getPlayerFrameData(playerId)
				self.cacheSyncStateTime[playerId] = {ackCommand, #playerNodeIds}
			else
				if (ackCommand - lastCacheInfo[1] > 10 and playerId == g_user_info.get_user_info().uid) or (#playerNodeIds ~= lastCacheInfo[2]) or not canSkipSyncState then
					if isTips then
						if self.frameRequestData[playerId] then
							-- message("请求数据推送")
						elseif (#playerNodeIds ~= lastCacheInfo[2]) then
							-- message("球的数量不一致推送")
						else
							-- message("其他原因")
						end
					end
					
					newPlayerInfos[playerId] = self:_getPlayerFrameData(playerId)
					self.cacheSyncStateTime[playerId] = {ackCommand, #playerNodeIds}
				else
					-- print("mismatch ignore ", lastCacheInfo[1], ackCommand, lastCacheInfo[2], #playerNodeIds)
				end
			end
		else
			-- print("run match", ackCommand)
		end
	end
	self.serverToClientComponent:UpdateServerAck(sendAcks)
	self.serverToClientComponent:SyncPlayerInfos(newPlayerInfos)
	self.lastVisiblePlayerIds = newVisiblePlayerIdMaps
end

function BBCCServerComponent:RequestData(uid)
	if not self.frameRequestData then
		self.frameRequestData = {}
	end
	-- print("玩家请求数据  ", uid)
	self.frameRequestData[uid] = true
end


function BBCCServerComponent:SyncEatFood()
	local foodEatInfos = self.gameManager:GetEatFoods()
	self.serverToClientComponent:SyncFoodEatInfo(foodEatInfos)
	if #foodEatInfos > 0 then
		-- print("食物被吃了 ", foodEatInfos)
	end
end

function BBCCServerComponent:SyncEatSpiky()
	local spikyEatInfos = self.gameManager:GetEatSpikys()
	self.serverToClientComponent:SyncSpikyEatInfos(spikyEatInfos)
end

function BBCCServerComponent:SyncEatSpore()
	local sporeEatInfos = self.gameManager:GetEatSpores()
	self.serverToClientComponent:SyncSporeEatInfo(sporeEatInfos)
end

function BBCCServerComponent:SyncNewFood()
	local foodInfos = self.gameManager:GetFrameNewFood()
	if foodInfos and #foodInfos > 0 then
		-- print("新的食物  ", foodInfos)
		self.serverToClientComponent:NewFoods(foodInfos)
	end
end

function BBCCServerComponent:SyncNewSpore()
	local newSpores = self.gameManager:GetFrameNewSpore()
	
	if not newSpores or #newSpores <= 0 then
		return
	end
	local syncInfos = {}
	for _, sporeId in ipairs(newSpores) do
		local fromId, uid, speedX, speedY, x, y, initSpeed, initDeltaSpeed, initStopFrame, cd = self.gameManager:GetSporeInfo(sporeId)
		if fromId then
			syncInfos[sporeId] = {fromId, uid, speedX, speedY, x, y, initSpeed, initDeltaSpeed, initStopFrame, cd}
		end
	end
	-- print("新的孢子:  ", newSpores)
	self.serverToClientComponent:NewSpores(syncInfos)
end

function BBCCServerComponent:SyncNewSpiky()
	local newSpikys = self.gameManager:GetFrameNewSpiky()
	if not newSpikys or #newSpikys <= 0 then
		return
	end
	local syncInfos = {}
	for _, spikyId in ipairs(newSpikys) do
		local x, y, mass = self.gameManager:GetSpikyInfo(spikyId)
		local info = {}
		info.idx = spikyId
		info.x = x
		info.y = y
		info.mass = mass
		syncInfos[spikyId] = info
	end
	self.serverToClientComponent:NewSpikys(syncInfos)
end