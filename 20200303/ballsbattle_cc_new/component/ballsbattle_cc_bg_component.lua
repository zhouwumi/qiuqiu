BBCCBgComponent = CreateClass()

local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')
local winSize = g_director:getWinSize()
local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
local ObjectPool = import('logic.dialog.ballsbattle_cc_new.bb_object_pool').ObjectPool

function BBCCBgComponent:__init__(mainPanel)
    self._mainPanel = mainPanel
    self.line_container = mainPanel.line_container
    self.line_container:removeAllChildren()
    self.winW, self.winH = winSize.width, winSize.height

    self.lineW, self.lineH = self._mainPanel.bg_container.left:GetContentSize()
    local scaleWidth, scaleHight = winSize.width / constant_ballsbattle.DESIGN_SIZE[1], winSize.height / constant_ballsbattle.DESIGN_SIZE[2]
    self.begainScale = math.min(scaleWidth,scaleHight)
    self.curScale = self.begainScale
    self.targetScale = 1

    self.sliderEffectPool = ObjectPool:New(constant_ballsbattle_cc.BBObjectTypes.SliderEffect, self._mainPanel)
    self.sliderEffectPool:CreateInitObjectCount(4, constant_ballsbattle_cc.BBObjectTypes.SliderEffect, self.sliderEffectPool)
    self:_updateVisibleRect()
    self:init3DLine()
end

function BBCCBgComponent:Update()
    -- print("BBCCBgComponent Update  ", self._mainPanel.frame)
	local targetScale, x, y = self:getTargetScale()
	if x ~= nil and self.targetScale ~= targetScale then
	    self.targetScale = targetScale
	    if targetScale > self.curScale then
	        self.delatScale = 1.0025
	    else
	        self.delatScale = 0.9975
	    end
	end

	if self.delatScale then
	    local newScale = self.curScale * self.delatScale
	    if (self.targetScale >= self.curScale and newScale >= self.targetScale) or
	       (self.targetScale <= self.curScale and newScale <= self.targetScale) then
	        self.delatScale = nil
	        newScale = self.targetScale
	    end
	    local playerObject = self._mainPanel.playerManager:GetMyPlayerObject()
	    if playerObject and playerObject:IsNewBirth() then
	    	self.curScale = self.targetScale
	    else
	    	self.curScale = newScale
	    end
	    
	    self._mainPanel.bg_container:setScale(self.curScale)
        self:_fixLineBlink()
	end

	if x ~= nil then
	    local point = cc.p(x / constant_ballsbattle.BG_SIZE[1],y / constant_ballsbattle.BG_SIZE[2])
	    self._mainPanel.bg_container:setAnchorPoint(point)
	    self._mainPanel.bg_container:setPosition(winSize.width/2, winSize.height/2)
	    self:_updateVisibleRect()
	    self.line_container:setAnchorPoint(point)
	    self:update3DLine(x, y, self.curScale)
	end
    self:_checkShowBoardEffect()
end

function BBCCBgComponent:_fixLineBlink()
    local delta = 11 / self.curScale - self.lineH
    if math.abs(delta) > 6 then
        self.lineH = 11 + delta / 6
        local size = self._mainPanel.bg_container.left:CalcSize(self.lineW, self.lineH)
        self._mainPanel.bg_container.left:setContentSize(size)
        self._mainPanel.bg_container.right:setContentSize(size)
        self._mainPanel.bg_container.top:setContentSize(size)
        self._mainPanel.bg_container.bottom:setContentSize(size)
    end
end

function BBCCBgComponent:_checkShowBoardEffect()
    local myPlayerObject = self._mainPanel.playerManager:GetMyPlayerObject()
    if not myPlayerObject then
        return
    end
    local isShow, moviesTable = myPlayerObject:CheckBoardHit()
    self._mainPanel.bg_container.left:setVisible(isShow)
    self._mainPanel.bg_container.right:setVisible(isShow)
    self._mainPanel.bg_container.bottom:setVisible(isShow)
    self._mainPanel.bg_container.top:setVisible(isShow)
    --添加撞击动画
    for i,moviePos in pairs(moviesTable) do
        for type, pos in pairs(moviePos) do
            if pos ~= -1 then
                self:addImpactMovie(type, pos)
            end
        end
    end
