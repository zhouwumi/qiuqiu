BBCCServerComponent = CreateClass()

function BBCCServerComponent:__init__(mainPanel)
	self._mainPanel = mainPanel
	self.gameManager = cc.BBGameManager:Create()
	self.gameManager:InitRoom()
	self.gameManager:SetServerState(true)
	self.gameManager:SetCanEatFoodSpiky(true)

	self:PrepareFoodSpiky()
	delay_call(0, function()
		if not mainPanel:is_valid() then
			return
		end
		self:FrameUpdate()
		return 1 / 15
	end)
end

function BBCCServerComponent:PrepareFoodSpiky()
	local foodInfos = self.gameManager:ClientGenerateFood(600)
    local spikyInfos = self.gameManager:ClientGenerateSpiky(30)
    for index = 1, #foodInfos do
        local idx = foodInfos[index]
        self.gameManager:AddNewFoodFromServer(idx)
    end
    for index = 1, #spikyInfos, 3 do
        local idx = spikyInfos[index]
        local pos = spikyInfos[index + 1]
        local mass = spikyInfos[index + 2]
        self.gameManager:AddNewSpikyFromServer(idx, pos, mass)
    end
    self.foodInfos = foodInfos
    self.spikyInfos = spikyInfos
end

function BBCCServerComponent:JoinPlayer(uid)
	self.gameManager:AddOperatePlayerJoin(uid)
end

function BBCCServerComponent:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, commandIdx, checkSum)
	print("server add command  ", uid, commandIdx, checkSum)
	self.gameManager:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, commandIdx, checkSum)
end

function BBCCServerComponent:FrameUpdate()
	self.gameManager:FrameUpdate()
	local ret = self:GenerateData()
	self._mainPanel:UpdateDataFromServer(ret)
	self:HandleAck()
end

function BBCCServerComponent:GenerateData()
	local newPlayerIds = self.gameManager:GetFrameNewPlayer()
	local newPlayerNodeIds = self.gameManager:GetFrameNewPlayerNodeIdxs()
	local ret = {}
	ret.newPlayers = {}
	for _, playerId in ipairs(newPlayerIds or {}) do
		local playerIdx, directionX, directionY, nmass, nextNodeIdx, ackCommand = self.gameManager:GetPlayer(playerId)
		table.insert(ret.newPlayers, {playerId, playerIdx, directionX, directionY, nmass, nextNodeIdx, ackCommand})
	end
	ret.newPlayerNodes = {}
	for _, playerNodeId in ipairs(newPlayerNodeIds) do
		local playerId, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed = self.gameManager:GetPlayerNodeInfo(playerNodeId)
		table.insert(ret.newPlayerNodes, {playerId, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed})
	end
	if #newPlayerIds > 0 then
		ret.foodInfos = self.foodInfos
		ret.spikyInfos = spikyInfos
	end
	return ret
end

function BBCCServerComponent:HandleAck()
	local sendAcks = {}
	local misMatchPlayerInfos = {}
	local playerIds = self.gameManager:GetAllPlayerIdxs()
	for _, playerId in ipairs(playerIds) do
		local isMatch = self.gameManager:IsPlayerCommandMatch(playerId)
		local ackCommand = self.gameManager:GetAckCommand(playerId)
		sendAcks[playerId] = ackCommand
		if not isMatch then
			misMatchPlayerInfos[playerId] = self:_getPlayerFrameData(playerId)
		else
			print("run match")
		end
	end
	self._mainPanel:UpdateServerAck(sendAcks)
	self._mainPanel:UpdatePlayerMismatch(misMatchPlayerInfos)
	-- return sendAcks, misMatchPlayerInfos
end

function BBCCServerComponent:_getPlayerFrameData(playerId)
	local data = {}
	local playerIdx, directionX, directionY, nmass, nextNodeIdx,ackCommand = self.gameManager:GetPlayer(playerId)
	data.playerIdx = playerIdx
	data.directionX = directionX
	data.directionY = directionY
	data.nmass = nmass
	data.nextNodeIdx = nextNodeIdx
	data.ackCommand = ackCommand
	data.nodes = {}
	local playerNodeIds = self.gameManager:GetAllPlayerNodeIdxs(playerId)
	for _, playerNodeId in ipairs(playerNodeIds or {}) do
		local uid, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed = self.gameManager:GetPlayerNodeInfo(playerNodeId)
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
		table.insert(data.nodes, node)
	end
	return data
end