local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local BallsBattleService = import('logic.dialog.ballsbattle.manager.ballsbattle_service')
local CCAnimateSprite = tolua_get_class('CCAnimateSprite')
local ballsUtils = import('logic_data.game_ballsbattle.ballsbattle_utils')

Bg = CreateClass()
local winSize = g_director:getWinSize()

function Bg:__init__(container, bg, line_container)
    self.container = container
    self.line_container = line_container
    line_container:removeAllChildren()
    self.container:setVisible(false)
    self.impactMovieList = {}
    -- self.bg = bg
    self:_initView()
end

function Bg:_initView()
    self.myPlayer = BallsBattleService.get_my_player()

    local scaleWidth,scaleHight = winSize.width/constant_ballsbattle.DESIGN_SIZE[1], winSize.height/constant_ballsbattle.DESIGN_SIZE[2]
    self.begainScale = math.min(scaleWidth,scaleHight)
    self.curScale = self.begainScale
    self.targetScale,x,y = self:getTargetScale()
    self:init3DLine()
    
    self.container:setPosition(winSize.width/2,winSize.height/2)

    self.container:setScale(self.targetScale)
    self.curScale = self.targetScale
    -- self.bg:setScale(self.begainScale)

    -- for i=30,4500,500 do
    --     for j=30,4500,500 do
    --         local index = math.random(1,3)
    --         local sp = cc.Sprite:create('gui/badam_ui/ballsbattle/bg/bg_'..index..'.png')
    --         self.bg:addChild(sp,index * 100)
    --         sp:setOpacity(122)
    --         sp:setPosition(cc.p(i + math.random(1,300),j + math.random(1,300)))
            
    --     end
    -- end
    self.width,self.height = self.container.left:GetContentSize()
    if x ~= nil then
        self.container:setAnchorPoint(cc.p(x/constant_ballsbattle.BG_SIZE[1],y/constant_ballsbattle.BG_SIZE[2]))
        BallsBattleService.get_grid_manager():updateField(x, y, self.curScale)
    end
    -- local x,y = ballsUtils.get_positon_by_locaiton(center)
    -- self.container:setPosition(winSize.width/2 - x * self.curScale,winSize.height/2 - y * self.curScale)
    self.container:setVisible(true)

    -- self.container:setPosition(-5000,-5000)
    -- local action = cc.MoveTo:create(20, cc.p(100,100))
    -- self.container:runAction(action)
end 

function Bg:addImpactMovie(type,pos)
    local obj 
    for i,poolObj in pairs(self.impactMovieList) do
        if not poolObj.isUse then
            obj = poolObj 
            obj:setVisible(true)
        end 
    end

    if obj == nil then
        obj = CCAnimateSprite:Create()
        obj:setScale(0.5)
        self.container:addChild(obj)
        table.insert(self.impactMovieList, obj)
    end

    obj:SetFrameDelay(0.03)
    obj:SetAniSptDisplayFrameByPath('gui/Animation/ballsbattle/side_effect/qiuqiu_pengzhuang.plist', nil, true)
    
    if type == 1 then
        obj:setRotation(-90)
        obj:setPosition(3,pos)
    elseif type == 2 then
        obj:setRotation(180)
        obj:setPosition(pos,3)
    elseif type == 3 then
        obj:setRotation(0)
        obj:setPosition(pos,constant_ballsbattle.BG_SIZE[1] - 3)
    else
        obj:setRotation(90)
        obj:setPosition(constant_ballsbattle.BG_SIZE[2] - 3,pos)
    end
    obj:setAnchorPoint(cc.p(0.5,0))
    obj:Play(1,0.03)
    obj:DelayCall(obj:GetPlayDuration(), function() 
        obj:setVisible(false) 
        obj.isUse = false
    end)
end

function Bg:getTargetScale()
    local startx, starty, endx, endy = self.myPlayer:getDisplayBallRect()
    --人物死亡，无球状态
    if endx == 0 then
        return  1
    end
    local width = (endx - startx)
    local hight = (endy - starty)
    local length = math.sqrt(width * width + hight * hight)/2
    return 1/(0.003132 * length + 0.90604) * self.begainScale, (startx + endx)/2, (starty + endy)/2
end

function Bg:update()
    --大小以及中心点
    local targetScale,x,y = self:getTargetScale()
    if x ~= nil and self.targetScale ~= targetScale then
        self.targetScale = targetScale
        if targetScale > self.curScale then
            self.delatScale = 1.005
        else
            self.delatScale = 0.995
        end
    end
    if self.delatScale then
        local newScale = self.curScale * self.delatScale
        if (self.targetScale >= self.curScale and newScale >= self.targetScale) or
           (self.targetScale <= self.curScale and newScale <= self.targetScale) then
            self.delatScale = nil
            newScale = self.targetScale
        end
        self.curScale = newScale
        BallsBattleService.cur_scale = self.curScale
        -- self.curScale = 0.07
        self.container:setScale(self.curScale)
        --线闪烁问题
        local delta = 11 / self.curScale - self.height
        if math.abs(delta) > 6 then
            self.height = 11 + delta/6
            local size = self.container.left:CalcSize(self.width, self.height)
            self.container.left:setContentSize(size)
            self.container.right:setContentSize(size)
            self.container.top:setContentSize(size)
            self.container.bottom:setContentSize(size)
        end
        -- print('height', height, size)

        -- self.bg:setScale(self.curScale)
    end
    -- self.bg:setPosition(x/2, y/2)
    if x ~= nil then
        local point = cc.p(x/constant_ballsbattle.BG_SIZE[1],y/constant_ballsbattle.BG_SIZE[2])
        self.container:setAnchorPoint(point)
        self.line_container:setAnchorPoint(point)
        BallsBattleService.get_grid_manager():updateField(x, y, self.curScale)
        self:update3DLine(x, y, self.curScale)
    end
