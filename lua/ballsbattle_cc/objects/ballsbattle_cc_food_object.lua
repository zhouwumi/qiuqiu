local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local Object = import('ballsbattle_cc_object').Object
local ballsUtils = import('logic_data.game_ballsbattle.ballsbattle_utils')
Food_Object,Super = CreateClass(Object)
--zorder 1到100为食物
local zOrder = 1

function Food_Object:_initView(pathPng)
    self.colorIndex = math.random(1,3)
    self.shapeIndex = math.random(1,2)    

    local shape = cc.Sprite:Create(constant_ballsbattle.SLIME_FOOD_PLIST,'ball_food/food_'..self.colorIndex..'_'..self.shapeIndex..'.png')
    local bg = cc.Sprite:Create(constant_ballsbattle.SLIME_FOOD_PLIST,'ball_food/food_bg_'..self.colorIndex..'.png')
    bg:DelayCall(math.random() * 2, function() 
        local opacityRandom = math.random(100,255)
        bg:setOpacity(opacityRandom)
        local FadeTo = cc.FadeTo:create(1, 0)
        local FadeTo2 = cc.FadeTo:create(1, opacityRandom)
        local repeatForever = cc.RepeatForever:create(cc.Sequence:create(FadeTo, FadeTo2))
        bg:runAction(repeatForever)
    end)

    shape:addChild(bg,-1)
    bg:setPosition(6.5,8)
    self.ballSp = shape
end

function Food_Object:caculateRadius()
    self.radius = 0
end
                
function Food_Object:dismiss(idx)
    -- local BallsBattleService = direct_import('logic.dialog.ballsbattle.manager.ballsbattle_service')
    -- local ball = BallsBattleService.get_ball_by_index(idx)
    local playerManager = g_panel_mgr.get_panel("dialog.ballsbattle_cc.dlg_ballsbattle_cc_main_panel").playerManager
    if not playerManager then
        return
    end
    if not self.isShow or ball.protectCD <= 0 then
        Super.dismiss(self)
        return 
    end
    local ball = playerManager:GetBall(idx)
    --播放飞动画

    local speed = 20 
    self.ballSp:DelayCall(0,function()
        if ball then
            local x,y = ball:getDisplayPosition()
            local distancex = x - self.lastPositionX
            local distancey = y - self.lastPositionY
            local distancePow = math.pow(distancex,2) + math.pow(distancey,2)
            if distancePow > math.pow(ball.radius, 2) then
                local radian = ballsUtils.get_radian_by_vector(distancex, distancey)
                distancex,distancey = ballsUtils.get_vector_by_rad(radian, speed)
                self.lastPositionX = self.lastPositionX + distancex
                self.lastPositionY = self.lastPositionY + distancey
                self.ballSp:setPosition(self.lastPositionX, self.lastPositionY)
                speed = speed + 1
                return 0.01
            end
            Super.dismiss(self)
        else
            Super.dismiss(self)
        end
    end)
end

function Food_Object:changeZOrder()

end

function Food_Object:changeScale()
    -- self.ballSp:setScale()
end