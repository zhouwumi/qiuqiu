Panel = g_panel_mgr.new_panel_class('ballsbattle/ballsbattle_main_panel')

local loading_key = "battleballs_cc"
local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local Live2DSprite = import('dialog/ballsbattle_cc/component/ballsbattle_cc_live_2d').Live2DSprite
local winSize = g_director:getWinSize()
local BallsBattleCCGameManager = import('dialog/ballsbattle_cc/manager/ballsbattle_cc_game_manager').BallsBattleCCGameManager
local BallsBattleCCDataManager = import('dialog/ballsbattle_cc/manager/ballsbattle_cc_data_manager').BallsBattleCCDataManager
local BallsBattleCCPlayerManager = import('dialog/ballsbattle_cc/manager/ballsbattle_cc_player_manager').BallsBattleCCPlayerManager

local BgContainer = import('dialog/ballsbattle_cc/component/ballsbattle_cc_bg_sp').Bg
local RemoteScene = import('dialog/ballsbattle_cc/component/ballsbattle_cc_remotescene_sp').RemoteScene
local RankList = import('dialog/ballsbattle_cc/component/ballsbattle_cc_rank_list_sp').RankList
local PingSp =  import('dialog/ballsbattle_cc/component/ballsbattle_cc_net_ping_sp').PingSp
local LeftTimeSp = import('dialog/ballsbattle_cc/component/ballsbattle_cc_lefttime_sp').LeftTimeSp
local FunctionSp = import('dialog/ballsbattle_cc/component/ballsbattle_function_sp').FunctionSp
local KillSp = import('dialog/ballsbattle_cc/component/ballsbattle_cc_kill_sp').KillSp
local DirectorSp = import('dialog/ballsbattle_cc/component/ballsbattle_cc_director_sp').DirectorSp

function Panel:init_panel()
	self.bgLayer:SetEnableCascadeOpacityRecursion(true)
    self.bgLayer:setOpacity(125)
    self.net = g_game_mgr.get_game_net('GameType_BALLSBATTLE')
    self:_initView()
    self:_initManager()
    self:_initComponent()
    self:_initEvent()
    self:getInitData()
end

function Panel:_initManager()
	self.gameManager = BallsBattleCCGameManager:New(self)
	self.dataManager = BallsBattleCCDataManager:New(self)
    self.playerManager = BallsBattleCCPlayerManager:New(self)
end

function Panel:_initComponent()
	self.bgContainer = BgContainer:New(self.bg_container, self.bg, self.line_container)
	self.remoteSp = RemoteScene:New(self.node)
    self.rankList = RankList:New(self.massRankNode)
    self.pingSp = PingSp:New(self.pingNode)
    self.leftimeSp = LeftTimeSp:New(self.lefttimeNode)
    self.functionSp = FunctionSp:New(self.functionNode)
    self.killSp = KillSp:New(self.killNode)
    self.directorSp = DirectorSp:New(self.driectNode)
end

function Panel:_initView()
    self.btnBig.OnClick = function()
        if g_native_conf['debug_mode'] then
            local ball = self.playerManager:GetMyPlayer().ballList[1]
            self.net:Request_CMD_SLIME_CHANGE_MASS_REQ(ball.idx,90)
        end
    end
    self.btnSmall.OnClick = function()
        self:get_layer():setScale(0.25)
        local ball = self.playerManager:GetMyPlayer().ballList[1]
        if ball.mass > 15 then
            self.net:Request_CMD_SLIME_CHANGE_MASS_REQ(ball.idx,-90)
        end
    end
    self.btnReturn.OnClick = function()
        confirm_yes_no(T("您的退出有可能影响其他玩家的游戏体验，是否确认退出？"), function()
            g_panel_mgr.close_cur_scene_and_panels()
        end)
    end
end

