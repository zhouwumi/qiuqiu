BallsBattleCCPlayerManager = CreateClass()

local Food_Object = import('logic.dialog.ballsbattle_cc.objects.ballsbattle_food_object').Food_Object
local Spiky_Object = import('logic.dialog.ballsbattle_cc.objects.ballsbattle_spiky_object').Spiky_Object
local Object = import('logic.dialog.ballsbattle_cc.objects.ballsbattle_object').Object
local Spore = import('logic.dialog.ballsbattle_cc.objects.ballsbattle_spore_object').Spore
local Ball = import('logic.dialog.ballsbattle_cc.objects.ballsbattle_ball_object').Ball
local Player = import('logic.dialog.ballsbattle_cc.objects.ballsbattle_player').Player

function BallsBattleCCPlayerManager:__init__(mainPanel)
	self._mainPanel = mainPanel

	self._bgContainer = mainPanel.bg_container
	self.foodBatchNode = mainPanel.bg_container
	self.isReady = false
end

function BallsBattleCCPlayerManager:OnRestartGame()
	self.isReady = false
end

function BallsBattleCCPlayerManager:OnGetGameData()
	self._spikyList = {}
    self._removeSpikyList = {}
    self._foodList = {}
    self._removeFoodList = {}
    self._ballList = {}
    self._removeBallList = {}
    self._sporeList = {}
    self._removeSporeList = {}
    
    self._playerList = {}
    self._playerSortList = {}
    self._killInfoList = {}
    self._cacheLive2d = {}

    self:_addPlayer(data.players)
    self:_addMoveObjct(data.nodes, nil, true, true)
    self.myPlayer = self._playerList[g_user_info.get_user_info().uid]
    self:_addMoveObjct(data.spores, true, true, true)

    for i,info in pairs(data.spiky_ball) do
        g_async_task_mgr.add_task(self._addSpiky, self, info)
    end

    for i,positionInfo in pairs(data.foods) do
        g_async_task_mgr.add_task(self._addFood, self, positionInfo)
    end

    --虚拟食物，前端显示
    -- self:_simulatFood()
    g_async_task_mgr.add_task(function()
    	self.isReady = true
    end)
end

function BallsBattleCCPlayerManager:IsReady()
	return self.isReady
end

function BallsBattleCCPlayerManager:OnUpdate()

end

function BallsBattleCCPlayerManager:Destory()
end

function BallsBattleCCPlayerManager:GetMyPlayer()
end

function BallsBattleCCPlayerManager:GetBall(idx)
end


function BallsBattleCCPlayerManager:_getUnuseObject(list,info)
    for i,object in pairs(list) do
        if object.canUse then
            object:reset(info)
            return object
        end
    end
end

function BallsBattleCCPlayerManager:_addSpiky(info)
    local newObject = self:_getUnuseObject( self._removeSpikyList,info)
    if newObject == nil then
        newObject = Spiky_Object:New(info)
        newObject:addToParent(self._bgContainer)
    end
    self._spikyList[info.idx] = newObject
end

--添加移动物体（包括球体，孢子）
function BallsBattleCCPlayerManager:_addMoveObjct(nodesList, isSpore, noLossMass, isAnys)
    local createFunc = function(info)
        local newObject 
        if not isSpore then
            local player = self._playerList[info.uid]
            --后续的新球，优先从内存池里面取出
            local cacheBalls = self._removeBallList[player:getPlayerAvator()]
            if not isAnys and cacheBalls then
                newObject = self:_getUnuseObject(cacheBalls,info)
            end
            if newObject == nil then
                newObject = Ball:New(info)
                newObject:addToParent(self._bgContainer)
            end
            player:addBall(newObject)
            self._ballList[info.idx] = newObject
        else
            if not isAnys then
                newObject = self:_getUnuseObject(self._removeSporeList,info)
            end
            if newObject == nil then
                newObject = Spore:New(info)
                newObject:addToParent(self.foodBatchNode)
            end
            self._sporeList[info.idx] = newObject
        end
        local fromBall = self._ballList[info.from_id]
        if fromBall ~= nil then
            if not noLossMass and fromBall then
                local mass = isSpore and 10 or newObject.mass
                fromBall:changeMass(mass * -1)
            end
            newObject.fromUid = fromBall.uid
        end
    end

    for i,info in pairs(nodesList) do
        if isAnys then
            g_async_task_mgr.add_task(createFunc,info)
        else
            createFunc(info)
        end
    end
end

function BallsBattleCCPlayerManager:_addPlayer(playerInfoList)
    for i,info in pairs(playerInfoList) do
        if self._playerList[info.uid] == nil then
            local newPlayer = Player:New(info)
            self._playerList[info.uid] = newPlayer
            table.insert(self._playerSortList, newPlayer)
        end
    end
end

function BallsBattleCCPlayerManager:_addFood(positionInfo)
    local newObject = Food_Object:New({location = positionInfo})
    newObject:addToParent(self.foodBatchNode)
    self._foodList[positionInfo] = newObject
end

function BallsBattleCCPlayerManager:update(isJump)
end