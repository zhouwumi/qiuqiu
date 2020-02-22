Panel = g_panel_mgr.new_panel_class('ballsbattle_cc/ballsbattle_cc_main_panel')

local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

local Live2DSprite = import('logic.dialog.ballsbattle_cc_new.component.ballsbattle_cc_live_2d').Live2DSprite
local RemoteScene = import('logic.dialog.ballsbattle_cc_new.component.ballsbattle_cc_remotescene_sp').RemoteScene
local BBCCBgComponent = import('logic.dialog.ballsbattle_cc_new.component.ballsbattle_cc_bg_component').BBCCBgComponent
local BBCCPingSp = import('logic.dialog.ballsbattle_cc_new.component.ballsbattle_cc_net_ping_sp').BBCCPingSp

local FoodObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_food_object').FoodObject
local SpikyObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_spiky_object').SpikyObject
local PlayerNodeObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_player_object_node').PlayerNodeObject
local PlayerObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_player_object').PlayerObject

local BBCCServerComponent = import('logic.dialog.ballsbattle_cc_new.server.ballsbattle_cc_server_component').BBCCServerComponent
local BBCCClientSendToServerComponent = import('logic.dialog.ballsbattle_cc_new.server.ballsbattle_cc_client_send_to_server_component').BBCCClientSendToServerComponent

local BBCCFoodManager = import('logic.dialog.ballsbattle_cc_new.manager.ballsbattle_cc_food_manager').BBCCFoodManager
local BBCCSpikySporeManager = import('logic.dialog.ballsbattle_cc_new.manager.ballsbattle_cc_spiky_or_spore_manager').BBCCSpikySporeManager
local BBCCObjectManager = import('logic.dialog.ballsbattle_cc_new.manager.ballsbattle_cc_object_manager').BBCCObjectManager
local BBCCPlayerManager = import('logic.dialog.ballsbattle_cc_new.manager.ballsbattle_cc_player_manager').BBCCPlayerManager
local BBCCFrameManager = import('logic.dialog.ballsbattle_cc_new.manager.ballsbattle_cc_frame_manager').BBCCFrameManager

local BBCCRightUI = import('logic.dialog.ballsbattle_cc_new.ui.ballsbattle_cc_right_ui').BBCCRightUI

function Panel:init_panel()
    local winSize = g_director:getWinSize()
    self.winW, self.winH = winSize.width, winSize.height
	self.bgLayer:SetEnableCascadeOpacityRecursion(true)
    self.foodTemplate = g_uisystem.load_template('ballsbattle_cc/balls_food')
    self.spikyTemplate = g_uisystem.load_template('ballsbattle_cc/balls_spiky')
    self.playerTemplate = g_uisystem.load_template('ballsbattle_cc/balls_player_node')
    self.sporeTemplate = g_uisystem.load_template('ballsbattle_cc/balls_spore')

    self:_initUI()
    self:_initGame()
    self.currentPlayerIdx = g_user_info.get_user_info().uid
end

function Panel:_initUI()
    self.remoteScene = RemoteScene:New(self, self.remoteSceneNode)
    self.bgComponent = BBCCBgComponent:New(self)
    -- self.pingComponent = BBCCPingSp:New(self)

    self.foodContainer = cc.Node:create()
    self.bg_container:AddChild(nil, self.foodContainer)

    self.sporeContainer = cc.Node:create()
    self.bg_container:AddChild(nil, self.sporeContainer)

    self.spikyContainer = cc.Node:create()
    self.bg_container:AddChild(nil, self.spikyContainer)

    self.playerContainer = cc.Node:create()
    self.bg_container:AddChild(nil, self.playerContainer)

    self.rightUI = BBCCRightUI:New(self)
end

function Panel:_initGame()
    if not cc.BBGameManager then
        return
    end
    self.serverComponent = BBCCServerComponent:New(self)
    self.clientSendToServerComponent = BBCCClientSendToServerComponent:New(self)
    self.gameManager = cc.BBGameManager:Create()
    self.foodManager = BBCCFoodManager:New(self)
    self.spikySporeManager = BBCCSpikySporeManager:New(self)
    self.objectManager = BBCCObjectManager:New(self)
    self.playerManager = BBCCPlayerManager:New(self)
    -- self.frameManager = BBCCFrameManager:New(self)

    self.gameManager:InitRoom()
    self.gameManager:SetUserId(g_user_info.get_user_info().uid)
    -- if constant_ballsbattle_cc.IsDebugBattleCC then
    --     self.gameManager:SetServerState(true)
    --     self:DebugTest()
    -- else
    --     self.gameManager:SetServerState(false)
    -- end
    self.gameManager:SetServerState(false)
    self:get_layer():DelayCall(1, function()
        self.serverComponent:JoinPlayer(g_user_info.get_user_info().uid)
    end)
