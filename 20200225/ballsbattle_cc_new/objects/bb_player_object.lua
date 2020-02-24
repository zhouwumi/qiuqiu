PlayerObject = CreateClass()

local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils

function PlayerObject:__init__(mainPanel)
	self._mainPanel = mainPanel
	self.allPlayerNodes = {}
    self.mass = 0
end

function PlayerObject:AddPlayerNode(playerNode)
	self.allPlayerNodes[playerNode.objectIdx] = playerNode
	playerNode.playerObject = self
end

function PlayerObject:RemovePlayerNode(playerNodeIdx)
	local playerNode = self.allPlayerNodes[playerNodeIdx]
	if  playerNode then
		playerNode:TryRemove(function()
			self.allPlayerNodes[playerNodeIdx] = nil
		end)
	end
end

function PlayerObject:GetTotalMass()
    return self.mass
end

function PlayerObject:OnJoinGame(uid)
	self.isNewBirth = true
	self.uid = uid
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
    local mass = 0
	for _, playerNode in pairs(self.allPlayerNodes) do
		playerNode:Update()
        mass = playerNode.mass
	end
    self.mass = mass
end

function PlayerObject:GetRectCenter()
	-- for _, playerNode in pairs(self.allPlayerNodes) do
	-- 	return playerNode.displayObject:GetPosition()
	-- end
	local startx, starty = constant_ballsbattle.BG_SIZE[1],constant_ballsbattle.BG_SIZE[2]
    local endx, endy = 0,0
    for i, ballObject in pairs(self.allPlayerNodes) do
    	if ballObject:IsValid() then
    		local x, y = ballObject:GetDisplayPosition()
    		startx = math.min(startx, x - ballObject.radius)
    		endx = math.max(endx, x + ballObject.radius)
    		starty = math.min(starty, y - ballObject.radius)
    		endy = math.max(endy, y + ballObject.radius)
    	end
        -- print(ballObject.radius, x, y)
    end
    -- print(startx, starty, endx, endy)
    return startx, starty, endx ,endy
end

function PlayerObject:CheckBoardHit()
	local isShow = false
    local moviesTable = {}
    for i, myBallObject in pairs(self.allPlayerNodes) do
        local moviePos = {-1, -1, -1, -1}
        local isImpact = {false,false,false,false}
        local x,y = myBallObject:GetDisplayPosition()
        if x - myBallObject.radius < 320 then
            isShow = true
            if x - myBallObject.radius < 1 then
                moviePos[1] = y
                isImpact[1] = true
            end 
        end
        
        if y - myBallObject.radius < 320 then
            isShow = true
            if y - myBallObject.radius < 1 then
                moviePos[2] = x
                isImpact[2] = true
            end
        end
        
        if y + myBallObject.radius > constant_ballsbattle.BG_SIZE[2] - 320 then
            isShow = true
            if y + myBallObject.radius > constant_ballsbattle.BG_SIZE[2] - 1 then
                moviePos[3] = x
                isImpact[3] = true
            end
        end

        if x + myBallObject.radius  > constant_ballsbattle.BG_SIZE[1] - 320 then
            isShow = true
            if x + myBallObject.radius > constant_ballsbattle.BG_SIZE[2] - 1 then
                moviePos[4] = y
                isImpact[4] = true
            end
        end
        for i,isImpact in pairs(isImpact) do
        	if not myBallObject.isImpactSide then
        		myBallObject.isImpactSide = {}
        	end
            if myBallObject.isImpactSide[i] ~= isImpact then
                myBallObject.isImpactSide[i] = isImpact
            else
                moviePos[i] = -1
            end
        end
        table.insert(moviesTable,moviePos)
    end
   return isShow, moviesTable
end