local ballsUtils = import('logic_data.game_ballsbattle.ballsbattle_utils')
local BallsBattleService = import('logic.dialog.ballsbattle.manager.ballsbattle_service')
local winSize = g_director:getWinSize()
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils

RemoteScene = CreateClass()
local max_radius = 63

function RemoteScene:__init__(mainPanel, node)
    self._mainPanel = mainPanel
    self.startX, self.startY = node.remoteSceneSp:getPosition()
    self.node = node
    self._remoteSceneHight = self.node.remoteSceneSp:GetContentSize()/2
    self._scaleRemoteSceneHight = self._remoteSceneHight * self.node.remoteSceneSp:getScaleX()
    self:_initTouch()
end

function RemoteScene:_initTouch()
    self.node.remoteLayer.OnBegin = function(pos)
        return self:_onTouchBegin(pos)
    end
    self.node.remoteLayer.OnDrag = function(pos)
        self:_onTouchMove(pos)
    end

    self.node.remoteLayer.OnEnd = function(pos)
        self:_onTouchEnd(pos)
    end
end


function RemoteScene:_onTouchBegin(pos)
    if self.isBegain then
        return false
    end
    self.isBegain = true
    self.startPos = cc.p(pos.x,pos.y)
    self.startPos.x = math.max(self._scaleRemoteSceneHight,pos.x)
    self.startPos.y = math.max(self._scaleRemoteSceneHight,pos.y)
    self.startPos.x = math.min(winSize.width - self._scaleRemoteSceneHight, self.startPos.x)
    self.startPos.y = math.min(winSize.height - self._scaleRemoteSceneHight, self.startPos.y)
    local inNodePos = self.node:convertToNodeSpace(self.startPos)
    self.node.remoteSceneSp:setPosition(cc.p(inNodePos.x - self._scaleRemoteSceneHight,inNodePos.y - self._scaleRemoteSceneHight))
    local x = (pos.x - self.startPos.x)
    local y = (pos.y - self.startPos.y)
    self.node.remoteSceneSp.directionSp:setPosition(x + self._remoteSceneHight, y + self._remoteSceneHight)
    return true
end

function RemoteScene:_onTouchMove(pos)
    self:_caculateDirection(pos)
end

function RemoteScene:_onTouchEnd(pos)
    self:_caculateDirection(pos)

    self.node.remoteSceneSp:setPosition(self.startX, self.startY)
    self.node.remoteSceneSp.directionSp:SetPosition('50%', '50%')

    self.isBegain = false
end

function RemoteScene:Reset(pos)
    self.isBegain = false
end
        
function RemoteScene:_caculateDirection(pos)
    local x = (pos.x - self.startPos.x)
    local y = (pos.y - self.startPos.y)
    local length = math.sqrt(x * x + y * y)
    local percent = math.ceil(length/max_radius * 100)
    if percent < 20 then
        percent = 0
    end
    if length > 0 then
        if length > max_radius then
            x = x / length * max_radius
            y = y / length * max_radius
            percent = 100
        end
    else
        x = 0
        y = 0
        percent = 0
    end
    self.node.remoteSceneSp.directionSp:setPosition(x + self._remoteSceneHight, y + self._remoteSceneHight)

    local angle
    if x ~= 0 or y ~= 0 then
        angle = ballsUtils.get_radian_by_vector(x, y)
        angle = math.floor(math.floor((math.deg(angle))) * 10)
        if angle == 3600 then
            angle = 0
        end
    else
        angle = 0
    end
    -- print("current dirction  ", angle, percent)
    if self._mainPanel.playerManager:GetMyPlayerObject() then
        self.currentDir = {angle, percent}
        self._mainPanel:GetMoveManager():SetMove(angle, percent)
        -- print(self._mainPanel.frame, angle, percent) 
    end
    -- BallsBattleService.send_dirction({angle, percent})
end