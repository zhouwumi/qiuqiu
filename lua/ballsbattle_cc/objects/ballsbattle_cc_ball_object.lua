-- local pathPng = 'ballsbattle/ball_sp'
local Move_Object = import('ballsbattle_cc_move_object').Move_Object
local ballsUtils = import('logic_data.game_ballsbattle.ballsbattle_utils')
local Live2DSprite = import('dialog/ballsbattle_cc/component/ballsbattle_cc_live_2d').Live2DSprite
-- local Service = import('logic.dialog.ballsbattle.manager.ballsbattle_service')
local balls_common_config = g_conf_mgr.get_conf("balls_common_config")
local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')

local TAG = 10000
local TAG_ROTATE = 10001

local MASS_CONFIG = {0,tonumber(balls_common_config.evolution_mass2.value),tonumber(balls_common_config.evolution_mass3.value)}
Ball,Super = CreateClass(Move_Object)

function Ball:_initInfo(info)
    Super._initInfo(self,info)
    self.uid = info.uid
    self.fuseCD = info.cd
    self.protectCD = info.protect or 0
    self.isImpactSide = {false,false,false,false}
    if self.protectCD > 0 then
        self.ballSp.sprite_protect:setVisible(true)
    else
        self.ballSp.sprite_protect:setVisible(false)
    end
    if g_user_info.get_user_info().uid == self.uid then
        self.arrowSp:setVisible(true)
    else
        self.arrowSp:setVisible(false)
    end
    self.arrowSp:setRotation(self.curRotation)
    self.cellKey = "ball"
    self.displayMode = nil
    
    self.curScale = nil
    self:changeNameScale()
end

function Ball:_initView()
    self.ballSp = g_uisystem.load_template_create('ballsbattle/ball_sp')
    self.arrowSp = self.ballSp.arrowSp
    self._spriteWidth = 50
end

function Ball:eatObject(object)
    -- if object.idx then
    --     print('eatObject',self.idx,self.mass,object.idx,object.mass)
    -- else
    --     print('eatObject',self.idx,self.mass,object.lastPositionX,object.lastPositionY)
    -- end
    self:changeMass(object.mass)
    object:dismiss(self.idx)
end

function Ball:updateInfo()
    Super.updateInfo(self)
    if self.protectCD > 0 then
        self.protectCD = self.protectCD - 1 
        if self.protectCD == 0 then
            self.ballSp.sprite_protect:setVisible(false)
            if self.uid == g_user_info.get_user_info().uid then
                g_panel_mgr.run_on_panel("ballsbattle.dlg_ballsbattle_main_panel", function(panel)
                    panel:showDirectorMessage(203)
                end)
            end
        end
    end
    if self.fuseCD >0 then
        self.fuseCD = self.fuseCD - 1
    end
end

function Ball:move()
    --不显示情况下不进行移动
    if self.isShow == false then
        return false
    end

    if self.changeSpeedCD > 0 then
        return Super.move(self)
    else
        self:cacultateNewPosition()
        -- if self.player == import('logic.dialog.ballsbattle.manager.ballsbattle_service').get_my_player() then
        --     print('cacultateNewPosition', self.lastPositionX, self.lastPositionY, self.idx,self.distanceX,self.distanceY)
        -- end
        --碰撞检测,虽然使用pair但是是有序的
        for j,ball in pairs(self.playerBallList) do
            if ball ~= self and ball.changeSpeedCD <= 0 and (self.fuseCD > 0 or ball.fuseCD > 0 or 
            (self.radius <= ball.radius * 1.05 and ball.radius <= self.radius * 1.05)) then
                self:checkImpact(ball)
            end
        end
    end
    -- if self.player == import('logic.dialog.ballsbattle.manager.ballsbattle_service').get_my_player() then
    --     print('move', self.lastPositionX, self.lastPositionY, self.idx)
    -- end
    self:changeNameScale()

    return true    
end

function Ball:cacultateNewPosition()
    -- local x = self.targetX - self.lastPositionX
    -- local y = self.targetY - self.lastPositionY
    -- if not ballsUtils.float_equal(x,0) or not ballsUtils.float_equal(y,0) then
        local lastPositionX = self.lastPositionX + self.distanceX
        local lastPositionY = self.lastPositionY + self.distanceY
        if (self.lastPositionX <= self.targetX and lastPositionX > self.targetX) or
            (self.lastPositionX >= self.targetX and lastPositionX < self.targetX) then
            lastPositionX = self.targetX
        end
        if (self.lastPositionY <= self.targetY and lastPositionY > self.targetY) or 
            (self.lastPositionY >= self.targetY and lastPositionY < self.targetY) then
            lastPositionY = self.targetY
        end
        -- if self.player ~= import('logic.dialog.ballsbattle.manager.ballsbattle_service').get_my_player() then
        -- end
        self.lastPositionX, self.lastPositionY = lastPositionX, lastPositionY
        -- print('cacultateNewPosition',lastPositionX, lastPositionY)
    -- end  
        -- if self.uid == g_user_info.get_user_info().uid then
        --     print('cacultateNewPosition',self.targetX,self.targetY,self.idx)
        -- end
