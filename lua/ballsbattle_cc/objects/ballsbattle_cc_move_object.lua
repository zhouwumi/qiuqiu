local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local Object = import('ballsbattle_cc_object').Object
-- local BallsBattleService = import('logic.dialog.ballsbattle.manager.ballsbattle_service')
local ballsUtils = import('logic_data.game_ballsbattle.ballsbattle_utils')
local frame = 20
Move_Object,Super = CreateClass(Object)

function Move_Object:_initInfo(info)
    self.canReset = false
    Super._initInfo(self,info)
    self:_initSpeed(info)
end

function Move_Object:_initSpeed(info)
    self.speedX = info.vector[1]
    self.speedY = info.vector[2]

    self.radian = ballsUtils.get_radian_by_vector(self.speedX, self.speedY)
    self.distanceX,self.distanceY = self.speedX, self.speedY
    self.targetRotation = 360 - math.deg(self.radian)
    self.curRotation = self.targetRotation
    self.changeSpeedCD = info.init
    self.beEat = false
    if info.init > 0 then
        self:caculateInitSpeed()
    end
    self:caculateSpeed()
end

function Move_Object:updateInfo()
    if self.changeSpeedCD > 0 then
        self.changeSpeedCD = self.changeSpeedCD - 1
    end
end

function Move_Object:update()
    if self:move() then
        self.lastPositionX = self:_checkBorder(self.lastPositionX)
        self.lastPositionY = self:_checkBorder(self.lastPositionY)

        -- BallsBattleService.get_grid_manager():updateOjectPosition(self)
    end
    -- if self.uid == g_user_info.get_user_info().uid then
    --     print('Move_Object',self.lastPositionX,self.lastPositionY,self.idx)
    -- end
end

function Move_Object:move()
    if self.changeSpeedCD > 0 then
        self.speedX = self.speedX + self.accelerationX
        self.speedY = self.speedY + self.accelerationY
        self.lastPositionX = self.lastPositionX + self.speedX
        self.lastPositionY = self.lastPositionY + self.speedY
        return true
    end
end

function Move_Object:_checkBorder(position)
    if position - self.radius < 0 then
        position = self.radius
    elseif position + self.radius > constant_ballsbattle.BG_SIZE[1] then
        position = constant_ballsbattle.BG_SIZE[1] - self.radius
    end
    position = ballsUtils.math_get_int(position)
    return position
end

function Move_Object:caculateSpeed()
    self.maxSpeed = math.floor(8*math.sqrt(20/(self.mass+10))*10)/10
    --两种移动方式，球体移动，比较混乱
    if self.distanceX ~= 0 or self.distanceY ~= 0 then
        self.distanceX,self.distanceY = ballsUtils.get_vector_by_rad(self.radian,self.maxSpeed)
    end
end

function Move_Object:caculateInitSpeed()
    local delta = math.abs((0 - 30)/ frame)
    local acceleration = self.radius/(frame * (frame - 1)/2) + delta
    local speed = acceleration * self.changeSpeedCD
    self.speedX,self.speedY = ballsUtils.get_vector_by_rad(self.radian, speed)
    self.accelerationX,self.accelerationY = ballsUtils.get_vector_by_rad(self.radian, acceleration * -1)
end

function Move_Object:makeCachePhoto()
    -- print('makeCachePhoto',self.idx)
    self.cachePositionX = self.lastPositionX
    self.cachePositionY = self.lastPositionY
    self.cacheSpeedX = self.speedX 
    self.cacheSpeedY = self.speedY 

    self.cacheChangeSpeedCD = self.changeSpeedCD
    self.cacheProtectCD = self.protectCD
    self.cacheFuseCD = self.fuseCD
    self.cacheMass = self.mass
    self.canReset = true
end

function Move_Object:resetToCachePhoto()
    -- print('resetToCachePhoto',self.canReset,self.idx)
    if self.canReset then
        self.lastPositionX = self.cachePositionX
        self.lastPositionY = self.cachePositionY
        self.speedX = self.cacheSpeedX 
        self.speedY = self.cacheSpeedY 

        self.changeSpeedCD = self.cacheChangeSpeedCD
        self.protectCD = self.cacheProtectCD
        self.fuseCD = self.cacheFuseCD
        self.mass = self.cacheMass
        self.canReset = false
    end
end