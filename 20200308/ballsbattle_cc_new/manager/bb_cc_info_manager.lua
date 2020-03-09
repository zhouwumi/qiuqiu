BBCCInfoManager = CreateClass()

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils

function BBCCInfoManager:__init__(mainPanel)
	self._mainPanel = mainPanel

    self.gridManager = self._mainPanel.gridManager
	self.foodManager = self._mainPanel.foodManager
	self.spikySporeManager = self._mainPanel.spikySporeManager
	self.playerManager = self._mainPanel.playerManager
	

	self.bgComponent = self._mainPanel.bgComponent
    self:ReEnter()
end

function BBCCInfoManager:ReEnter()
    self.lastAckCache = {}
    self.gameManager = self._mainPanel.gameManager
    show_loading_panel("wait_player", 10)
end

local function unpackSporeInfo(sporeInfo)
    return sporeInfo.idx, sporeInfo.fromId, sporeInfo.uid, sporeInfo.speedX, sporeInfo.speedY, sporeInfo.x, sporeInfo.y, sporeInfo.initSpeed, sporeInfo.initDeltaSpeed, sporeInfo.initStopFrame, sporeInfo.cd
end

function BBCCInfoManager:SyncFirstData(data)
    local sporeInfos = data.sporeInfos or {}
    for _, sporeInfo in pairs(sporeInfos) do
        local idx, fromId, uid, speedX, speedY, x, y, initSpeed, initDeltaSpeed, initStopFrame, cd = unpackSporeInfo(sporeInfo)
        self.gameManager:AddNewSporeFromServer(idx, fromId, uid, x, y, speedX, speedY, initSpeed, initDeltaSpeed, initStopFrame, cd)
        self.spikySporeManager:CreateSporeFromServer(idx)
    end

    if data.foodInfos then
        for index = 1, #data.foodInfos do
            local idx = data.foodInfos[index]
            self.foodManager:CreateFoodWithServerInfo(idx)
            -- self.gameManager:AddNewFoodFromServer(idx)
        end
    end
     -- print("同步初始信息", data.foodInfos)

    if data.spikyInfos then
        for _, spikyInfo in ipairs(data.spikyInfos or {}) do
            local idx, x, y, mass, radius = unpack(spikyInfo)
            local pos = BBCCUtils.ClientPackLocation(x, y)
            -- self.gameManager:AddNewSpikyFromServer(idx, pos, mass)

            local newSpikyInfo = {}
            newSpikyInfo.x = x
            newSpikyInfo.y = y
            newSpikyInfo.mass = mass
            newSpikyInfo.idx = idx
            self.spikySporeManager:CreateSpikyFromServer(newSpikyInfo)
        end
    end
    if data.playerInfos then
        for _, playerInfo in pairs(data.playerInfos) do
            self:_syncInitCreatePlayer(playerInfo)
        end
        -- self.playerManager:GeneratePlayerAndNodes(data.playerInfos)
    end
    close_loading_panel("wait_player")
    self.bgComponent:Update()
    local removeList, addList = self.gridManager:OnUpdateGridVisible(true)
    if removeList or addList then
        self.foodManager:OnUpdateFood(removeList, addList)
        self.spikySporeManager:OnUpdateVisible(removeList, addList)
    end
    -- self.foodManager:OnUpdateFood(true)
    -- self.spikySporeManager:OnUpdateVisible(true)
    self._mainPanel:RunLoop()
end

function BBCCInfoManager:_syncInitCreatePlayer(data)
    self.playerManager:GeneratePlayer(data)
    self.playerManager:GeneratePlayerNodes(data)
end