end

function Panel:UpdateDataFromServer(data)
    if data.foodInfos then
        for index = 1, #data.foodInfos do
            local idx = data.foodInfos[index]
            self.gameManager:AddNewFoodFromServer(idx)
        end
    end

    if data.spikyInfos then
        for index = 1, #data.spikyInfos do
            local idx = data.spikyInfos[index]
            local pos = data.spikyInfos[index + 1]
            local mass = data.spikyInfos[index + 2]
            self.gameManager:AddNewSpikyFromServer(idx, pos, mass)
        end
    end

    if data.foodInfos or data.spikyInfos then
        self.objectManager:GenerateFoodAndSpiky()
    end

    local isMeOK = false
    local newPlayers = data.newPlayers
    for _, playerInfo in ipairs(newPlayers) do
        local playerId, playerIdx, directionX, directionY, nmass, nextNodeIdx = unpack(playerInfo)
        print("新加入一个玩家", playerId, playerIdx, directionX, directionY, nmass, nextNodeIdx)
        self.gameManager:CreatePlayerFromServer(playerId, playerIdx, directionX, directionY, nmass, nextNodeIdx)
        if playerId == g_user_info.get_user_info().uid then
            isMeOK = true
        end
    end

    local newPlayerNodes = data.newPlayerNodes or {}
    for _, nodeInfo in ipairs(newPlayerNodes) do
        local playerId, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed = unpack(nodeInfo)
        print("新加入一个节点  ", playerId, idx)
        self.gameManager:CreatePlayerNodeFromServer(playerId, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed)
        print("加入节点结束  ", playerId)
    end

    self.playerManager:GeneratePlayerAndNodes(newPlayers, newPlayerNodes)
    self.bgComponent:Update()
    self.foodManager:OnUpdateFood(true)
    self.spikySporeManager:OnUpdateSpiky(true)
    if isMeOK then
        self:RunLoop()
    end
end

function Panel:UpdateServerAck(serverAcks)
    if not serverAcks then
        return
    end
    local myAck = serverAcks[g_user_info.get_user_info().uid]
    if not myAck then
        return
    end
    self.gameManager:SyncServerAckCommand(g_user_info.get_user_info().uid, myAck)
    print("server ack ", myAck)
end

function Panel:UpdatePlayerMismatch(misMatchPlayerInfos)
    if not misMatchPlayerInfos then
        return
    end
    local playerInfo = misMatchPlayerInfos[g_user_info.get_user_info().uid]
    if not playerInfo then return end
    print("sync mismatch info from server:  ",playerInfo)
    self:SyncServerInfo(playerInfo)
end

-- [ackCommand] = 74,
--     [directionX] = 2270,
--     [directionY] = 100,
--     [nextNodeIdx] = 2,
--     [nmass] = 894,
--     [nodes] = {
--         [1] = {
--             [cd] = 0,
--             [fromId] = 0,
--             [idx] = 1671546417,
--             [initDeltaSpeed] = 0,
--             [initSpeed] = 0,
--             [initStopFrame] = 0,
--             [mass] = 19,
--             [protect] = 0,
--             [x] = 3708.985,
--             [y] = 31,
--         },
--     },
--     [playerIdx] = 1,
function Panel:SyncServerInfo(playerInfo)
    local ackCommand = playerInfo.ackCommand
    local directionX = playerInfo.directionX
    local directionY = playerInfo.directionY
    local nextNodeIdx = playerInfo.nextNodeIdx
    local nmass = playerInfo.nmass
    self.gameManager:ClientSyncServerPlayerInfo(g_user_info.get_user_info().uid, nextNodeIdx, nmass, directionX, directionY, ackCommand)
    for _, node in ipairs(playerInfo.nodes or {}) do
        local cd = node.cd
        local fromId = node.fromId
        local idx = node.idx
        local initDeltaSpeed = node.initDeltaSpeed
        local initSpeed = node.initSpeed
        local initStopFrame = node.initStopFrame
        local mass = node.mass
        local protect = node.protect
        local x = node.x
        local y = node.y
        self.gameManager:ClientSyncServerPlayerNode(g_user_info.get_user_info().uid, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed)
    end
    self.gameManager:ClientCatchup(g_user_info.get_user_info().uid)
    self.gameManager:CalcBallDelta(g_user_info.get_user_info().uid)
end

