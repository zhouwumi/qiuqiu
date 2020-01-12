Panel = g_panel_mgr.new_panel_class('ballsbattle_cc/ballsbattle_cc_main_panel')

local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

local Live2DSprite = import('logic.dialog.ballsbattle_cc.component.ballsbattle_cc_live_2d').Live2DSprite
local RemoteScene = import('logic.dialog.ballsbattle_cc.component.ballsbattle_cc_remotescene_sp').RemoteScene
local BBCCBgComponent = import('logic.dialog.ballsbattle_cc.component.ballsbattle_cc_bg_component').BBCCBgComponent
local BBCCPingSp = import('logic.dialog.ballsbattle_cc.component.ballsbattle_cc_net_ping_sp').BBCCPingSp

local FoodObject = import('logic.dialog.ballsbattle_cc.objects.bb_food_object').FoodObject
local SpikyObject = import('logic.dialog.ballsbattle_cc.objects.bb_spiky_object').SpikyObject
local PlayerNodeObject = import('logic.dialog.ballsbattle_cc.objects.bb_player_object_node').PlayerNodeObject
local PlayerObject = import('logic.dialog.ballsbattle_cc.objects.bb_player_object').PlayerObject


local BBCCFoodManager = import('logic.dialog.ballsbattle_cc.manager.ballsbattle_cc_food_manager').BBCCFoodManager
local BBCCSpikySporeManager = import('logic.dialog.ballsbattle_cc.manager.ballsbattle_cc_spiky_or_spore_manager').BBCCSpikySporeManager
local BBCCObjectManager = import('logic.dialog.ballsbattle_cc.manager.ballsbattle_cc_object_manager').BBCCObjectManager
local BBCCPlayerManager = import('logic.dialog.ballsbattle_cc.manager.ballsbattle_cc_player_manager').BBCCPlayerManager
-- local BBCCFrameManager = import('logic.dialog.ballsbattle_cc.manager.ballsbattle_cc_frame_manager').BBCCFrameManager
local BBCCFrameManager = import('logic.dialog.ballsbattle_cc.manager.ballsbattle_cc_frame_manager_version2').BBCCFrameManager

local BBCCRightUI = import('logic.dialog.ballsbattle_cc.ui.ballsbattle_cc_right_ui').BBCCRightUI

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
    self.gameManager = cc.BBGameManager:Create()
    self.foodManager = BBCCFoodManager:New(self)
    self.spikySporeManager = BBCCSpikySporeManager:New(self)
    self.objectManager = BBCCObjectManager:New(self)
    self.playerManager = BBCCPlayerManager:New(self)
    self.frameManager = BBCCFrameManager:New(self)

    self.gameManager:InitRoom()
    if constant_ballsbattle_cc.IsDebugBattleCC then
        self.gameManager:SetServerState(true)
        self:DebugTest()
    else
        self.gameManager:SetServerState(false)
    end
end

function Panel:DebugTest()
    local foodInfos = self.gameManager:ClientGenerateFood(600)
    local spikyInfos = self.gameManager:ClientGenerateSpiky(30)
    for index = 1, #foodInfos, 2 do
        local idx = foodInfos[index]
        local pos = foodInfos[index + 1]
        self.gameManager:AddNewFoodFromServer(pos)
    end
    for index = 1, #spikyInfos, 3 do
        local idx = spikyInfos[index]
        local pos = spikyInfos[index + 1]
        local mass = spikyInfos[index + 2]
        self.gameManager:AddNewSpikyFromServer(idx, pos, mass)
    end

    self.objectManager:GenerateFoodAndSpiky()
    self.currentPlayerIdx = g_user_info.get_user_info().uid
    self.frame = 0
    self.foceUpdate = true
    self._delayCall = delay_call(1 / 30, function()
        self.frame = self.frame + 1
        self.remoteScene:SendDir()
        self.foodManager:OnUpdateFood(self.foceUpdate)
        self.spikySporeManager:OnUpdateSpiky(self.foceUpdate)
        self.foceUpdate = false
        -- self.gameManager:OnUpdate()
        self.gameManager:OnFrameUpdate()
        if self.frame % 5 == 0 then
            self.gameManager:OnKeyFrameUpdate()
        end
        self.playerManager:Update()
        self.objectManager:Update()
        self.bgComponent:Update()
         if self.frame % 5 == 0 then
            self.gameManager:EndUpdate()
        end
        return 1 / 30
    end)
end

function Panel:OnRoomInfoPush(data)
    self.frameManager:OnGetFirstState(data)
    self.objectManager:OnGetFirstState(data)
    self:get_layer():DelayCall(1 / 30, function()
        self:QuickHandleFrames()
        if #self.frameManager.frameDatas > 0 then
            return 1 / 30
        end
        self:StartHandleLoop()
    end)
    
end

function Panel:QuickHandleFrames()
    if #self.frameManager.frameDatas <= 0 then
        return
    end
    for index = 1, 5 do
        self.frameManager:OnUpdate()
    end
    self.bgComponent:Update()
    self.foodManager:OnUpdateFood(true)
    self.spikySporeManager:OnUpdateSpiky(true)
end


function Panel:StartHandleLoop()
    self.hasCompeleteQuickHandleFrame = true
    self.foceUpdate = true
    self._delayCall = delay_call(1 / 30, function()
        self.remoteScene:SendDir()
        self.foodManager:OnUpdateFood(self.foceUpdate)
        self.spikySporeManager:OnUpdateSpiky(self.foceUpdate)
        self.foceUpdate = false
        self:GameLoop()
        return 1 / 30
    end)
end

function Panel:GameLoop()
    local runTimes = math.max(#self.frameManager.frameDatas, 1)
    runTimes = 1
    for index = 1, runTimes do
        if constant_ballsbattle_cc.IsSupportSimulate then
            self.frameManager:OnUpdateWithSimulate()
        else
            self.frameManager:OnUpdate()
        end
    end
    self.bgComponent:Update()
    self.playerManager:UpdateAllPlayers()
    self.objectManager:UpdateSpore()
end

function Panel:IsKeyFrame()
    return self.frameManager.isKeyFrameUpdate
end

function Panel:on_before_destroy()
    if cc.BBGameManager then
        if self.hitHandle then
            self.gameManager:RemoveEatCbFunction(self.hitHandle)
            self.hitHandle = nil
        end
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