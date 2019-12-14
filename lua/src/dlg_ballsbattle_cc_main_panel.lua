Panel = g_panel_mgr.new_panel_class('ballsbattle_cc/ballsbattle_cc_main_panel')

local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local Live2DSprite = import('ballsbattle_cc_live_2d').Live2DSprite

function Panel:init_panel()
    self.winW, self.winH = self.bgLayer:GetContentSize()
	self.bgLayer:SetEnableCascadeOpacityRecursion(true)
    self:_initGame()
end

function Panel:_initGame()
    if not cc.BBGameManager then
        return
    end
    local manager = cc.BBGameManager:getInstance()
    self.manager = manager
    manager:InitRoom()
    manager:InitFood()
    self.hitHandle = manager:SetEatFoodCbFunction(function(foodIdx)
        self:_onEatFood(foodIdx)
    end)

    self.foodContainer = cc.Node:create()
    self.bg_container:AddChild(nil, self.foodContainer)

    self.playerContainer = cc.Node:create()
    self.bg_container:AddChild(nil, self.playerContainer)

    self.bgLayer.OnBegin = function(pos)
        self:_tryMovePlayer(pos)
        return true
    end

    self.bgLayer.OnDrag = function(pos)
        self:_tryMovePlayer(pos)
    end

    self.btnSpawnFood.OnClick = function()
        self:_generateFood()
    end
    self.btnSpawnPlayer.OnClick = function()
        self._hasAddPlayer = true
        self:_testAddPlayer()
    end

    self.btnDeletePlayer.OnClick = function()
        if self._hasAddPlayer then
            self:_removePlayer()
            
            self._hasAddPlayer = false
        end
    end
    self.btnCheckPlayerHit.OnClick = function()
        self:_checkPlayerHit()
    end
    
    self.btnControPlayer.OnClick = function()
        self:_controllerPlayer()
    end
    self.currentPlayerIdx = 1

    local frame = 0
    self._delayCall = delay_call(1 / 30, function()
        frame = frame + 1
        self.frame = frame
        local time1 = utils_get_tick()
        self.manager:OnUpdate()
        local time2 = utils_get_tick()
        self:_updateBgPosition()
        return 1 / 30
    end)
end

function Panel:_updateBgPosition()
    local bgContainerW, bgContainerH = self.bg_container:GetContentSize()
    local minX = self.winW - bgContainerW
    local maxX = 0
    local minY = self.winH - bgContainerH
    local maxY = 0

    if self.currentPlayerNode then
        local x, y = self.currentPlayerNode:GetPosition()
        local bgX = self.winW / 2 - x
        local bgY = self.winH / 2 - y
        bgX = math.max(minX, math.min(maxX, bgX))
        bgY = math.max(minY, math.min(maxY, bgY))
        self.bg_container:SetPosition(bgX, bgY)
    end
end

function Panel:_testAddPlayer()
	self.manager:CreatePlayer(self.currentPlayerIdx)
    local playerNodeIdxs = self.manager:GetPlayerNodeIdx(self.currentPlayerIdx)
    print("playerNodeIdxs:  ", playerNodeIdxs)
    for _, idx in ipairs(playerNodeIdxs) do
        local x, y, radius = self.manager:GetPlayerNodeInfo(idx)
        print(x, y, radius)
        local node = cc.Node:create()
        local avatorSp = Live2DSprite:New("ball_1",'breathe')
        print(avatorSp == nil)
        node:AddChild(nil, avatorSp.sp)
        node:SetPosition(x, y)
        node:setScale(radius / 50)
        self.playerContainer:AddChild(nil, node)
        self.currentPlayerNode = node
        node.nodeIdx = idx
    end
end

function Panel:_removePlayer()
    if self.currentPlayerNode then
        self.currentPlayerNode:removeFromParent()
        self.currentPlayerNode = nil
        self.manager:RemovePlayer(self.currentPlayerIdx)
    end
end

function Panel:_generateFood()
    local allFoods = self.manager:GetAllFoodInfos()
    self.foodMap = {}
    for _, value  in ipairs(allFoods) do
        local food = g_uisystem.load_template_create('ballsbattle_cc/balls_food')
        food.bg:DelayCall(math.random() * 2, function() 
            local opacityRandom = math.random(100,255)
            food.bg:setOpacity(opacityRandom)
            local FadeTo = cc.FadeTo:create(1, 0)
            local FadeTo2 = cc.FadeTo:create(1, opacityRandom)
            local repeatForever = cc.RepeatForever:create(cc.Sequence:create(FadeTo, FadeTo2))
            food.bg:runAction(repeatForever)
        end)
        food.mask.OnClick = function()
            food.label:setVisible(not food.label:isVisible())
        end
        local x = value % 4500
        local y = (value - x ) / 4500
        food:setPosition(x, y)
        self.foodContainer:AddChild(nil, food)
        local idx = self.manager:GetFoodIdxByPos(value)
        food.label:SetString(tostring(idx))
        self.foodMap[idx] = food
    end
end



function Panel:_controllerPlayer()
    self.isControling = not self.isControling
    if self.isControling then
        self.btnControPlayer:SetText("停止控制玩家")
    else
        self.btnControPlayer:SetText("开始控制玩家")
    end
end

function Panel:_tryMovePlayer(pos)
    if self.isControling then
        local localPos = self.bg_container:convertToNodeSpace(pos)
        if self.currentPlayerNode then
            self.currentPlayerNode:SetPosition(localPos.x, localPos.y)
            self.manager:ChangePlayerNodePos(self.currentPlayerNode.nodeIdx, localPos.x, localPos.y)
        end
        
        self:_onPlayerPosChange()
    end
end

function Panel:_onPlayerPosChange()
    -- local hitResults = self.manager:CheckPlayerHit(self.currentPlayerIdx)
    -- print(hitResults)
end

function Panel:_checkPlayerHit()
    
end

function Panel:_onEatFood(foodIdx)
    print("eat food frame: ", self.frame, " foodIdx: ", foodIdx)
    if not self.foodMap[foodIdx] then
        error("eat food but lua has not this food idx:  ", foodIdx)
    else
        local food = self.foodMap[foodIdx]
        self.foodMap[foodIdx] = nil
        food:removeFromParent()
    end
end

function Panel:on_before_destroy()
    if cc.BBGameManager then
        if self.hitHandle then
            cc.BBGameManager:getInstance():RemoveEatFoodCbFunction(self.hitHandle)
            self.hitHandle = nil
        end
        
        cc.BBGameManager:getInstance():destroyInstance()
    end
    if self._delayCall then
        self._delayCall('cancel')
        self._delayCall = nil
    end
end