function Panel:_initEvent()
    import('logic_net.logic_paomadeng').logic:close_pamadeng_panel()
    g_logicEventHandler:Trigger('logic_audio_play_event', 'AUDIO_PLAY_MUSIC', constant_ballsbattle.MUSIC_MAIN_BG)

    g_eventHandler:AddCallback('event_applicationDidEnterBackground', function()
        self.enterBackgroundTime = utils_get_tick()
        if self.remoteSp ~= nil then
            self.remoteSp:Reset()
        end
    end, nil, nil, self)

    g_eventHandler:AddCallback('event_applicationWillEnterForeground', function()
        if self.enterBackgroundTime ~= nil and utils_get_tick() - self.enterBackgroundTime > 5 then
            self:_restartGame()
        end
        self.enterBackgroundTime = nil
        if self.remoteSp ~= nil then
            self.remoteSp:Reset()
        end
    end, nil, nil, self)

    self:get_layer():DelayCall(0.5, function()
        --减少计算
        if not self.isStart then
            return 0.1
        end
        self.rankList:update()
        return 0.5
    end)

    

    self:get_layer():PlayAnimation("bg_movie")
    g_game_mgr:get_cur_game():OnGameMainPanelOpen()
    -- 初始化电量和时间组件
    logic_utils_set_battery_component(self.batteryNode)
end

function Panel:getInitData()
    show_loading_panel(loading_key, 100)
    self.isStart = false
    local scaleWidth,scaleHight = winSize.width/constant_ballsbattle.DESIGN_SIZE[1], winSize.height/constant_ballsbattle.DESIGN_SIZE[2]
    local scale = math.min(scaleWidth,scaleHight)

    self.net:Request_CMD_SLIME_JOIN_REQ({math.ceil(winSize.width / scale) + 1000, math.ceil(winSize.height / scale) + 1000})
end

function Panel:onGetInitData(data)
	close_loading_panel(loading_key)
	self.dataManager:SetData(data)

    self.dataManager:OnGetGameData()
    self.gameManager:OnGetGameData()
    self.playerManager:OnGetGameData()

    self.isStart = true
    self:_startDelayCall()
end

function Panel:_restartGame()
    self:_clearDelayCall()
    self.gameManager:OnRestartGame()
    self.dataManager:OnRestartGame()
    self.playerManager:OnRestartGame()
    self:getInitData()
end

function Panel:on_before_destroy()
    self.gameManager:Destory()
    self.dataManager:Destory()
    self.playerManager:Destory()

    self:_clearDelayCall()

    g_game_mgr:get_cur_game():OnGameMainPanelClose()
    g_logicEventHandler:Trigger('logic_audio_play_event', 'AUDIO_PLAY_MUSIC', constant_ballsbattle.MUSIC_CHOOSE_ROOM)
end

function Panel:_clearDelayCall()
    if self._delayCall then
        self._delayCall('cancel')
        self._delayCall = nil
    end
end

function Panel:_startDelayCall()
    self._delayCall = delay_call(0, function()
        if self.isStart and self.playerManager:isReady() then
            self:_onTimer()
        end
        
        return constant_ballsbattle.INTERVAL_EVERY_FRAME
    end)
end

function Panel:_onTimer()
    self.dataManager:OnUpdate()
    self.gameManager:OnUpdate()
    self.playerManager:OnUpdate()

    self.pingSp:update()
    self.killSp:update()
    self.directorSp:update()
    self.leftimeSp:update()
    self.functionSp:update()

    self.bgContainer:updateDisplay()
end

function Panel:updateBG()
    self.bgContainer:update()
end

function Panel:onGetTickerData(data)
    self.isSendChangeMass = false
    -- BallsBattleService.on_get_ticker_data(data)
    self.gameManager:PushTickData(data)
end

function Panel:onPlayerChange(data)
    self.playerManager:onPlayerChange(data)
    -- BallsBattleService.on_player_change(data)
end

function Panel:showDirectorMessage(id)
    self.directorSp:addSpeicalMessage(id)
end


function Panel:onRevive()
    if self.bgContainer and self.isStart then
        self.bgContainer:onRevive()
    end
end