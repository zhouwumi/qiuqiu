BBGameMainGameManager = CreateClass()

local BBGameFoodSpikyManager = import('logic.dialog.ballsbattle_cc_new.game.manager.bb_game_food_spiky_manager').BBGameFoodSpikyManager
local BBGameFrameInManager = import('logic.dialog.ballsbattle_cc_new.game.manager.bb_game_frame_in_manager').BBGameFrameInManager
local BBGameObjectManager = import('logic.dialog.ballsbattle_cc_new.game.manager.bb_game_object_manager').BBGameObjectManager
local BBGamePlayerManager = import('logic.dialog.ballsbattle_cc_new.game.manager.bb_game_player_manager').BBGamePlayerManager
local BBGameSporeManager = import('logic.dialog.ballsbattle_cc_new.game.manager.bb_game_spore_manager').BBGameSporeManager
local BBGameConfigManager = import('logic.dialog.ballsbattle_cc_new.game.manager.bb_game_config_manager').BBGameConfigManager
local BBGameMoveManager = import('logic.dialog.ballsbattle_cc_new.game.move.bb_game_move_manager').BBGameMoveManager

local BBGameQuadTree = import('logic.dialog.ballsbattle_cc_new.game.base.bb_game_quad_tree').BBGameQuadTree
local BBGameRect = import('logic.dialog.ballsbattle_cc_new.game.base.bb_game_rect').BBGameRect
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBGameMainGameManager:__init__()
	self.mGameFrame = 0
	local w = constant_ballsbattle_cc.BBConfigManager.gameWidth
	local h = constant_ballsbattle_cc.BBConfigManager.gameHeight
	self.NodeTree = BBGameQuadTree:New(w, h)
	self.NodeTree:OnBuildFinish()

	self.objectManager = BBGameObjectManager:New(self)
	self.playerManager = BBGamePlayerManager:New(self)
	self.frameInManager = BBGameFrameInManager:New(self)
	self.foodSpikyManager = BBGameFoodSpikyManager:New(self)
	self.sporeManager = BBGameSporeManager:New(self)
	self.moveManager = BBGameMoveManager:New()
	self.configManager = BBGameConfigManager:New()

	self.spikyRect = BBGameRect:New(500, 500, w - 500, h - 500)
	self.mapRect = BBGameRect:New(0, 0, w, h)
end

function BBGameMainGameManager:GetGameFrame()
	return self.mGameFrame
end

function BBGameMainGameManager:GetAllPlayerNodeIdxs(uid)
	local ret = {}
	local player = self.playerManager:GetPlayer(uid)
	if player then		
		for _, playerNode in ipairs(player.vecPlayerNodes or {}) do
			table.insert(ret, playerNode.idx)
		end
	end
	return ret
end


function BBGameMainGameManager:IsNeedSyncState(uid)
	local player = self.playerManager:GetPlayer(uid)
	if player then		
		return player:IsNeedSyncState()
	end
	return false
end

function BBGameMainGameManager:CanSkipSyncState(uid)
	local player = self.playerManager:GetPlayer(uid)
	if player then		
		return player:CanSkipSyncState()
	end
	return false
end

function BBGameMainGameManager:GetPlayerDelay()
	local player = self.playerManager:GetPlayer(g_user_info.get_user_info().uid)
	if player then
		return #player.mCommandList.mCommandVec
	end
	return 0
end

function BBGameMainGameManager:FrameUpdate()
	self.mGameFrame = self.mGameFrame + 1
	self.playerManager:HandleFrameInputCommand();
	self.sporeManager:MoveSpores();
	self.frameInManager:FrameClear();
end

function BBGameMainGameManager:UpdatePlayer(uid)
	local player = self:GetPlayer(uid)
	if player then
		player:Update()
	end
end

function BBGameMainGameManager:GetSpiky(idx)
	return self.foodSpikyManager:GetSpikyInfo(idx)
end

function BBGameMainGameManager:GetSpore(idx)
	return self.sporeManager:GetSpore(idx)
end

function BBGameMainGameManager:GetPlayerNode(idx)
	return self.playerManager.mapPlayerNodes[idx]
end

function BBGameMainGameManager:GetPlayer(uid)
	return self.playerManager:GetPlayer(uid)
end

function BBGameMainGameManager:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, id, checkSum)
	self.frameInManager:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, id, checkSum);
end