end

function Ball:setTargetPoint(x,y)
    self.targetX, self.targetY = x,y
    x,y = self.targetX - self.lastPositionX,self.targetY - self.lastPositionY
    if not ballsUtils.float_equal(x,0) or not ballsUtils.float_equal(y,0) then
        self.radian = ballsUtils.get_radian_by_vector(x, y)
        self.distanceX,self.distanceY = ballsUtils.get_vector_by_rad(self.radian,self.maxSpeed)
        self.targetRotation = 360 - math.deg(self.radian)
    else
        --速度为零的情况下，方向不发生改变
        self.distanceX,self.distanceY = 0,0
    end
    -- print('self.distanceX,self.distanceY',self.idx,self.distanceX,self.distanceY,self.targetX,self.targetY)
end

function Ball:onRadianChange()
    --角度变化过小，不进行计算
    local delta = math.abs(self.curRotation - self.targetRotation)
    if delta < 1 then
        return
    end
    delta = math.abs(self.arrowSp:getRotation() - self.targetRotation)
    if delta > 180 then
        delta = 360 - 180
    end
    local time = delta / 720
    self.curRotation = self.targetRotation
    self.arrowSp:stopActionByTag(TAG_ROTATE)
    local rotateTo = cc.RotateTo:create(time, self.targetRotation)
    rotateTo:setTag(TAG_ROTATE)
    self.arrowSp:runAction(rotateTo)
end

function Ball:changeAvator()
    local state = 1
    for i,mass in ipairs(MASS_CONFIG) do
        if self.player.avatarLevel < i then
            break
        end
        if self.mass > mass then
            state = i
        end
    end
    if self.massState ~= state then
        self.massState = state
        for i,mass in pairs(MASS_CONFIG) do
            local visible = false
            if i == self.massState then
                visible = true
            end

            for j,sp in pairs(self.avatorSpList[i]) do
                sp:setVisible(visible)
            end
        end
    end
end

function Ball:changeScale(delay)
    -- if self.player == import('logic.dialog.ballsbattle.manager.ballsbattle_service').get_my_player() then
    --     print('self.radius / self._spriteWidth',self.radius / self._spriteWidth)
    --     print('self.radius',self.radius)
    -- end
    if delay then
        self.ballSp:stopActionByTag(TAG)
        local scaleTo = cc.ScaleTo:create(delay, self.radius / self._spriteWidth)
        scaleTo:setTag(TAG)
        self.ballSp:runAction(scaleTo)
    else
        Super.changeScale(self)
    end
end

function Ball:changeMass(mass)
    self.mass = self.mass + mass
    -- print('changeMass',mass,self.idx,self.mass)
    self.player:changeMass(mass)

    self:caculateRadius()

    self:changeScale(0.3)
    self:caculateSpeed()
    self:changeZOrder()
    self:changeAvator()

    if self.uid == g_user_info.get_user_info().uid and self.mass > 100 then
        g_panel_mgr.run_on_panel("ballsbattle.dlg_ballsbattle_main_panel", function(panel)
            panel:showDirectorMessage(202)
        end)
    end
end

function Ball:checkImpact(object)
    local x,y = object.lastPositionX - self.lastPositionX,object.lastPositionY - self.lastPositionY
    if x == 0 and y == 0 then
        return
    end

    local distancePow = math.pow(x,2) + math.pow(y,2)
    if distancePow < math.pow(object.radius + self.radius,2) then
        local distance = object.radius + self.radius - math.sqrt(distancePow)
        local radian = ballsUtils.get_radian_by_vector(x, y)
        x, y = ballsUtils.get_vector_by_rad(radian, distance)
        if self.lastPositionX <= object.lastPositionX then
            self.lastPositionX = self.lastPositionX - math.abs(x)
        elseif self.lastPositionX > object.lastPositionX then
            self.lastPositionX = self.lastPositionX + math.abs(x)
        end

        if self.lastPositionY <= object.lastPositionY then
            self.lastPositionY = self.lastPositionY - math.abs(y)
        elseif self.lastPositionY > object.lastPositionY then
            self.lastPositionY = self.lastPositionY + math.abs(y)
        end
    end
end

function Ball:resetFuseCD()
    self.fuseCD = ballsUtils.math_get_int(self.mass * 0.4 * 30)
end

function Ball:addToParent(container)
    container:addChild(self.ballSp)
end

