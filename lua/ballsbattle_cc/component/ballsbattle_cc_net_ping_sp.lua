local BallsBattleService = import('logic.dialog.ballsbattle.manager.ballsbattle_service')
local ballsUtils = import('logic_data.game_ballsbattle.ballsbattle_utils')

PingSp = CreateClass()

function PingSp:__init__(sp)
    self.massLabel = sp.massLabel
    self.pingSp = sp.pingSprite
    self.pingLabel = sp.pingLabel
    self.net = g_game_mgr.get_game_net('GameType_BALLSBATTLE')
end

function PingSp:update()
    local displayeName = ballsUtils.get_mass_displayname(BallsBattleService.get_my_player().totalMass or 0)
    self.massLabel:SetString(displayeName)

    local delayTime = math.floor(self.net:getNetPing() * 1000)
    local color
    if delayTime > 400 then
        color = 'hong'
    elseif delayTime > 200 then
        color = 'huang'
    else
        color = 'lv'
    end

    -- if g_native_conf['debug_control'].balls_debug_mode then
    --     self.pingLabel:setVisible(true)
    --     self.pingLabel:SetString(delayTime..'ms')
    -- else
        self.pingLabel:setVisible(false)
    -- end 

    self.pingSp:SetPath('','gui/badam_ui/ballsbattle/choose_room_panel/wifi_'..color..'.png')
end