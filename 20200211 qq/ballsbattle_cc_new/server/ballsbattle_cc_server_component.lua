BBCCServerComponent = CreateClass()

function BBCCServerComponent:__init__(mainPanel)
	self._mainPanel = mainPanel
	self.gameManager = cc.BBServerGameManager:Create()
	self.gameManager:InitRoom()
	self.gameManager:SetServerState(true)
	self.gameManager:SetCanEatFoodSpiky(true)
	self.gameManager:SetCanEatSpore(true)

	self.cacheSyncStateTime = {}
	self.gameManager:InitFoodSpiky()
	delay_call(0, function()
		if not mainPanel:is_valid() then
			return
		end
		self:FrameUpdate()
		return 1 / 15
	end)
end

function BBCCServerComponent:JoinPlayer(uid)
	self.gameManager:AddOperatePlayerJoin(uid)
end

function BBCCServerComponent:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, commandIdx, checkSum)
	print("server add command  ", uid, commandIdx, checkSum)
	if isShoot then
		-- message("接受到了吐孢子指令")
		print('接受到了吐孢子指令')
	end
	self.gameManager:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, commandIdx, checkSum)
end

function BBCCServerComponent:FrameUpdate()
	self.gameManager:FrameUpdate()
	self:SyncEatFood()
	self:SyncEatSpore()
	self:SyncNewFood()
	self:SyncNewSpore()
	self:SyncNewPlayers()
	self:HandleAck()
end

--只同步新的玩家
function BBCCServerComponent:SyncNewPlayers()
	local newPlayerIds = self.gameManager:GetFrameNewPlayer()
	local newPlayerNodeIds = self.gameManager:GetFrameNewPlayerNodeIdxs()
	local ret = {}
	ret.newPlayers = {}
	local cachaPlayers = {}
	for _, playerId in ipairs(newPlayerIds or {}) do
		local playerIdx, directionX, directionY, nmass, nextNodeIdx, ackCommand, finalPointX, finalPointY, stopped = self.gameManager:GetPlayer(playerId)
		table.insert(ret.newPlayers, {playerId, playerIdx, directionX, directionY, nmass, nextNodeIdx, ackCommand,finalPointX, finalPointY, stopped})
		cachaPlayers[playerId] = true
	end
	ret.newPlayerNodes = {}
	for _, playerNodeId in ipairs(newPlayerNodeIds) do
		local playerId, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX, speedY = self.gameManager:GetPlayerNodeInfo(playerNodeId)
		if cachaPlayers[playerId] then
			table.insert(ret.newPlayerNodes, {playerId, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX, speedY})
		end
	end
	if #newPlayerIds > 0 then
		local foodInfos = self.gameManager:GetAllFoodInfos()
		local spikyInfoIds = self.gameManager:GetAllSpikyInfos()
		local spikyInfos = {}
		for _, idx in ipairs(spikyInfoIds or {}) do
			local x, y, mass, raidus = self.gameManager:GetSpikyInfo(idx)
			table.insert(spikyInfos, {idx, x, y, mass, raidus})
		end
		ret.foodInfos = foodInfos
		ret.spikyInfos = spikyInfos
	end
	self._mainPanel:SyncFirstData(ret)
end

function BBCCServerComponent:HandleAck()
	local sendAcks = {}
	local newPlayerInfos = {}
	local playerIds = self.gameManager:GetAllPlayerIdxs()
	for _, playerId in ipairs(playerIds) do
		local isMatch = self.gameManager:IsPlayerCommandMatch(playerId)
		local ackCommand = self.gameManager:GetAckCommand(playerId)
		sendAcks[playerId] = ackCommand

		local isNeedSyncState = self.gameManager:IsNeedSyncState(playerId)
		local canSkipSyncState = self.gameManager:CanSkipSyncState(playerId)
		if isNeedSyncState then
			local playerNodeIds = self.gameManager:GetAllPlayerNodeIdxs(playerId) or {}
			local lastCacheInfo = self.cacheSyncStateTime[playerId]
			if not lastCacheInfo then
				newPlayerInfos[playerId] = self:_getPlayerFrameData(playerId)
				self.cacheSyncStateTime[playerId] = {ackCommand, #playerNodeIds}
			else
				if (ackCommand - lastCacheInfo[1] > 5 or #playerNodeIds ~= lastCacheInfo[2]) or not canSkipSyncState then
					newPlayerInfos[playerId] = self:_getPlayerFrameData(playerId)
					self.cacheSyncStateTime[playerId] = {ackCommand, #playerNodeIds}
				else
					print("mismatch ignore ", lastCacheInfo[1], ackCommand, lastCacheInfo[2], #playerNodeIds)
				end
			end
		else
			print("run match", ackCommand)
		end
	end
	self._mainPanel:UpdateServerAck(sendAcks)
	self._mainPanel:SyncPlayerInfos(newPlayerInfos)
end

function BBCCServerComponent:_getPlayerFrameData(playerId)
	local data = {}
	local playerIdx, directionX, directionY, nmass, nextNodeIdx,ackCommand,finalPointX, finalPointY, stopped = self.gameManager:GetPlayer(playerId)
	data.playerIdx = playerIdx
	data.directionX = directionX
	data.directionY = directionY
	data.nmass = nmass
	data.nextNodeIdx = nextNodeIdx
	data.ackCommand = ackCommand
	data.finalPointX = finalPointX
	data.finalPointY = finalPointY
	data.stopped = stopped
	data.nodes = {}
	local playerNodeIds = self.gameManager:GetAllPlayerNodeIdxs(playerId)
	for _, playerNodeId in ipairs(playerNodeIds or {}) do
		local uid, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX, speedY = self.gameManager:GetPlayerNodeInfo(playerNodeId)
		local node = {}
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
		table.insert(data.nodes, node)
	end
	return data
end

function BBCCServerComponent:SyncEatFood()
	local foodEatInfos = self.gameManager:GetEatFoods()
	self._mainPanel:SyncFoodEatInfo(foodEatInfos)
end

function BBCCServerComponent:SyncEatSpore()
	local sporeEatInfos = self.gameManager:GetEatSpores()
	self._mainPanel:SyncSporeEatInfo(sporeEatInfos)
end


function BBCCServerComponent:SyncNewFood()
	local foodInfos = self.gameManager:GetFrameNewFood()
	if foodInfos and #foodInfos > 0 then
		self._mainPanel:NewFoods(foodInfos)
	end
end

function BBCCServerComponent:SyncNewSpore()
	local newSpores = self.gameManager:GetFrameNewSpore()
	print(newSpores)
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
	print("新的孢子:  ", syncInfos)
	self._mainPanel:NewSpores(syncInfos)
end