function Ball:setPlayer(player)
    if self.avatorSp == nil then
        avatorSp = Live2DSprite:New(player:getPlayerAvator(),'breathe')
        self.avatorSp = avatorSp
        self.ballSp.player_avator:removeAllChildren()
        self.ballSp.player_avator:addChild(self.avatorSp.sp)
        self.ballAvatorConfig = constant_ballsbattle.BALL_AVATOER_CONFIG[player:getPlayerAvator()]

        --把相应组件加入数组，利用命名和配置表组装
        self.avatorSpList = {}
        for i,info in pairs(self.ballAvatorConfig) do
            local avatorTable = {}
            for j,key in pairs(info) do
                table.insert(avatorTable,self.ballSp[key])
            end
            table.insert(self.avatorSpList,avatorTable)
        end
    end
    self.player = player

    -- if g_native_conf['debug_control'].balls_debug_mode then
    --     self.ballSp.userName:SetString("idx"..self.idx)
    -- else
        self.ballSp.userName:SetString(player:getPlayerDisPlayName())
    -- end
    
    self.ballInterpolation:setAvatorSp(self.avatorSp)
    self.playerBallList = self.player._ballList

    local pathPng = 'gui/badam_ui/ballsbattle/ball/'..self.player:getPlayerAvator()
    for i,list in pairs(self.ballAvatorConfig) do
        if player.avatarLevel >= i then
            for j,name in pairs(list) do
                self.ballSp[name]:SetPath(nil,pathPng..'/'..name..'.png')
            end
        else
            for j,name in pairs(list) do
                self.ballSp[name]:setVisible(false)
            end
        end
    end
    self:changeAvator()
    -- Service.get_grid_manager():addObject(self)
end

function Ball:synPosition(location)
    local x, y = ballsUtils.get_positon_by_locaiton(location)
    -- local tx, ty = ballsUtils.get_positon_by_locaiton(point)

    -- if self.lastPositionX ~= x or self.lastPositionY ~= y then
         -- if tx == self.targetX and  ty == self.lastPositionX and self.player == import('logic.dialog.ballsbattle.manager.ballsbattle_service').get_my_player() then
         --    __G__TRACKBACK__('坐标出现误差')
            -- print('lastPosition',x,y,self.lastPositionX,self.lastPositionY,self.idx)
         --    -- -- -- print('idx',self.idx,self.changeSpeedCD)
         --    print('x',tx,ty,self.targetX,self.targetY,self.mass,mass)
         -- end
        self.lastPositionX, self.lastPositionY = x, y
        -- self:updatePosition()
    -- end
    -- if self.player == import('logic.dialog.ballsbattle.manager.ballsbattle_service').get_my_player() then
    -- print('sdfsfdsfdsf',self.idx)
    -- end
    -- print('xxxxxx',self.lastPositionX,self.lastPositionY)
    -- Service.get_grid_manager():updateOjectPosition(self)
end

function Ball:getDisplayPosition()
   return self.ballInterpolation:getDisplayPosition()
end

function Ball:updateMotiton(ratio, ballObject)
    if self.displayMode == 1 and self.motion ~= 'breathe' then
        return
    end
   self.ballInterpolation:updateMotiton(ratio, ballObject)
end

function Ball:checkSpore(object)
   self.ballInterpolation:checkSpore(object)
end

function Ball:setMotion(motion)
   self.ballInterpolation:setMotion(motion)
end

function Ball:checkScale(ratio)
    local mode
    if self.radius < 20/ratio then
        mode = 1
    else
        mode = 2
    end

    if self.displayMode ~= mode then
        if mode == 1 then
            self.ballSp.userName:setVisible(false)
        else 
            self.ballSp.userName:setVisible(true)
        end
        self.ballInterpolation.avatorSp:ChangeMode(mode)
        self.displayMode = mode
    end
end


-- function Ball:updatePosition(dtPercent)
--    --做插值运算
--     self.ballInterpolation:setPosition(self.lastPositionX, self.lastPositionY,dtPercent)
--     -- self:changeNameScale(ratio)
-- end

function Ball:changeNameScale()
    ratio = direct_import('logic.dialog.ballsbattle.manager.ballsbattle_service').cur_scale
    local scale = self.ballSp:getScale()
    if scale ~= self.curScale and self.displayMode ~= 1 then
        self.curScale = scale
        self.ballSp.userName:setScale(0.8 / (ratio * self.curScale))
    end
end

function Ball:show()
    self.isShow = true
    self.ballSp:setVisible(true)
    self.player.showBallCount = self.player.showBallCount + 1
end

function Ball:hide()
    if self.uid ~= g_user_info.get_user_info().uid  then
        self.isShow = false
        self.ballSp:setVisible(false)
    end

    self.player.showBallCount = self.player.showBallCount - 1
end

function Ball:dismiss()
    --从场景网格中删除    
    -- for i=#self.cellList,1,-1 do
    --     local info = self.cellList[i]
    --     self:removeCell(info)
    -- end
    self.rectTable = nil
    self.ballInterpolation:reset()
    --延迟一帧设置隐藏
    self.ballSp:DelayCall(0,function()
        self.ballSp:setVisible(false)
    end)
    self.canUse = true

    self.player.showBallCount = self.player.showBallCount - 1
end