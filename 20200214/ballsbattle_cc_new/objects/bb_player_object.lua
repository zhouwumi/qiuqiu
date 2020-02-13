PlayerObject = CreateClass()

local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils

function PlayerObject:__init__(mainPanel, uid)
	self._mainPanel = mainPanel
	self._gameManager = mainPanel.gameManager
	self.uid = uid
	self.allPlayerNodes = {}
end

function PlayerObject:AddPlayerNode(playerNode)
	self.allPlayerNodes[playerNode.objectIdx] = playerNode
	playerNode.playerObject = self
end

function PlayerObject:RemovePlayerNode(playerNodeIdx)
	local playerNode = self.allPlayerNodes[playerNodeIdx]
	if  playerNode then
		playerNode:Remove()
		self.allPlayerNodes[playerNodeIdx] = nil
	end
end

function PlayerObject:NewBirth()
	self.isNewBirth = true
	delay_call(0, function()
		self.isNewBirth = false
	end)
end

function PlayerObject:IsNewBirth()
	return self.isNewBirth
end

function PlayerObject:IsDead()
	return table.count(self.allPlayerNodes) <= 0
end

function PlayerObject:Update()
	for _, playerNode in pairs(self.allPlayerNodes) do
		playerNode:Update()
	end
end


function PlayerObject:PrintPostionInfo()
	local playerNodeIdxs = self._gameManager:GetPlayerNodeIdx(self.uid)
	-- print(playerNodeIdxs)
	for _, idx in ipairs(playerNodeIdxs) do
	    local playerNode = self.allPlayerNodes[idx]
	    playerNode:PrintPostionInfo()
	end
end

function PlayerObject:GetRectCenter()
	-- for _, playerNode in pairs(self.allPlayerNodes) do
	-- 	return playerNode.displayObject:GetPosition()
	-- end
	local startx, starty = constant_ballsbattle.BG_SIZE[1],constant_ballsbattle.BG_SIZE[2]
    local endx, endy = 0,0
    for i, ballObject in pairs(self.allPlayerNodes) do
        local x, y = ballObject:GetDisplayPosition()
        startx = math.min(startx, x - ballObject.radius)
        endx = math.max(endx, x + ballObject.radius)
        starty = math.min(starty, y - ballObject.radius)
        endy = math.max(endy, y + ballObject.radius)
        -- print(ballObject.radius, x, y)
    end
    -- print(startx, starty, endx, endy)
    return startx, starty, endx ,endy
end
