local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
Live2DSprite = CreateClass()

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
local getPath = function(pathType, path, motion)
    if pathType == 1 then
        return 'gui/Animation/ballsbattle/'..path..'/model.json'
    else
        return 'gui/badam_ui/ballsbattle/live2d/'..path..'/'..motion..'.png'
    end
end

function Live2DSprite:__init__(path, motion, isAsyn)
    self.path = path
    self.isAsyn = isAsyn
    motion = motion or "default"
    if constant_ballsbattle.USE_LIVE_2D then
        self.sp = cc.Live2DSprite:Create(getPath(1, path), isAsyn)
        self.sp:PlayMotion(motion)
        self.sp:setAnchorPoint(cc.p(0.5,0.5))
        self.sp:SetContentSize(100,100)
        self.isLive2d = true
        self.canChangeMode = true
    else
        local texture = g_director:getTextureCache():addImage(getPath(2, path, motion))
        texture:generateMipmap()
        self.sp = cc.Sprite:createWithTexture(texture)
        self.sp:setStretchEnabled(true)
        self.sp:SetContentSize(200,200)
    end
end

function Live2DSprite:SetContentSize(width,height)
    if self.isLive2d then
        self.sp:SetContentSize(width,height)
    else
        self.sp:SetContentSize(width * 2,height * 2)
    end
end

--换皮
function Live2DSprite:SetModelPath(path, motion, isAsyn)
    if self.isLive2d then
        self.sp:SetModelPath(getPath(1, path),isAsyn)
        self.sp:PlayMotion(motion)
    else
        self.sp:SetPath('',getPath(1, path, motion))
    end
end

function Live2DSprite:PlayMotion(motion, isLoop, isLoopFadeIn, offset)
    if self.isLive2d then
        self.sp:PlayMotion(motion, isLoop, isLoopFadeIn, offset)
    else
        --默认球体就一个表情，暂时不做切换
        -- self.sp:SetPath('',getPath(1, path, motion))
    end
end

function Live2DSprite:ChangeMode(mode)
    if self.canChangeMode then
        if self.copySp == nil then
            local texture = g_director:getTextureCache():addImage(getPath(2, self.path, 'breathe'))
            texture:generateMipmap()
            self.copySp = cc.Sprite:createWithTexture(texture)
            self.copySp:setStretchEnabled(true)
            self.copySp:SetContentSize(200,200)
            self.sp:getParent():addChild(self.copySp)
            self.copySp:setPosition(self.sp:getPosition())
        end
        
        if mode == 1 then
            self.sp:setVisible(false)
            self.copySp:setVisible(true)
            self.isLive2d = false
        else
            self.sp:setVisible(true)
            self.copySp:setVisible(false)
            self.isLive2d = true
        end
    end
end

function Live2DSprite:retain()
    self.sp:retain()
    self.copySp = nil
end

function Live2DSprite:release()
    self.sp:release()
end

function Live2DSprite:StopAllMotions()
    self.sp:StopAllMotions()
end