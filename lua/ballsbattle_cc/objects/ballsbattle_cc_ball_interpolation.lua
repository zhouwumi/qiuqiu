Ball_Interpolation = CreateClass()
local ballsUtils = import('logic_data.game_ballsbattle.ballsbattle_utils')
local winSize = g_director:getWinSize()
local TAG = 10001

function Ball_Interpolation:__init__(ballObject)
    self.ballObject = ballObject
    self.isPlayingMovie = false 
    self.motion = 'breathe'
end

function Ball_Interpolation:initPosition()
    self.x = self.ballObject.lastPositionX
    self.y = self.ballObject.lastPositionY
    self.ballObject.ballSp:setPosition(self.x, self.y)
    --追影子速度逐渐提高
    self.speedRadio = 1
end

function Ball_Interpolation:setPosition(x,y,dtPercent)
    -- dtPercent = 1
    if self.ballObject.ballSp:isVisible() == false then
        self.x = x
        self.y = y
        self.beforeHide = true
        self.ballObject.ballSp:setPosition(self.x, self.y)
        self.speedRadio = 1
        return
    end

    if self.beforeHide then
        self.beforeHide = false
        self.x = x
        self.y = y
        self.speedRadio = 1
    else
        local maxSpeed = self.ballObject.maxSpeed
        local distancex = x - self.x
        local distancey = y - self.y
        --位置不发生改变的情况
        if distancex == 0 and distancey == 0 then
            self.speedRadio = 1
            return 
        end
        
        if self.ballObject.changeSpeedCD > 0 then
            --分裂情况下的速度
            maxSpeed = 30
        end

        local distancePow = math.pow(distancex,2) + math.pow(distancey,2)
        local maxDistance = maxSpeed * self.speedRadio * dtPercent
        if distancePow > math.pow(maxDistance, 2) and distancePow < 100000 then
            local radian = ballsUtils.get_radian_by_vector(distancex, distancey)
            distancex,distancey = ballsUtils.get_vector_by_rad(radian, maxDistance)
            self.x = self.x + distancex
            self.y = self.y + distancey
            self.speedRadio = math.min(1.5,self.speedRadio + 0.01)
        else
            self.speedRadio = 1
            self.x = x
            self.y = y
        end
    end
    self.ballObject.ballSp:setPosition(self.x, self.y)
end

function Ball_Interpolation:getDisplayPosition()
    return self.x, self.y
end

function Ball_Interpolation:setAvatorSp(avatorSp)
    self.avatorSp = avatorSp
end

function Ball_Interpolation:checkSpore(object)
    -- local distancePow = math.pow(object.lastPositionX - self.x,2) + math.pow(object.lastPositionY - self.y,2)
    -- if distancePow < math.pow(self.ballObject.radius , 2) then
    --     object:simulateBeEat()
    -- end
end

function Ball_Interpolation:updateMotiton(ratio, ballObject)
    -- if math.abs(self.ballObject.radius - ballObject.radius) > math.min(self.ballObject.radius, ballObject.radius) * 0.05 then
    --     local x,y = ballObject:getDisplayPosition()
    --     local distancePow = math.pow(x - self.x,2) + math.pow(y - self.y,2)
    --     if distancePow < math.pow(ballObject.radius + self.ballObject.radius + 50 * 1/ratio, 2) then
    --         if self.ballObject.radius > ballObject.radius then
    --             self:setMotion('happy')
    --             ballObject:setMotion('fear')
    --         else
    --             self:setMotion('fear')
    --             ballObject:setMotion('happy')
    --         end
    --     end
    -- end
end

function Ball_Interpolation:setMotion(motion)
    if not self.isPlayingMovie then
        self:playMotion(motion)
    elseif self.motion ~= 'fear' and motion == 'fear' then
        self:playMotion(motion)
    end
end

function Ball_Interpolation:playMotion(motion)
    self.motion = motion
    self.isPlayingMovie = true
    self.avatorSp:PlayMotion(motion,true)

    self.ballObject.ballSp:stopActionByTag(TAG)
    local act = cc.Sequence:create(
        cc.DelayTime:create(2),
        cc.CallFunc:create(function() 
            self.isPlayingMovie = false
            -- self.avatorSp:StopAllMotions() 
            self.motion = 'breathe'
            self.avatorSp:PlayMotion(self.motion,true)
        end)
    )
    act:setTag(TAG)
    self.ballObject.ballSp:runAction(act)
end

function Ball_Interpolation:reset()
    self.motion = 'breathe'
    self.isPlayingMovie = false
    -- self.avatorSp:StopAllMotions()
    self.ballObject.ballSp:stopActionByTag(TAG)
    self.avatorSp:PlayMotion(self.motion,true)
end