end

-- 一些显示效果
function Bg:updateDisplay()
    local radio = self.curScale/self.begainScale
    local isHide,moviesTable = self.myPlayer:updateDisplay(radio)
    BallsBattleService.get_grid_manager():checkScale(radio,self.myPlayer)

    self.container.left:setVisible(isHide)
    self.container.right:setVisible(isHide)
    self.container.top:setVisible(isHide)
    self.container.bottom:setVisible(isHide)
    --添加撞击动画
    for i,moviePos in pairs(moviesTable) do
        for type,v in pairs(moviePos) do
            for j,pos in pairs(v) do
                self:addImpactMovie(type,pos)
            end
        end
    end
end

function Bg:init3DLine()
    if not constant_ballsbattle.USE_LIVE_2D or cc.DrawNode3D == nil then
        return
    end
    local _camera = cc.Camera:createPerspective(60, winSize.width / winSize.height, 10, 1000)
    _camera:setCameraFlag(cc.CameraFlag.USER1)
    self.line_container:addChild(_camera)

    local zeye = winSize.width / (4 * math.tan(30 / 180 * math.pi))
    self.initZeye = zeye
    local eye = ccp3(winSize.width / 2, winSize.height / 2.0, zeye)
    local center = ccp3(winSize.width / 2, winSize.height / 2, 0.0)
    local up = ccp3(0.0, 1.0, 0.0)
    _camera:setPosition3D(eye)
    _camera:lookAt(center, up)
    local opacity = 0.75
    local default_color = cc.c4f(81 / 255 * opacity, 111 / 255 * opacity, 161 / 255 * opacity, opacity)
    local line = cc.DrawNode3D:create()
    local function drawLine(pos1, pos2, color)
        if color == nil then
            color = default_color
        end
        line:drawLine(pos1, pos2, color)
    end

    local line_container_width, line_container_height = self.line_container:GetContentSize()
    local lineLen = self.line_container:GetContentSize()  + winSize.width * 2
    local lineIndent = lineLen / 50
    local lineCount = math.ceil(lineLen / lineIndent) + 1

    local originX = (line_container_width - lineLen) / 2
    local originY = (line_container_height - lineLen) / 2

    local zSpace = 60
    local maxZIndex = 4
    local minZ = -1 * (maxZIndex - 1) * zSpace - 1
    local maxZ = 200
    --各个平台表现不一样
    local curPlatform = g_application:getTargetPlatform()
    if (curPlatform == cc.PLATFORM_OS_IPHONE or curPlatform == cc.PLATFORM_OS_IPAD) then
        minZ = minZ + 200
        maxZ = maxZ + 200
    elseif (curPlatform == cc.PLATFORM_OS_ANDROID) then
        minZ = minZ + 100
        maxZ = maxZ + 100
    end

    for z = 1, maxZIndex do
        local zz = minZ + (z - 1) * zSpace
        
        local curX = originX
        local curY = originY

        for i = 1, lineCount do
            drawLine(ccp3(curX, originY, zz), ccp3(curX, originY + lineLen, zz))
            drawLine(ccp3(originX, curY, zz), ccp3(originX + lineLen, curY, zz))

            curX = curX + lineIndent
            curY = curY + lineIndent
        end
    end
    
    local curX = originX
    for x = 1, lineCount + 1 do
        local curY = originY
        for y = 1, lineCount + 1 do
            drawLine(ccp3(curX, curY, minZ), ccp3(curX, curY, maxZ))
            curY = curY + lineIndent
        end
        curX = curX + lineIndent
    end

    line:SetPosition(0, 0)
    self.line_container:addChild(line)
    self.line_container:setCameraMask(cc.CameraFlag.USER1)

    self.camera = _camera
end

function Bg:update3DLine(x, y, scale)
    if cc.DrawNode3D == nil or self.camera == nil then
        return
    end
    -- local lastPos3D = self.camera:getPosition3D()
    -- local eyeX = winSize.width/2 - x * self.curScale
    -- local eyeY = winSize.height/2 - y * self.curScale
    -- local eyeZ = math.min(self.initZeye + (scale - self.begainScale) * 50, 1000)
    self.camera:setPosition3D(ccp3(x, y, self.initZeye))
end

function Bg:setScale(scale)
    self.container:setScale(scale * self.begainScale)
    message(scale)
end

function Bg:onRevive()
    self.targetScale,x,y = self:getTargetScale()
    self.container:setPosition(winSize.width/2,winSize.height/2)

    self.container:setScale(self.targetScale)
    self.curScale = self.targetScale
    if x ~= nil then
        self.container:setAnchorPoint(cc.p(x/constant_ballsbattle.BG_SIZE[1],y/constant_ballsbattle.BG_SIZE[2]))
        BallsBattleService.get_grid_manager():updateField(x, y, self.curScale,true)
    end
end