function BBCCInfoManager:SyncNewPlayers(data)
    local isMeBirth = false
    local newPlayers = data.newPlayers
    -- print("新的玩家  ", newPlayers)
    -- for _, data in pairs(newPlayers or {}) do
    --     self:_syncServerInfo(data)
    -- end
    for _, data in pairs(newPlayers or {}) do
        if data.uid == g_user_info.get_user_info().uid then
            isMeBirth = true
            break
        end
    end

    for _, data in pairs(newPlayers or {}) do
       self:_syncInitCreatePlayer(data)
    end

    -- self.playerManager:GeneratePlayerAndNodes(newPlayers)

    if isMeBirth then
        self._is_frame_new_join = true
        delay_call(0, function()
            self._is_frame_new_join = false
            self.playerManager:SyncAllPlayerNodeToRender()
        end)
        self:_onRevive()
    end
end

function BBCCInfoManager:_onRevive()
    print("我重新进来了")
    self.bgComponent:Update()
    local removeList, addList = self.gridManager:OnUpdateGridVisible(true)
    if removeList or addList then
        self.foodManager:OnUpdateFood(removeList, addList)
        self.spikySporeManager:OnUpdateVisible(removeList, addList)
    end
    g_panel_mgr.close('ballsbattle_cc_new.panel.dlg_ballsbattle_cc_revive_panel')
end

function BBCCInfoManager:NewFoods(foodInfos)
    for index = 1, #foodInfos do
        local idx = foodInfos[index]
        -- self.gameManager:AddNewFoodFromServer(idx)
        self.foodManager:CreateFoodWithServerInfo(idx)
        -- message("新食物生成")
        -- print("新食物生成")
    end
end

function BBCCInfoManager:NewSpores(sporeInfos)
    for sporeId, info in pairs(sporeInfos) do
        local fromId, uid, speedX, speedY, x, y, initSpeed, initDeltaSpeed, initStopFrame, cd = unpack(info)
        if uid == g_user_info.get_user_info().uid then
            local fromNode = self.playerManager:GetPlayerNodeObject(fromId)
            if fromNode then
                local x, y = fromNode:GetDisplayPosition()
                self.gameManager:SetRealClientRenderXY(fromId, x, y)
            end
        end
        self.gameManager:SyncShootFromServer(sporeId, fromId, uid, speedX, speedY, x, y)
        x = self.gameManager:GetSporeRenderX(sporeId)
        y = self.gameManager:GetSporeRenderY(sporeId)
        self.spikySporeManager:CreateSporeFromServer(sporeId)
        -- message("有新的孢子了")
        -- print("有新的孢子了", sporeId)
    end
end

function BBCCInfoManager:NewSpikys(spikyInfos)
    for spikyId, info in pairs(spikyInfos) do
        local idx = info.idx
        local x = info.x
        local y = info.y
        local mass = info.mass
        local pos = BBCCUtils.ClientPackLocation(x, y)
        -- self.gameManager:AddNewSpikyFromServer(idx, pos, mass)
        self.spikySporeManager:CreateSpikyFromServer(info)
        -- print("有新的刺球了")
    end
end

function BBCCInfoManager:SyncFoodEatInfo(foodEatInfos)
    if not foodEatInfos or #foodEatInfos <= 0 then
        return
    end
    -- print("移除食物 ",foodEatInfos)
    local massChange = {}
    for i = 1, #foodEatInfos, 2 do
        local foodIdx = foodEatInfos[i]
        local nodeIdx = foodEatInfos[i + 1]
        self.foodManager:RemoveFoodFromServer(foodIdx, nodeIdx)
        -- self.gameManager:RemoveFoodFromServer(foodIdx)
        local mass = massChange[nodeIdx] or 0
        mass = mass + 1
        massChange[nodeIdx] = mass
    end
    for nodeIdx, mass in pairs(massChange) do
        self.gameManager:OnNodeEatFood(nodeIdx, mass)
    end
end

function BBCCInfoManager:SyncSporeEatInfo(sporeEatInfos)
    if not sporeEatInfos or #sporeEatInfos <= 0 then
        return
    end
    -- print("移除孢子 ",sporeEatInfos)
    for i = 1, #sporeEatInfos, 2 do
        local sporeId = sporeEatInfos[i]
        local nodeIdx = sporeEatInfos[i + 1]
        self.spikySporeManager:RemoveSpore(sporeId)
        self.gameManager:RemoveSporeFromServer(sporeId)
    end