function BBGameMainGameManager:RemovePlayerCommand(uid)
	local playerCommands = self.frameInManager.playerCommands;
	local commands = playerCommands[uid]
	if commands and #commands > 0 then
		local lastCommand = commands[#commands]
		local player = self:GetPlayer(uid)
		if player then
			player.predictionData.lastAngle = lastCommand.angle;
			player.predictionData.lastPressure = lastCommand.pressure;
		end
	end
	playerCommands[uid] = nil
end

function BBGameMainGameManager:GetAllPlayerIdxInRect(minX, maxX, minY, maxY)
	local ret = {}
	for _, playerId in ipairs(self.playerManager.playerIds or {}) do
		local player = self:GetPlayer(playerId)
		if player then
			local rect = player:GetGroupRect()
			if rect:IntersectsRectPoint(minX, maxX, minY, maxY) then
				table.insert(ret, playerId)
			end
		end
	end
	return ret;
end

function BBGameMainGameManager:SyncServerAckCommand(uid, ackCommand)
	local player = self.playerManager:GetPlayer(uid)
	if player then
		player.mCommandList:ClientSyncServerCommandAck(ackCommand)
	end
end

function BBGameMainGameManager:ClientSyncServerPlayerInfo(uid, nMass, directionX, directionY, ackCommand, finalPointX, finalPointY, Stopped)
	local player = self.playerManager:GetPlayer(uid)
	if player then
		player:ClientSyncServerPlayerInfo(nMass, directionX, directionY, ackCommand, finalPointX, finalPointY, Stopped)
	end	
end

function BBGameMainGameManager:CalcBallDelta(uid)
	local player = self.playerManager:GetPlayer(uid)
	if player then
		player:CalcBallDelta()
	end	
end

function BBGameMainGameManager:ClientCatchup(uid)
	local player = self.playerManager:GetPlayer(uid)
	if player then
		player:clientCatchup()
	end	
end

function BBGameMainGameManager:ClientSyncServerPlayerNode(uid, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX, speedY)
	local player = self.playerManager:GetPlayer(uid)
	if player then
		local playerNode = player:GetPlayerNode(idx)
		if playerNode then
			playerNode:ClientSyncServerPlayerNode(fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX,speedY)
		end
	end	
end

function BBGameMainGameManager:ClientSyncRenderToLocation(idx)
	local node = self:GetPlayerNode(idx)
	if node then
		node:ClientSyncRenderToLocation()
	end	
end

function BBGameMainGameManager:CreatePlayerNodeFromServer(uid, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX, speedY)
	self.playerManager:CreatePlayerNodeFromServer(uid, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed,speedX,speedY);
end

function BBGameMainGameManager:CreateSplitPlayerNodeFromServer(uid, idx, fromId, x, y, mass, speedX, speedY, cd, protect)
	self.playerManager:CreateSplitPlayerNodeFromServer(uid, idx, fromId, x, y, mass, speedX, speedY, cd, protect);
end

function BBGameMainGameManager:RemovePlayerNodeFromServer(uid, idx)
	self.playerManager:RemovePlayerNode(uid, idx);
end

function BBGameMainGameManager:CreatePlayerFromServer(uid, directionX, directionY, NMass, finalPointX, finalPointY, Stopped)
	self.playerManager:CreatePlayerFromServer(uid, directionX, directionY, NMass, finalPointX, finalPointY, Stopped);
end

function BBGameMainGameManager:AddNewFoodFromServer(posKey)
	self.foodSpikyManager:ClientAddNewFoodFromServer(posKey);
end


function BBGameMainGameManager:AddNewSporeFromServer(idx, fromId, uid, x, y, directionX, directionY,curSpeed, deltaSpeed, initStopFrame, cd)
	self.sporeManager:AddNewSporeFromServer(idx, fromId, uid, x, y, directionX, directionY, curSpeed, deltaSpeed, initStopFrame, cd);
end

function BBGameMainGameManager:AddNewSpikyFromServer(idx, posKey, mass)
	self.foodSpikyManager:ClientAddNewSpikyFromServer(idx, posKey, mass)
end

function BBGameMainGameManager:SyncShootFromServer(idx, fromId, uid, speedX, speedY, locationX, locationY)
	self.sporeManager:SyncShootFromServer(idx, fromId, uid, speedX, speedY, locationX, locationY);
end

function BBGameMainGameManager:RemoveFoodFromServer(posKey)
	self.foodSpikyManager:RemoveFoodByIdx(posKey);
end

function BBGameMainGameManager:RemoveSporeFromServer(sporeIdx)
	self.sporeManager:RemoveSporeByIdx(sporeIdx);
end

function BBGameMainGameManager:CanAddPlayerCommand(uid)
	local player = self.playerManager:GetPlayer(uid)
	if player then
		return not player.mCommandList:IsBacklogged()
	end
	return false;
end

function BBGameMainGameManager:SimulateEatFoods(playerIds)
	return {}
end

function BBGameMainGameManager:SimulateEatSpores(playerIds)
	return {}
end

function BBGameMainGameManager:GetNodeInRound(nodeIdx, expandCircle)
	local ret = {}
	local playerNode = self:GetPlayerNode(nodeIdx);
	if playerNode then
		local TempVec = {}
		local TempPlayerRect = playerNode.rect:Expand(expandCircle, expandCircle);
		self.NodeTree:GetAllHitCircleNodeIdxs(TempPlayerRect, TempVec);
		for _, idx in ipairs(TempVec or {}) do
			local targetNode = self:GetPlayerNode(idx)
			if targetNode then
				if playerNode.protect > 0 or targetNode.protect > 0 or playerNode.uid == targetNode.uid then
				else
					table.insert(ret, targetNode.idx)
				end
			end
		end
	end
	return ret
end

----------export-----------------
function BBGameMainGameManager:GetPlayerNodeInfo(idx)
	local node = self:GetPlayerNode(idx)
	if not node then
		return
	end
	return node.uid, node.idx, node.fromId, node:GetIntLocationX(), node:GetIntLocationY(), node:GetBallMass(), node.cd,node.protect, node.initStopFrame, node.initSpeed, node.initDeltaSpeed, node:GetIntSpeedX(), node:GetIntSpeedY()
end

function BBGameMainGameManager:GetPlayerSimpleNodeInfo(idx)
	local node = self:GetPlayerNode(idx)
	if not node then
		print("not find player node for idx: ", idx)
		return
	end
	-- print('get player node simple info for idx: ', idx)
	return node:GetIntLocationX(), node:GetIntLocationY(), node:GetBallMass(), node.cd, node.protect, node.currentSpeedVec.x, node.currentSpeedVec.y, node.initStopFrame
end

function BBGameMainGameManager:GetPlayerNodeRenderX(idx)
	local node = self:GetPlayerNode(idx)
	if not node then
		return
	end
	return node:GetRenderX()
end

function BBGameMainGameManager:GetPlayerNodeRenderY(idx)
	local node = self:GetPlayerNode(idx)
	if not node then
		return
	end
	return node:GetRenderY()
end

function BBGameMainGameManager:GetPlayerNodeRenderPos(idx)
	local node = self:GetPlayerNode(idx)
	if not node then
		return
	end
	return node:GetRenderX(), node:GetRenderY()
end

function BBGameMainGameManager:GetSpikyInfo(idx)
	local node = self:GetSpiky(idx)
	if not node then
		return node
	end
	return node.location.x, node.location.y, node:GetBallMass()
end

function BBGameMainGameManager:GetSporeInfo(idx)
	local node = self:GetSpore(idx)
	if not node then
		return
	end
	return  node.fromId, node.uid, node:GetIntSpeedX(), node:GetIntSpeedY(), node:GetIntLocationX(), node:GetIntLocationY(), node.initSpeed, node.initDeltaSpeed, node.initStopFrame, node.cd
end

function BBGameMainGameManager:GetSporeRenderX(idx)
	local node = self:GetSpore(idx)
	if not node then
		return
	end
	return node:GetRenderX()
end

function BBGameMainGameManager:GetSporeRenderY(idx)
	local node = self:GetSpore(idx)
	if not node then
		return
	end
	return node:GetRenderY()
end

function BBGameMainGameManager:GetSporeRenderPos(idx)
	local node = self:GetSpore(idx)
	if not node then
		print("not find spore for idx", idx)
		return
	end
	return node:GetRenderX(), node:GetRenderY()
end

function BBGameMainGameManager:OnNodeEatFood(nodeIdx, massChange)
	local node = self:GetPlayerNode(nodeIdx)
	if not node then
		return node
	end
	node:ChangeDeltaMass(massChange)
	-- if node.mass < constant_ballsbattle_cc.BBConfigManager.initBallMass then
	-- 	node:SetBallMass(constant_ballsbattle_cc.BBConfigManager.initBallMass)
	-- end
end