end

function BBCCBgComponent:addImpactMovie(type, pos)
    local obj = self.sliderEffectPool:GetOrCreateObect(constant_ballsbattle_cc.BBObjectTypes.SliderEffect, self.sliderEffectPool)
    local displayObject = obj.displayObject
    if type == 1 then
        displayObject:setRotation(-90)
        displayObject:setPosition(0, pos)
    elseif type == 2 then
        displayObject:setRotation(180)
        displayObject:setPosition(pos, 0)
    elseif type == 3 then
        displayObject:setRotation(0)
        displayObject:setPosition(pos, constant_ballsbattle.BG_SIZE[1])
    else
        displayObject:setRotation(90)
        displayObject:setPosition(constant_ballsbattle.BG_SIZE[2], pos)
    end
    obj:Show()
end

function BBCCBgComponent:getTargetScale()
	local playerObject = self._mainPanel.playerManager:GetMyPlayerObject()
	if playerObject then
		local startx, starty, endx, endy = playerObject:GetRectCenter()
		--人物死亡，无球状态
		if endx == 0 then
		    return  1
		end
		local width = (endx - startx)
		local hight = (endy - starty)
		local length = math.sqrt(width * width + hight * hight)/2
		-- print(length, self.begainScale, 1/(0.003132 * length + 0.90604) * self.begainScale)
		return 1/(0.003132 * length + 0.90604) * self.begainScale, (startx + endx)/2, (starty + endy)/2
	end
	return 1
end

function BBCCBgComponent:_updateVisibleRect()
	local minLocalPoint = self._mainPanel.bg_container:convertToNodeSpace(ccp(0, 0))
	local maxLocalPoint = self._mainPanel.bg_container:convertToNodeSpace(ccp(self.winW, self.winH))
	local bgW, bgH = self._mainPanel.bg_container:GetContentSize()
	minLocalPoint.x = math.max(minLocalPoint.x, 0)
	minLocalPoint.y = math.max(minLocalPoint.y, 0)
	maxLocalPoint.x = math.min(maxLocalPoint.x, bgW)
	maxLocalPoint.y = math.min(maxLocalPoint.y, bgH)

	self.minLocalPoint = minLocalPoint
	self.maxLocalPoint = maxLocalPoint
end

function BBCCBgComponent:init3DLine()
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
function BBCCBgComponent:update3DLine(x, y, scale)
    self.camera:setPosition3D(ccp3(x, y, self.initZeye))
end

function BBCCBgComponent:GetVisibleRect()
	return self.minLocalPoint, self.maxLocalPoint
end

function BBCCBgComponent:GetCanSyncRect()
    local visibleMinPoint, visibleMaxPoint = self:GetVisibleRect()
    local deltaX = visibleMaxPoint.x - visibleMinPoint.x
    local deltaY = visibleMaxPoint.y - visibleMinPoint.y
    return visibleMinPoint.x - deltaX / 2, visibleMinPoint.y - deltaY / 2, visibleMaxPoint.x + deltaX / 2, visibleMaxPoint.y + deltaY / 2
end

function BBCCBgComponent:GetClientCanSyncRect()
    -- return self:GetCanSyncRect()
    local visibleMinPoint, visibleMaxPoint = self:GetVisibleRect()
    local deltaX = visibleMaxPoint.x - visibleMinPoint.x
    local deltaY = visibleMaxPoint.y - visibleMinPoint.y
    return visibleMinPoint.x - deltaX / 2 - 200, visibleMinPoint.y - deltaY / 2 - 200, visibleMaxPoint.x + deltaX / 2 + 200, visibleMaxPoint.y + deltaY / 2 + 200
end