function Panel:RunLoop()
    local frame = 0
    self.foceUpdate = false
    self._delayCall = delay_call(0, function()
        frame = frame + 1
        self.foodManager:OnUpdateFood(self.foceUpdate)
        self.spikySporeManager:OnUpdateSpiky(self.foceUpdate)
        self.foceUpdate = false

        self.isKeyFrame = frame % constant_ballsbattle_cc.FrameRate == 0
        self.playerManager:Update()
        
        if self.isKeyFrame then
            local angle, pressure, isShoot, isSplit, commandIdx, checkSum
            local myPlayerObject = self.playerManager:GetMyPlayerObject()
            local hasOperate = false
            if myPlayerObject and not myPlayerObject:IsDead() then
                if self.gameManager:CanAddPlayerCommand(g_user_info.get_user_info().uid) then
                    hasOperate = true
                    cc.BBMoveManager:IncreaseFrame()
                    angle = cc.BBMoveManager:GetAngle()
                    pressure = cc.BBMoveManager:GetPressure()
                    isShoot = cc.BBMoveManager:IsShoot()
                    isSplit = cc.BBMoveManager:IsSplit()
                    commandIdx = cc.BBMoveManager:GetNextIndex()
                    if pressure ~= 0 then
                        print("客户端执行命令  ",angle, pressure, isSplit, isShoot, commandIdx)
                    end
                    self.gameManager:AddPlayerCommand(self.currentPlayerIdx, angle, pressure, isSplit, isShoot, commandIdx, 0)
                end
            end
            self.gameManager:FrameUpdate()
            -- self.playerManager:GetFrameNewPlayers()
            
            if hasOperate then
                local checkSum = cc.BBMoveManager:GetCheckSum()
                print("客户端执行的checkSum  ", checkSum)
                self.clientSendToServerComponent:AddPlayerCommand(self.currentPlayerIdx, angle, pressure, isSplit, isShoot, commandIdx, checkSum)
            end
        end        
        self.bgComponent:Update()
        return constant_ballsbattle_cc.RenderFPS
    end)
end

function Panel:DebugTest()
    local foodInfos = self.gameManager:ClientGenerateFood(600)
    local spikyInfos = self.gameManager:ClientGenerateSpiky(30)
    for index = 1, #foodInfos do
        local idx = foodInfos[index]
        self.gameManager:AddNewFoodFromServer(idx)
    end
    for index = 1, #spikyInfos, 3 do
        local idx = spikyInfos[index]
        local pos = spikyInfos[index + 1]
        local mass = spikyInfos[index + 2]
        self.gameManager:AddNewSpikyFromServer(idx, pos, mass)
    end

    self.objectManager:GenerateFoodAndSpiky()
    self.currentPlayerIdx = g_user_info.get_user_info().uid
    -- self.frame = 0
    self.foceUpdate = true
    local frame = 0
    self._delayCall = delay_call(0, function()
        frame = frame + 1
    --     self.frame = self.frame + 1
    --     self.remoteScene:SendDir()
        self.foodManager:OnUpdateFood(self.foceUpdate)
        self.spikySporeManager:OnUpdateSpiky(self.foceUpdate)
        self.foceUpdate = false
        local myPlayerObject = self.playerManager:GetMyPlayerObject()
        if myPlayerObject and not myPlayerObject:IsDead() then
            cc.BBMoveManager:IncreaseFrame()
            local angle = cc.BBMoveManager:GetAngle()
            local pressure = cc.BBMoveManager:GetPressure()
            local isShoot = cc.BBMoveManager:IsShoot()
            local isSplit = cc.BBMoveManager:IsSplit()
            local nextIndex = cc.BBMoveManager:GetNextIndex()
            self.gameManager:AddPlayerCommand(self.currentPlayerIdx, angle, pressure, isSplit, isShoot, nextIndex, 0)
        end

        self.isKeyFrame = frame % constant_ballsbattle_cc.FrameRate == 0
        self.playerManager:Update()
        
        if self.isKeyFrame then
            self.gameManager:FrameUpdate()
            self.playerManager:GetFrameNewPlayers()
        end
        
        
        self.bgComponent:Update()
    --     -- self.gameManager:OnUpdate()
    --     self.gameManager:OnFrameUpdate()
    --     if self.frame % 5 == 0 then
    --         self.gameManager:OnKeyFrameUpdate()
    --     end
    --     self.playerManager:Update()
    --     self.objectManager:Update()
    --     self.bgComponent:Update()
    --      if self.frame % 5 == 0 then
    --         self.gameManager:EndUpdate()
    --     end
        return constant_ballsbattle_cc.RenderFPS
    end)
end

function Panel:on_before_destroy()
    if cc.BBGameManager then
        cc.BBGameManager:Destory(self.gameManager)
    end
    if self._delayCall then
        self._delayCall('cancel')
        self._delayCall = nil
    end
end

function Panel:esc_key_close()
    confirm_yes_no(T("您的退出有可能影响其他玩家的游戏体验，是否确认退出？"), function()
        if g_game_mgr.is_in_game_hall() then
            self:close_panel()
        else
            g_game_mgr.exit_game_cur_game()
        end
    end)
end