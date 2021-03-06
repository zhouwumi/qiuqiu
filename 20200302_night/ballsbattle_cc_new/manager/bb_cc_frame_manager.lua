BBCCFrameManager = CreateClass()

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')


function BBCCFrameManager:__init__(mainPanel)
	self._mainPanel = mainPanel
	self.foodManager = self._mainPanel.foodManager
	self.spikySporeManager = self._mainPanel.spikySporeManager
	self.playerManager = self._mainPanel.playerManager
	
    self.infoManager = self._mainPanel.infoManager

	self.bgComponent = self._mainPanel.bgComponent
	self.clientSendToServerComponent = self._mainPanel.clientSendToServerComponent
	self.serverComponent = self._mainPanel.serverComponent

	self.myUserId = g_user_info.get_user_info().uid
    self:ReEnter()
end

function BBCCFrameManager:ReEnter()
    self.foceUpdate = false
    self.lastCacheRequestTime = {}
    self.playerUpdateFrames = {}
    self.lastPlayerCount = nil
    self.gameManager = self._mainPanel.gameManager
end

function BBCCFrameManager:_getCanUpdatePlayerIds()
    if constant_ballsbattle_cc.IsScopeOpen then
        local minX, minY, maxX, maxY = self.bgComponent:GetClientCanSyncRect()
        local playerIds = self.gameManager:GetAllPlayerIdxInRect(minX, maxX, minY, maxY)
        if self.lastPlayerCount and #playerIds ~= self.lastPlayerCount then
            -- message("人数改变了  "..#playerIds)
            print("人数改变了   ", #playerIds)
        end
        self.lastPlayerCount = #playerIds
        self._mainPanel.labelPlayerCount:SetString('同屏人数  '..#playerIds)
        local show_str = ''
        for _, playerId in ipairs(playerIds) do
            show_str = show_str..playerId.."\n"
        end
        self._mainPanel.labelUpdatePlayerIds:SetString(show_str)
        return playerIds
    else
        return table.keys(self.playerManager.allPlayers)
    end
end

function BBCCFrameManager:OnUpdate()
    if not self._times then self._times = 0 end
    self._times = self._times + 1
    local moveManager = self._mainPanel:GetMoveManager()

	self.foodManager:OnUpdateFood(self.foceUpdate)
    self.spikySporeManager:OnUpdateVisible(self.foceUpdate)
    self.foceUpdate = false

    self.isKeyFrame = self._mainPanel.frame % constant_ballsbattle_cc.FrameRate == 0
    
    self.spikySporeManager:UpdateSpore()
    
    local playerIds = self:_getCanUpdatePlayerIds()
    self.updatePlayerIds = playerIds

    if self.isKeyFrame then
        local angle, pressure, isShoot, isSplit, commandIdx, checkSum
        local myPlayerObject = self.playerManager:GetMyPlayerObject()
        local hasOperate = false
        if myPlayerObject and not myPlayerObject:IsDead() then
            if self.gameManager:CanAddPlayerCommand(self.myUserId) then
                hasOperate = true
                moveManager:IncreaseFrame()
                angle = moveManager:GetAngle()
                pressure = moveManager:GetPressure()
                isShoot = moveManager:IsShoot()
                isSplit = moveManager:IsSplit()
                commandIdx = moveManager:GetNextIndex()
                print("客户端执行命令  ",commandIdx)
                -- print("客户端执行命令  ",angle, pressure, isSplit, isShoot, commandIdx)
                if isShoot then
                    -- print("客户端收到了吐bao子")
                end
                self.gameManager:AddPlayerCommand(self.myUserId, angle, pressure, isSplit, isShoot, commandIdx, 0)
            end
        end
        
        local time1 = utils_get_tick()
        self.gameManager:FrameUpdate()
        
        
        self._moveTime = 0
        self._crcTime = 0
        self._hitTime = 0
        local time2 = utils_get_tick()
        for _, playerId in ipairs(playerIds) do
            self.gameManager:UpdatePlayer(playerId)
            self.playerUpdateFrames[playerId] = self._mainPanel.frame
            local player = self.playerManager:GetPlayer(playerId)
            player:AddNewFrame()
        end
        local time3 = utils_get_tick()
        local updateTime = time3 - time2
        local total_time = time3 - time1
        -- print("客户端执行时间  ", time2 - time1, updateTime, total_time, self._mainPanel.frame, #playerIds, self._moveTime / updateTime, self._crcTime / updateTime, self._hitTime / updateTime)
        print("客户端执行时间  ",self._mainPanel.frame, self._times)

       self:_requestPlayerInfos()
        

        moveManager:ResetFrame()
        -- self.foodManager:SimulateEatFood()
        -- self.spikySporeManager:SimulateEatSpores()
        
        if hasOperate then
            local checkSum = moveManager:GetCheckSum()
            print("客户端执行的checkSum  ", checkSum)
            self.clientSendToServerComponent:AddPlayerCommand(self.myUserId, angle, pressure, isSplit, isShoot, commandIdx, checkSum)
        end
    end
    -- local player = self.playerManager:GetPlayer(g_user_info.get_user_info().uid)
    -- player:Update()
    -- self.playerManager:Update(self.updatePlayerIds)    
end

function BBCCFrameManager:CacheTime(moveTime, crcTime)
    self._moveTime = self._moveTime + moveTime
    self._crcTime = self._crcTime + crcTime
end

function BBCCFrameManager:CacheHitTime(hitTime)
    self._hitTime = self._hitTime + hitTime
end

function BBCCFrameManager:_requestPlayerInfos()
    for _, playerId in ipairs(self.updatePlayerIds or {}) do
        local isNeedSyncState = self.gameManager:IsNeedSyncState(playerId)
        if playerId ~= self.myUserId then
            if isNeedSyncState then
                local canRequest = true
                local lastTime = self.lastCacheRequestTime[playerId]
                local lastAck = self.infoManager.lastAckCache[playerId]
                if not lastAck then
                    lastAck = -1
                end
                if lastTime then
                    if lastTime > lastAck then
                        canRequest = false
                    elseif self._mainPanel.frame - lastTime < 10 then
                        canRequest = false
                    end
                end
                print("计算不一致  ", playerId, self._mainPanel.frame)
                if canRequest then
                    -- message("请求数据")
                    print("请求数据 ", self._mainPanel.frame, playerId, lastTime, lastAck)
                    self.clientSendToServerComponent:RequestServerData(playerId)
                    -- self.serverComponent:RequestData(playerId)
                    self.lastCacheRequestTime[playerId] = self._mainPanel.frame
                end
            else
                -- print("计算一致  ", playerId, self._mainPanel.frame)
            end
        end
    end
end