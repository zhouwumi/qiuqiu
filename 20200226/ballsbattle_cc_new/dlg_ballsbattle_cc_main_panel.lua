Panel = g_panel_mgr.new_panel_class('ballsbattle_cc/ballsbattle_cc_main_panel')

local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

local RemoteScene = import('logic.dialog.ballsbattle_cc_new.component.ballsbattle_cc_remotescene_sp').RemoteScene
local BBCCBgComponent = import('logic.dialog.ballsbattle_cc_new.component.ballsbattle_cc_bg_component').BBCCBgComponent
local BBCCPingSpComponent = import('logic.dialog.ballsbattle_cc_new.component.ballsbattle_cc_net_ping_sp').BBCCPingSpComponent
local BBCCDirectorComponent = import('logic.dialog.ballsbattle_cc_new.component.bb_cc_director_component').BBCCDirectorComponent

local BBCCServerComponent = import('logic.dialog.ballsbattle_cc_new.server.ballsbattle_cc_server_component').BBCCServerComponent
local BBCCClientSendToServerComponent = import('logic.dialog.ballsbattle_cc_new.server.ballsbattle_cc_client_send_to_server_component').BBCCClientSendToServerComponent
local BBCCRobotComponent = import('logic.dialog.ballsbattle_cc_new.server.ballsbattle_cc_robot_component').BBCCRobotComponent

local BBCCFoodManager = import('logic.dialog.ballsbattle_cc_new.manager.ballsbattle_cc_food_manager').BBCCFoodManager
local BBCCSpikySporeManager = import('logic.dialog.ballsbattle_cc_new.manager.ballsbattle_cc_spiky_or_spore_manager').BBCCSpikySporeManager
local BBCCPlayerManager = import('logic.dialog.ballsbattle_cc_new.manager.ballsbattle_cc_player_manager').BBCCPlayerManager
local BBCCFrameManager = import('logic.dialog.ballsbattle_cc_new.manager.bb_cc_frame_manager').BBCCFrameManager
local BBCCInfoManager = import('logic.dialog.ballsbattle_cc_new.manager.bb_cc_info_manager').BBCCInfoManager

local BBCCRightUI = import('logic.dialog.ballsbattle_cc_new.ui.ballsbattle_cc_right_ui').BBCCRightUI

