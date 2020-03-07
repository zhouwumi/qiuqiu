BBGamePlayerManager = CreateClass()

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
local BBGameMathUtils = import('logic.dialog.ballsbattle_cc_new.game.math.bb_game_math_utils').BBGameMathUtils
local BBGamePlayerObject = import('logic.dialog.ballsbattle_cc_new.game.objects.bb_game_player_object').BBGamePlayerObject
local BBGamePlayerNodeObject = import('logic.dialog.ballsbattle_cc_new.game.objects.bb_game_player_node').BBGamePlayerNodeObject
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

local local_bb_int_to_float = BBGameMathUtils.bb_int_to_float
function BBGamePlayerManager:__init__(gameManager)
	self.gameManager = gameManager
	self.mapPlayerNodes = {}
	-- self.playerIds = {}
	self.mapPlayers = {}
end

function BBGamePlayerManager:OnNewPlayerNodeGenerate(player, playerNode)
	player:AddPlayerNode(playerNode);
	self.mapPlayerNodes[playerNode.idx] = playerNode
	-- self.gameManager.NodeTree:AddCircleNode(playerNode);
end

function BBGamePlayerManager:CreatePlayerFromServer(uid, directionX, directionY, NMass, finalPointX, finalPointY, Stopped)
	local player = BBGamePlayerObject:New()
	player.uid = uid
	player.gameManager = self.gameManager
	player.NMass = NMass;
	player.direction.x = directionX;
	player.direction.y = directionY;
	player.FinalPoint.x = local_bb_int_to_float(finalPointX);
	player.FinalPoint.y = local_bb_int_to_float(finalPointY);
	player.Stopped = Stopped;
	self.mapPlayers[uid] = player
	-- for _, playerId in ipairs(self.playerIds or {}) do
	-- 	if playerId == uid then
	-- 		return
	-- 	end
	-- end
	-- table.insert(self.playerIds, uid)
	-- table.sort(self.playerIds, function(a, b) return a < b end)
end

function BBGamePlayerManager:GetPlayer(uid)
	return self.mapPlayers[uid]
end

function BBGamePlayerManager:RemovePlayerNode(uid, nodeIdx)
	if not self.mapPlayerNodes[nodeIdx] then
		return
	end
	local node = self.mapPlayerNodes[nodeIdx];
	self.mapPlayerNodes[nodeIdx] = nil
	-- mapPlayerNodes.erase(nodeIdx);
	-- self.gameManager.NodeTree:RemoveCircleNode(node);

	local player = self.mapPlayers[uid]
	if player then
		player:RemovePlayerNode(node)
		-- for i, playerNode in ipairs(player.vecPlayerNodes or {}) do
		-- 	if playerNode == node then
		-- 		table.remove(player.vecPlayerNodes, i)
		-- 		return
		-- 	end
		-- end
	end
end

function BBGamePlayerManager:CreatePlayerNodeFromServer(uid, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX, speedY)
	local newPlayerNode = BBGamePlayerNodeObject:New()
	newPlayerNode.uid = uid;
	newPlayerNode.idx = idx;
	newPlayerNode.cd = cd;
	newPlayerNode.fromId = fromId;
    newPlayerNode:SetBallMass(mass);
	newPlayerNode.protect = protect;
	newPlayerNode.currentSpeedVec.x = local_bb_int_to_float(speedX)
	newPlayerNode.currentSpeedVec.y = local_bb_int_to_float(speedY)
	newPlayerNode:ChangePosition(local_bb_int_to_float(x), local_bb_int_to_float(y));
	newPlayerNode:ChangeRenderPosition(local_bb_int_to_float(x), local_bb_int_to_float(y));
	newPlayerNode.initStopFrame = initStopFrame;
	newPlayerNode.initSpeed = initSpeed;
	newPlayerNode.initDeltaSpeed = initDeltaSpeed;
	local player = self:GetPlayer(uid);
	-- //从lua层来主动创建的node不放入到新的玩家节点列表中
	self:OnNewPlayerNodeGenerate(player, newPlayerNode);
