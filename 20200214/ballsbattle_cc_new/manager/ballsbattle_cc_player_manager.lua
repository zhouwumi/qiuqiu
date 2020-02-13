BBCCPlayerManager = CreateClass()

local PlayerNodeObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_player_object_node').PlayerNodeObject
local BBFakeBallObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_fake_ball_object').BBFakeBallObject
local PlayerObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_player_object').PlayerObject
local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils

function BBCCPlayerManager:__init__(mainPanel)
	self._mainPanel = mainPanel
	self._gameManager = mainPanel.gameManager
	self._objectManager = mainPanel.objectManager
	self.allPlayers = {}
	self.allPlayerNodes = {}
	self.allFakeNodes = {}
end

function BBCCPlayerManager:RemovePlayer(uid)
	local playerObject = self.allPlayers[uid]
	local keys = table.keys(playerObject.allPlayerNodes)
	for _, key in ipairs(keys) do
	    playerObject:RemovePlayerNode(key)
	end
	self._gameManager:RemovePlayer(uid)
	self.allPlayers[uid] = nil
end

function BBCCPlayerManager:GetPlayerNodeCount()
	local count = 0
	for _, playerObject in pairs(self.allPlayers or {}) do
		count = count + table.count(playerObject.allPlayerNodes)
	end

	return count
end


function BBCCPlayerManager:GeneratePlayerAndNodes(newPlayers)
	--new player
	for playerId, playerInfo in pairs(newPlayers or {}) do
        print("生成了新玩家  ", playerId)
		local playerObject = self.allPlayers[playerId]
		if not playerObject then
			playerObject = PlayerObject:New(self._mainPanel, playerId)
			self.allPlayers[playerId] = playerObject
		end
		playerObject:NewBirth()
		playerObject.isMe = playerId == g_user_info.get_user_info().uid

		for _, node in ipairs(playerInfo.nodes) do
		    local uid = node.uid
		    local idx = node.idx
		    local mass = node.mass
		    local fromId = node.fromId
		    local x = node.x
		    local y = node.y
		    local cd = node.cd
		    local protect = node.protect
		    local initStopFrame = node.initStopFrame
		    local initSpeed = node.initSpeed
		    local initDeltaSpeed = node.initDeltaSpeed
		    local speedX = node.speedX
		    local speedY = node.speedY

		    print("新球的位置  ", x, y)
			local playerNode = PlayerNodeObject:New(self._mainPanel, idx)
			playerNode:SetBirthInfo(mass)
			playerNode.isMe = uid == g_user_info.get_user_info().uid
			local playerObject = self.allPlayers[playerId]
			playerObject:AddPlayerNode(playerNode)
			self.allPlayerNodes[idx] = playerNode

			local fakeBallObject = BBFakeBallObject:New(self._mainPanel, idx)
			fakeBallObject:SetPosition(x, y)
			self.allFakeNodes[idx] = fakeBallObject
		end
    end
end

function BBCCPlayerManager:AddNewPlayerNode(playerNodeInfo)
	local uid = playerNodeInfo.uid
	local cd = playerNodeInfo.cd
    local fromId = playerNodeInfo.fromId
    local idx = playerNodeInfo.idx
    local initDeltaSpeed = playerNodeInfo.initDeltaSpeed
    local initSpeed = playerNodeInfo.initSpeed
    local initStopFrame = playerNodeInfo.initStopFrame
    local mass = playerNodeInfo.mass
    local protect = playerNodeInfo.protect
    local x = playerNodeInfo.x
    local y = playerNodeInfo.y
    local speedX = playerNodeInfo.speedX
    local speedY = playerNodeInfo.speedY

    local playerNode = PlayerNodeObject:New(self._mainPanel, idx)
	playerNode:SetBirthInfo(mass)
	local playerObject = self.allPlayers[uid]
	playerObject:AddPlayerNode(playerNode)
	self.allPlayerNodes[idx] = playerNode
	playerNode.isMe = uid == g_user_info.get_user_info().uid

	local fakeBallObject = BBFakeBallObject:New(self._mainPanel, idx)
	fakeBallObject:SetPosition(x, y, mass)
	self.allFakeNodes[idx] = fakeBallObject
end

function BBCCPlayerManager:ServerChangeFakeNode(idx, x, y, mass)
	local fakeBallObject = self.allFakeNodes[idx]
	fakeBallObject:SetPosition(x, y, mass)
end

function BBCCPlayerManager:RemovePlayerNode(uid, nodeIdx)
	local playerObject = self.allPlayers[uid]
	playerObject:RemovePlayerNode(nodeIdx)
	self.allPlayerNodes[nodeIdx] = nil

	local fakeBallObject = self.allFakeNodes[nodeIdx]
	fakeBallObject:Remove()
end

function BBCCPlayerManager:Update()
	for _, playerObject in pairs(self.allPlayers) do
		playerObject:Update()
	end
end

function BBCCPlayerManager:PrintPlayerInfo()
	for _, playerObject in pairs(self.allPlayers) do
	    playerObject:PrintPostionInfo()
	end
end

function BBCCPlayerManager:GetMyPlayerObject()
	local playerObject = self.allPlayers[self._mainPanel.currentPlayerIdx]
	return playerObject
end