end

function BBCCInfoManager:SyncSpikyEatInfos(spikyEatInfos)
    if not spikyEatInfos or #spikyEatInfos <= 0 then
        return
    end
    -- print("移除刺球 ", spikyEatInfos)
    -- message("刺球被吃了")
    for i = 1, #spikyEatInfos do
        local spikyIdx = spikyEatInfos[i]
        self.spikySporeManager:RemoveSpikyFromServer(spikyIdx)
    end
end


function BBCCInfoManager:UpdateServerAck(serverAcks)
    if not serverAcks then
        return
    end
    local myAck = serverAcks[g_user_info.get_user_info().uid]
    if not myAck then
        return
    end
    self.gameManager:SyncServerAckCommand(g_user_info.get_user_info().uid, myAck)
    -- print("server ack ", myAck)
end

function BBCCInfoManager:SyncCommands(frameCommands)
    for uid, uidCommands in pairs(frameCommands) do
        if uid ~= g_user_info.get_user_info().uid then
            for _, info in ipairs(uidCommands) do
                local uid, angle, pressure, isSplit, isShoot, commandIdx, checkSum = unpack(info)
                self.gameManager:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, commandIdx, checkSum)
            end
        end
    end
end


function BBCCInfoManager:SyncPlayerInfos(newPlayerInfos)
    if not newPlayerInfos then
        return
    end
    local playerInfo = newPlayerInfos[g_user_info.get_user_info().uid]
    for playerId, playerInfo in pairs(newPlayerInfos) do
        playerInfo.playerId = playerId
        -- print("sync info from server:  ", self._mainPanel.frame, playerInfo)
        self:_syncServerInfo(playerInfo)
    end
    -- if not playerInfo then return end
    -- print("sync info from server:  ",playerInfo)
    
    -- self:SyncServerInfo(playerInfo)
end

function BBCCInfoManager:IsInVisibleRect(x, y)
    local minPoint, maxPoint = self._mainPanel.bgComponent:GetVisibleRect()
    if x < minPoint.x or x > maxPoint.x or y < minPoint.y or y > maxPoint.y then
        return false
    end
    return true
end

 --两种情况直接同步位置,1:目标位置不在视角内,2:差距在200个像素差以上,3:太多帧没更新了
 ----当复活的时候重新刷新所有的节点,要不然有些节点会快速的追帧
function BBCCInfoManager:_isNeedPullNodePosition(uid, idx, newX, newY, oldX, oldY)
    if self._is_frame_new_join then
        return true
    end
    if oldX and oldY and self:IsInVisibleRect(oldX, oldY) then
        return false, 1
    end
    local curX, curY = newX / 1000, newY / 1000
    --两种情况直接同步位置,1:目标位置不在视角内,2:差距在200个像素差以上
    if not self:IsInVisibleRect(curX, curY) then
        return true
    else
        local lastUpdateFrame = self._mainPanel.frameManager.playerUpdateFrames[uid]
        if lastUpdateFrame and self._mainPanel.frame - lastUpdateFrame > 10 then
            return true
        else
            local oldPlayerNode = self._mainPanel.playerManager:GetPlayerNodeObject(idx)
            local oldX, oldY = oldPlayerNode:GetDisplayPosition()
            local deltaX = oldX - curX
            local deltaY = oldY - curY
            if deltaX * deltaX + deltaY * deltaY > 40000 then
                -- __G__TRACKBACK__('dddddddddddddddd')
                return true
            end
        end
    end
    return false, 2
end

