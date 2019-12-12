FunctionSp = CreateClass()
local balls_common_config = g_conf_mgr.get_conf("balls_common_config")
local Ballsbattle_Service = import('logic.dialog.ballsbattle.manager.ballsbattle_service')

function FunctionSp:__init__(sp)
    self.net = g_game_mgr.get_game_net('GameType_BALLSBATTLE')
    self.sp = sp
    self.frame = 5
    self.isTouch = false
    
    self:initBtn()
    self.myPlayer = Ballsbattle_Service.get_my_player()
end


function FunctionSp:update()
    self.frame = self.frame + 1
    self:sendShoot()
end

function FunctionSp:initBtn()
    self.sp.btnSplit.OnClick = function()
        if #self.myPlayer:getBallList() == tonumber(balls_common_config.max_cell.value) then
            --球体数量超过12
            g_panel_mgr.run_on_panel("ballsbattle.dlg_ballsbattle_main_panel", function(panel)
                panel:showDirectorMessage(212)
            end)
        elseif not self.myPlayer:canDoFunction() then
            --球体质量不超过40全部
            g_panel_mgr.run_on_panel("ballsbattle.dlg_ballsbattle_main_panel", function(panel)
                panel:showDirectorMessage(213)
            end)
        else
            g_panel_mgr.run_on_panel("ballsbattle.dlg_ballsbattle_main_panel", function(panel)
                panel:showDirectorMessage(208)
            end)
            self.net:Request_CMD_SLIME_SPLIT_REQ()
        end
    end
    
    self.sp.btnThrow.OnBegin = function()
        self.isTouch = true
        return true
    end

    self.sp.btnThrow.OnEnd = function()
        if not self.myPlayer:canDoFunction() then
            --球体质量不超过40全部
            g_panel_mgr.run_on_panel("ballsbattle.dlg_ballsbattle_main_panel", function(panel)
                panel:showDirectorMessage(214)
            end)
        else
            self:sendShoot()
        end
        self.isTouch = false
    end
end


function FunctionSp:sendShoot()
    if self.frame > 4 and self.isTouch then
        if not self.myPlayer:canDoFunction() then
            --球体质量不超过40全部
            g_panel_mgr.run_on_panel("ballsbattle.dlg_ballsbattle_main_panel", function(panel)
                panel:showDirectorMessage(214)
            end)
        else
            self.frame = 0
            g_panel_mgr.run_on_panel("ballsbattle.dlg_ballsbattle_main_panel", function(panel)
                panel:showDirectorMessage(207)
            end)
            self.net:Request_CMD_SLIME_SHOOT_REQ()
        end
    end
end