function Panel:init_panel()
    if not g_user_info.get_user_info().uid then
        g_user_info.get_user_info().uid = 10000
    end
    local winSize = g_director:getWinSize()
    self.winW, self.winH = winSize.width, winSize.height
	self.bgLayer:SetEnableCascadeOpacityRecursion(true)
    self.bgLayer:setOpacity(125)
    self.foodTemplate = g_uisystem.load_template('ballsbattle_cc/balls_food')
    self.spikyTemplate = g_uisystem.load_template('ballsbattle_cc/balls_spiky')
    self.playerTemplate = g_uisystem.load_template('ballsbattle_cc/balls_player_node')
    self.sporeTemplate = g_uisystem.load_template('ballsbattle_cc/balls_spore')
    self.ballFakeTemplate = g_uisystem.load_template('ballsbattle_cc/balls_fake_ball')

    self:_initUI()
    self:_initGame()
    self.isFirst = true
    local delay_time = {30, 60, 100, 200, 600, 400}
    -- self.lastIdx = 1
    self:get_layer():DelayCall(10, function()
        -- self.lastIdx = self.lastIdx % 2 + 1
        local next_idx = math.ceil(math.random() * #delay_time)
        local time = delay_time[next_idx]
        constant_ballsbattle_cc.SimulateDelay = time
        message("网速切换了....   "..time)
        return 10
    end)
end

function Panel:_initUI()
    self.remoteScene = RemoteScene:New(self, self.remoteSceneNode)
    self.bgComponent = BBCCBgComponent:New(self)
    self.directorComponent = BBCCDirectorComponent:New(self)
    self.pingComponent = BBCCPingSpComponent:New(self)

    self.foodContainer = cc.Node:create()
    self.bg_container:AddChild(nil, self.foodContainer)

    self.sporeContainer = cc.Node:create()
    self.bg_container:AddChild(nil, self.sporeContainer)

    self.playerContainer = cc.Node:create()
    self.bg_container:AddChild(nil, self.playerContainer)

    self.spikyContainer = cc.Node:create()
    self.bg_container:AddChild(nil, self.spikyContainer)

    self.rightUI = BBCCRightUI:New(self)
    logic_utils_set_battery_component(self.batteryNode)
end

function Panel:_initGame()
    self.serverComponent = BBCCServerComponent:New(self)
    self.clientSendToServerComponent = BBCCClientSendToServerComponent:New(self)
    self:_initGameManager()
    self.foodManager = BBCCFoodManager:New(self)
    self.spikySporeManager = BBCCSpikySporeManager:New(self)
    self.playerManager = BBCCPlayerManager:New(self)
    self.infoManager = BBCCInfoManager:New(self)
    self:get_layer():DelayCall(1, function()
        self.serverComponent:JoinPlayer(g_user_info.get_user_info().uid)
    end)


    for index = 1, constant_ballsbattle_cc.RobotCnt do
        BBCCRobotComponent:New(self, index)
    end 
    
    self.frameManager = BBCCFrameManager:New(self)    
end

function Panel:_initGameManager()
    if constant_ballsbattle_cc.UseLuaVersion then
        self.gameManager = import('logic.dialog.ballsbattle_cc_new.game.manager.bb_game_main_game_manager').BBGameMainGameManager:New()
    else
        if self.gameManager then
            cc.BBGameManager:Destory(self.gameManager)
            self.gameManager = nil
        end
        self.gameManager = cc.BBClientGameManager:Create()
        self.gameManager:InitRoom()
        self.gameManager:SetUserId(g_user_info.get_user_info().uid)
        self.gameManager:SetServerState(false)
        -- self.gameManager:SetCanEatFoodSpiky(true)
        -- self.gameManager:SetCanEatSpore(true)
    end
    -- local configManager = self:GetConfigManager()
    -- configManager:SetLogState(constant_ballsbattle_cc.IsCOpenLog)
    -- -- configManager:SetMaxCommandNoAck(30)
    -- configManager:SetDebugOtherBallMas(50)
    -- configManager:SetDebugMyBallMass(200)
    -- configManager:SetMinSpikyMass(30)
    -- configManager:SetMaxSpikyMass(32)
    -- configManager:SetClientDebug(true)
end

function Panel:GetMoveManager()
    local moveManager = nil
    if constant_ballsbattle_cc.UseLuaVersion then
        moveManager = self.gameManager.moveManager
    else
        moveManager = cc.BBMoveManager
    end
    return moveManager
end

function Panel:GetConfigManager()
    if constant_ballsbattle_cc.UseLuaVersion then
        return self.gameManager.configManager
    end
    return cc.BBConfigManager
end

function Panel:ChangeDesk()
    self:ReEnter()
    self:get_layer():DelayCall(1, function()
        self.serverComponent:JoinPlayer(g_user_info.get_user_info().uid)
    end)
end

function Panel:ReEnter()
    if self._delayCall then
        self._delayCall()
        self._delayCall = nil
    end
    self.frame = 0
    self:_initGameManager()
    
    self.foodManager:ReEnter()
    self.spikySporeManager:ReEnter()
    self.playerManager:ReEnter()
    self.infoManager:ReEnter()
    self.frameManager:ReEnter()
end

function Panel:RunLoop()
    if self._delayCall then
        return
    end

    self.frame = 0
    self.foceUpdate = false
    self._delayCall = delay_call(0, function()
        self.frame = self.frame + 1
        self.labelFrame:SetString(tostring(self.frame))
        
        self.frameManager:OnUpdate()
        self.directorComponent:OnUpdate()
        -- self.pingComponent:OnUpdate()

        local myPlayerObject = self.playerManager:GetMyPlayerObject()
        if myPlayerObject then
            local startx, starty, endx ,endy = myPlayerObject:GetRectCenter()
            self.labelPosition:SetString(math.floor((startx + endx) / 2).." : "..math.floor((starty + endy) / 2))
        end
        return constant_ballsbattle_cc.RenderFPS
    end)
end

function Panel:showDirectorMessage(id)
    self.directorComponent:addSpeicalMessage(id)
end

function Panel:on_before_destroy()
    if not constant_ballsbattle_cc.UseLuaVersion then
        if cc.BBGameManager then
            cc.BBGameManager:Destory(self.gameManager)
        end
    end
    
    if self._delayCall then
        self._delayCall()
        self._delayCall = nil
    end
end

function Panel:esc_key_close()
    confirm_yes_no(T("您的退出有可能影响其他玩家的游戏体验，是否确认退出？"), function()
        self:GetMoveManager():ResetManager()
        if g_game_mgr.is_in_game_hall() then
            self:close_panel()
        else
            g_game_mgr.exit_game_cur_game()
        end
    end)
end