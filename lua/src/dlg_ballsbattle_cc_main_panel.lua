Panel = g_panel_mgr.new_panel_class('ballsbattle_cc/ballsbattle_cc_main_panel')

local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local Live2DSprite = import('ballsbattle_cc_live_2d').Live2DSprite

local Type_PlayerNode = 1
local Type_Food = 2
local Type_Spiky = 3

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
    self.hitHandle = manager:SetEatCbFunction(function(removeObjType, removeObjIdx, eatObjType, eatObjIdx)
        self:_onEatObj(removeObjType, removeObjIdx, eatObjType, eatObjIdx)
    end)

    self.foodContainer = cc.Node:create()
    self.bg_container:AddChild(nil, self.foodContainer)

    self.spikyContainer = cc.Node:create()
    self.bg_container:AddChild(nil, self.spikyContainer)

    self.playerContainer = cc.Node:create()
    self.bg_container:AddChild(nil, self.playerContainer)

    self.bgLayer.OnBegin = function(pos)
        print(pos)
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
    self.btnStartMove.OnClick = function()
        self:StartMovePlayer()
    end

    self.currentPlayerIdx = 1

    local frame = 0
    self._delayCall = delay_call(1 / 30, function()
        frame = frame + 1
        self.frame = frame
        local time1 = utils_get_tick()
        self.manager:OnUpdate()
        local time2 = utils_get_tick()

        if self.currentPlayerNode then
            local playerNodeIdxs = self.manager:GetPlayerNodeIdx(self.currentPlayerIdx)
            -- print("playerNodeIdxs:  ", playerNodeIdxs)
            for _, idx in ipairs(playerNodeIdxs) do
                local x, y, radius = self.manager:GetPlayerNodeInfo(idx)
                self.currentPlayerNode:SetPosition(x, y)
            end
        end
        -- self:_updateBgPosition()
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
    -- print("playerNodeIdxs:  ", playerNodeIdxs)
    for _, idx in ipairs(playerNodeIdxs) do
        local x, y, radius = self.manager:GetPlayerNodeInfo(idx)
        -- print(x, y, radius)
        local node = cc.Node:create()
        local avatorSp = Live2DSprite:New("ball_1",'breathe')
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
    local foodTemplate = g_uisystem.load_template('ballsbattle_cc/balls_food')
    for _, value  in ipairs(allFoods) do
        local food = g_uisystem.create_item(foodTemplate)
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

    self.spikyMap = {}
    local spikyTemplate = g_uisystem.load_template('ballsbattle_cc/balls_spiky')
    local allSpikys = self.manager:GetAllSpikyInfos()
    for _, value in ipairs(allSpikys) do
        local x, y, radius = self.manager:GetSpikyInfo(value)
        local node = g_uisystem.create_item(spikyTemplate)
        node:SetPosition(x, y)
        node:setScale(radius / 50)
        self.spikyContainer:AddChild(nil, node)
        node.nodeIdx = value
        node.mask.OnClick = function()
            print("cick spliky ", value)
            node.label:setVisible(not node.label:isVisible())
        end
        node.label:SetString(tostring(value))
        self.spikyMap[value] = node
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

function Panel:StartMovePlayer()
    self.manager:StartMovePlayer(self.currentPlayerIdx)
end

function Panel:_onPlayerPosChange()
    -- local hitResults = self.manager:CheckPlayerHit(self.currentPlayerIdx)
    -- print(hitResults)
end

function Panel:_checkPlayerHit()
    
end


function Panel:_onEatObj(removeObjType, removeObjIdx, eatObjType, eatObjIdx)
    print("eat obj frame: ", self.frame, "obj ", removeObjIdx, " eat by player ", eatObjIdx)

    if removeObjType == Type_Food  then
        if not self.foodMap[removeObjIdx] then
            error("eat food but lua has not this food idx:  ", removeObjIdx)
        else
            local food = self.foodMap[removeObjIdx]
            self.foodMap[removeObjIdx] = nil
            food:removeFromParent()
        end
    elseif removeObjType == Type_Spiky then
        if not self.spikyMap[removeObjIdx] then
            error("eat spiky but lua has not this spiky idx:  ", removeObjIdx)
        else
            local spiky = self.spikyMap[removeObjIdx]
            self.spikyMap[removeObjIdx] = nil
            spiky:removeFromParent()
        end
    end
    
end

function Panel:on_before_destroy()
    if cc.BBGameManager then
        if self.hitHandle then
            cc.BBGameManager:getInstance():RemoveEatCbFunction(self.hitHandle)
            self.hitHandle = nil
        end
        
        cc.BBGameManager:getInstance():destroyInstance()
    end
    if self._delayCall then
        self._delayCall('cancel')
        self._delayCall = nil
    end
end