end

function BBGamePlayerManager:CreateSplitPlayerNodeFromServer(uid, idx, fromId, x, y, mass, speedX, speedY, cd, protect, initStopFrame, initSpeed, initDeltaSpeed)
	local player = self:GetPlayer(uid);
	if not player then
		return
	end
	local sourceNode = player:GetPlayerNode(fromId);
	if not sourceNode then
		return;
	end
	local newPlayerNode = BBGamePlayerNodeObject:New()
	newPlayerNode.fromId = fromId;
	newPlayerNode.uid = uid;
	newPlayerNode.player = player;
	newPlayerNode.idx = idx;
	newPlayerNode:ChangePosition(local_bb_int_to_float(x), local_bb_int_to_float(y));
	newPlayerNode.currentSpeedVec.x = local_bb_int_to_float(speedX)
	newPlayerNode.currentSpeedVec.y = local_bb_int_to_float(speedY)
	newPlayerNode:SetBallMass(mass);
	newPlayerNode.cd = cd;
	newPlayerNode.protect = protect;
	local moveVecX, moveVecY = BBGameMathUtils.GetFixedVetor2(newPlayerNode.currentSpeedVec, sourceNode:GetRadius());
	newPlayerNode:ChangeRenderPosition(sourceNode.location.x + moveVecX, sourceNode.location.y + moveVecY);
	-- newPlayerNode:ChangeRenderPosition(sourceNode.realX + moveVecX, sourceNode.realY + moveVecY)
	newPlayerNode:CalcBallDelta();
	newPlayerNode.initStopFrame = initStopFrame;
	newPlayerNode.initSpeed = initSpeed;
	newPlayerNode.initDeltaSpeed = initDeltaSpeed;
	-- newPlayerNode:CalculateInitMoveParams(newPlayerNode:GetRadius(), constant_ballsbattle_cc.BBConfigManager.splitStopFrame, constant_ballsbattle_cc.BBConfigManager.splitInitSpeed);
	self:OnNewPlayerNodeGenerate(player, newPlayerNode);
end

function BBGamePlayerManager:AddPlayerSplitNewBallFromServer(idx, fromId, uid, mass, x, y, directionX, directionY, currentX, currentY, curSpeed, deltaSpeed, initStopFrame, cd, protect)
	local newPlayerNode = BBGamePlayerNodeObject:New()
	newPlayerNode.uid = uid;
	newPlayerNode.idx = idx;
	newPlayerNode.cd = cd;
	newPlayerNode.fromId = fromId;
	newPlayerNode:SetBallMass(mass);
	newPlayerNode.initStopFrame = initStopFrame;
	newPlayerNode.protect = protect;
	newPlayerNode.currentSpeedVec.x = currentX
	newPlayerNode.currentSpeedVec.y = currentY
	-- newPlayerNode.currentSpeedVec.SetPoint(currentX, currentY);
	-- //newPlayerNode.direction.SetPoint(directionX, directionY);
	newPlayerNode:ChangePosition(x, y);
	newPlayerNode:ChangeRenderPosition(x, y);
	newPlayerNode.initSpeed = curSpeed;
	newPlayerNode.initDeltaSpeed = deltaSpeed;
	local player = self:GetPlayer(uid);
	-- //从lua层来主动创建的node不放入到新的玩家节点列表中
	self:OnNewPlayerNodeGenerate(player, newPlayerNode);
end

function BBGamePlayerManager:HandleFrameInputCommand()
	local playerCommands = self.gameManager.frameInManager.playerCommands;
	for uid, commands in pairs(playerCommands or {}) do
		for _, command in ipairs(commands or {}) do
			local player = self:GetPlayer(uid)
			if player then
				player.mCommandList:pushCommand(command)
			end
		end
	end
end