BBGameSporeManager = CreateClass()

local BBGameMathUtils = import('logic.dialog.ballsbattle_cc_new.game.math.bb_game_math_utils').BBGameMathUtils
local BBGameSporeObject = import('logic.dialog.ballsbattle_cc_new.game.objects.bb_game_move_ball').BBGameSporeObject
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBGameSporeManager:__init__(gameManager)
	self.gameManager = gameManager
	self.mapSpores = {}
	self.vecSporeIds = {}
end

function BBGameSporeManager:ClearAllSpore()
	self.mapSpores = {}
end

function BBGameSporeManager:RemoveSporeByIdx(idx)
	if not self.mapSpores[idx] then return end
	local sporeNode = self.mapSpores[idx];
	self.mapSpores[idx] = nil
	self.gameManager.NodeTree:RemoveCircleNode(sporeNode);
	table.arr_remove_v(self.vecSporeIds, idx)
	table.sort(self.vecSporeIds, function(a, b)
		return a < b
	end)
end


function BBGameSporeManager:GetSpore(idx)
	return self.mapSpores[idx]
end

function BBGameSporeManager:FixSpore(sporeNode)
	local isFix, fixedX, fixedY = BBGameMathUtils.FixCircle(self.gameManager.mapRect, sporeNode.location.x, sporeNode.location.y, sporeNode:GetRadius())
	sporeNode:ChangePosition(fixedX, fixedY)
end

function BBGameSporeManager:MoveSpores()
	for _, sporeNode in pairs(self.mapSpores or {}) do
		if sporeNode.initStopFrame > 0 then
			local oldX = sporeNode.location.x;
			local oldY = sporeNode.location.y;
			sporeNode:InitMove();
			self:FixSpore(sporeNode);
			local deltaX = sporeNode.location.x - oldX;
			local deltaY = sporeNode.location.y - oldY;
			local newRenderX = sporeNode.mDeltaData.location.x + deltaX;
			local newRenderY = sporeNode.mDeltaData.location.y + deltaY;
			if sporeNode.mDeltaData.wrapTicks > 0 then
				sporeNode.mDeltaData.wrapTicks = sporeNode.mDeltaData.wrapTicks - 1
				newRenderX = newRenderX + sporeNode.mDeltaData.wrapLocationOffset.x;
				newRenderY = newRenderY + sporeNode.mDeltaData.wrapLocationOffset.y;
			end
			-- print("spore move  ", newRenderX, newRenderY, sporeNode.location.x, sporeNode.location.y)
			-- //BBMathUtils::BBLOG("sporeNode change render real pos %i %f-%f, %f-%f", sporeNode->idx, sporeNode->location.x, sporeNode->location.y, newRenderX, newRenderY);
			-- //BBMathUtils::BBLOG("sporeNode change render pos  %i %f-%f, %f-%f", sporeNode->idx, sporeNode->mDeltaData.wrapLocationOffset.x, sporeNode->mDeltaData.wrapLocationOffset.y, deltaX, deltaY);
			sporeNode:ChangeRenderPosition(newRenderX, newRenderY);
			
			if sporeNode.cd > 0 then
				sporeNode.cd = sporeNode.cd - 1
			end
			self.gameManager.NodeTree:UpdateCircleNode(sporeNode)
		end
	end
end

function BBGameSporeManager:AddNewSporeFromServer(idx, fromId, uid, x, y, directionX, directionY, initSpeed, initDeltaSpeed, initStopFrame, cd)
	local newSpore = BBGameSporeObject:New()
	newSpore.fromId = fromId
	newSpore.idx = idx
	newSpore.uid = uid
	newSpore:ChangePosition(BBGameMathUtils.bb_int_to_float(x), BBGameMathUtils.bb_int_to_float(y))
	newSpore:ChangeRenderPosition(BBGameMathUtils.bb_int_to_float(x), BBGameMathUtils.bb_int_to_float(y))

	newSpore:SetBallMass(constant_ballsbattle_cc.BBConfigManager.sporeMass)
	newSpore:SetSpeedVec(directionX, directionY);
	newSpore.initStopFrame = initStopFrame;
	newSpore.cd = cd;
	newSpore.initSpeed = initSpeed;
	newSpore.initDeltaSpeed = initDeltaSpeed;

	newSpore:CalcBallDelta();
	self.mapSpores[newSpore.idx] = newSpore
	self.gameManager.NodeTree:AddCircleNode(newSpore);
	table.insert(self.vecSporeIds, newSpore.idx)
	table.sort(self.vecSporeIds, function(a, b) return a < b end)
end

function BBGameSporeManager:SyncShootFromServer(idx, fromId, uid, speedX, speedY, locationX, locationY)
	local  playerNode = self.gameManager:GetPlayerNode(fromId)
	if not playerNode then
		return
	end
	-- print("BBGameSporeManager 生成了一个孢子", idx, fromId, uid, speedX, speedY, locationX, locationY)
	local newSpore = BBGameSporeObject:New()
	newSpore.fromId = fromId
	newSpore.idx = idx
	newSpore.uid = uid
	newSpore:ChangePosition(BBGameMathUtils.bb_int_to_float(locationX), BBGameMathUtils.bb_int_to_float(locationY))
	newSpore:SetBallMass(constant_ballsbattle_cc.BBConfigManager.sporeMass)
	newSpore:SetSpeedVec(speedX, speedY);


	local moveVecX, moveVecY = BBGameMathUtils.GetFixedVetor2(newSpore.currentSpeedVec, playerNode:GetRadius());
	newSpore:ChangeRenderPosition(playerNode.realX + moveVecX, playerNode.realY + moveVecY);
	
	newSpore.cd = constant_ballsbattle_cc.BBConfigManager.sporeCdTime
	newSpore:CalculateInitMoveParams(newSpore:GetRadius(), constant_ballsbattle_cc.BBConfigManager.sporeStopFrame, constant_ballsbattle_cc.BBConfigManager.sporeInitSpeed)
	newSpore:CalcBallDelta();
	self.mapSpores[newSpore.idx] = newSpore
	self.gameManager.NodeTree:AddCircleNode(newSpore)
	table.insert(self.vecSporeIds, newSpore.idx)
	table.sort(self.vecSporeIds, function(a, b) return a < b end)
end