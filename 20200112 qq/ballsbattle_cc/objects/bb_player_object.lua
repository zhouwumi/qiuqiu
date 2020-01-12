PlayerObject = CreateClass()

local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')

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

function PlayerObject:Update()
	local playerNodeIdxs = self._gameManager:GetPlayerNodeIdx(self.uid)
	local playerNodeCount = #playerNodeIdxs
	for _, idx in ipairs(playerNodeIdxs) do
	    local playerNode = self.allPlayerNodes[idx]
	    playerNode:Update(playerNodeCount > 0)
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
    end
    return startx, starty, endx ,endy
end

function PlayerObject:PlayerStartSimulate(startSimulateFrame)
	for _, playerNode in pairs(self.allPlayerNodes or {}) do
		playerNode:NodeStartSimulate(startSimulateFrame)
	end
end

function PlayerObject:PlayerStopSimulate(endSimulateFrame)
	for _, playerNode in pairs(self.allPlayerNodes or {}) do
		playerNode:NodeStopSimulate(endSimulateFrame)
	end
end