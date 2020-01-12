BBCCPlayerManager = CreateClass()

local PlayerNodeObject = import('logic.dialog.ballsbattle_cc.objects.bb_player_object_node').PlayerNodeObject
local PlayerObject = import('logic.dialog.ballsbattle_cc.objects.bb_player_object').PlayerObject

function BBCCPlayerManager:__init__(mainPanel)
	self._mainPanel = mainPanel
	self._gameManager = mainPanel.gameManager
	self._objectManager = mainPanel.objectManager
	self.allPlayers = {}
end

function BBCCPlayerManager:_createPlayerNode(uid, playerNodeIdx)
	local playerObject = self.allPlayers[uid]
	local x, y, mass = self._gameManager:GetPlayerNodeInfo(playerNodeIdx)
	local playerNode = PlayerNodeObject:New(self._mainPanel, playerNodeIdx)
	playerObject:AddPlayerNode(playerNode)
	playerNode:SetBirthInfo(x, y, mass)
	print("生成玩家节点  ", uid, playerNodeIdx, self._gameManager:GetGameFrame(), x, y, mass)
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

function BBCCPlayerManager:KeyFrameCreateNewPlayerNodes()
	local newPlayerNodes = self._gameManager:GetFrameNewPlayerNodeIdxs()
	if #newPlayerNodes > 0 then
		print(newPlayerNodes)
		local playerId
		for i = 1, #newPlayerNodes do
			if playerId == nil then
				playerId = newPlayerNodes[i]
			elseif newPlayerNodes[i] == -1 then
				playerId = nil
			else
				self:_createPlayerNode(playerId, newPlayerNodes[i])
			end
		end
	end
end

function BBCCPlayerManager:KeyFrameRemovePlayerNodes()
	local removedPlayerIdxs = self._gameManager:GetFrameRemovedPlayerNodeIdxs()
	if #removedPlayerIdxs > 0 then
		print("需要移除的玩家节点  ", removedPlayerIdxs)
		for index = 1, #removedPlayerIdxs, 2 do
			local playerId = removedPlayerIdxs[index]
			local playerNodeId = removedPlayerIdxs[index + 1]
			local playerObject = self.allPlayers[playerId]
			playerObject:RemovePlayerNode(playerNodeId)
		end
	end
end

function BBCCPlayerManager:UpdateAllPlayers()
	for id, playerObject in pairs(self.allPlayers) do
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