function BBCCInfoManager:_syncServerInfo(playerInfo)
    if not self.lastAckCache then
        self.lastAckCache = {}
    end

    local uid = playerInfo.uid
    local ackCommand = playerInfo.ackCommand
    local directionX = playerInfo.directionX
    local directionY = playerInfo.directionY
    local nmass = playerInfo.nmass
    local finalPointX = playerInfo.finalPointX
    local finalPointY = playerInfo.finalPointY
    local stopped = playerInfo.stopped
    local curAllPlayerNodeIds = self.gameManager:GetAllPlayerNodeIdxs(uid)
    local oldPlayerNodeIdCache = {}
    for _, oldIdx in ipairs(curAllPlayerNodeIds) do
        oldPlayerNodeIdCache[oldIdx] = true
    end
    local newPlayerNodeIdCache = {}
    
    -- self.gameManager:SyncServerAckCommand(g_user_info.get_user_info().uid, ackCommand)
    self.gameManager:ClientSyncServerPlayerInfo(uid, nmass, directionX, directionY, ackCommand, finalPointX, finalPointY, stopped)
    for _, node in ipairs(playerInfo.nodes or {}) do
        local cd = node.cd
        local fromId = node.fromId
        local idx = node.idx
        newPlayerNodeIdCache[idx] = true
        local initDeltaSpeed = node.initDeltaSpeed
        local initSpeed = node.initSpeed
        local initStopFrame = node.initStopFrame
        local mass = node.mass
        local protect = node.protect
        local x = node.x
        local y = node.y
        local speedX = node.speedX
        local speedY = node.speedY
        if oldPlayerNodeIdCache[idx] then --只是同步数据
            self.gameManager:ClientSyncServerPlayerNode(uid, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX, speedY)
            local curX, curY = x / 1000, y / 1000
            local oldX, oldY = curX, curY
            local node = self.playerManager:GetPlayerNodeObject(idx)
            if node then
                oldX, oldY = node:GetDisplayPosition()
            end
            -- local oldX, oldY = self
            local canPull, result = self:_isNeedPullNodePosition(uid, idx, x, y, oldX, oldY)
            if canPull then
                self.gameManager:ClientSyncRenderToLocation(idx)
            end           
        else --新的节点
            if initStopFrame > 0 and fromId ~= 0 then --分裂出来的球
                self.gameManager:CreateSplitPlayerNodeFromServer(uid, idx, fromId, x, y, mass, speedX, speedY, cd, protect, initStopFrame, initSpeed, initDeltaSpeed)
            else --复活出来的球
                self.gameManager:CreatePlayerNodeFromServer(uid, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX, speedY)
            end
            node.uid = uid
            self.playerManager:AddNewPlayerNode(node)
        end
    end
    if uid ~= g_user_info.get_user_info().uid then
        self.gameManager:RemovePlayerCommand(uid)
        -- message("同步了数据")
    end

    for oldNodeIdx, _ in pairs(oldPlayerNodeIdCache or {}) do
        if not newPlayerNodeIdCache[oldNodeIdx] then
            -- print("移除节点  ", oldNodeIdx)
            self.gameManager:RemovePlayerNodeFromServer(uid, oldNodeIdx)
            self.playerManager:RemovePlayerNode(uid, oldNodeIdx)

            if uid == g_user_info.get_user_info().uid then
                local mainPlayer = self._mainPanel.playerManager:GetMyPlayerObject()
                if mainPlayer and mainPlayer:IsDead() then
                    local moveManager = self._mainPanel:GetMoveManager()
                    moveManager:ResetManager()
                    g_panel_mgr.show('ballsbattle_cc_new.panel.dlg_ballsbattle_cc_revive_panel')
                end
            end
        end
    end



    local lastTime = utils_get_tick()
    if uid == g_user_info.get_user_info().uid then
        self.gameManager:ClientCatchup(uid)
        self.gameManager:CalcBallDelta(uid)
    end
    
    local curTime = utils_get_tick()
    if curTime - lastTime > 0.1 then
        __G__TRACKBACK__("cost too much time"..curTime - lastTime)
    else
        -- print("normal cost time ", curTime - lastTime)
    end
    self.lastAckCache[uid] = self._mainPanel.frame
end
