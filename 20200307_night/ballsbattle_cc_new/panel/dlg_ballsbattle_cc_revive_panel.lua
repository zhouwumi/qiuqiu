Panel = g_panel_mgr.new_panel_class('ballsbattle/ballsbattle_revive_panel')
local balls_common_config = g_conf_mgr.get_conf("balls_common_config")
local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')

local loading_key = 'BALLSBATTLE_REVIVE'

function Panel:init_panel()
    self.reviveFrame = tonumber(balls_common_config.respawn_time.value)
    -- self.net = g_game_mgr.get_game_net('GameType_BALLSBATTLE')

    self.btnYes.OnClick = function()
        --TODO 读取配置表
        if g_user_info.get_user_info().coins > tonumber(balls_common_config.respawn_cost.value) then
            message("还未接入服务器")
            -- show_loading_panel(loading_key, 30)
            -- self.net:Request_CMD_SLIME_RESPAWN_REQ()
            g_panel_mgr.run_on_panel("ballsbattle_cc_new.dlg_ballsbattle_cc_main_panel", function(panel)
                panel:showDirectorMessage(210)
            end)
        else
            message(T('金币不足'))
        end
    end
    
    self.labelrevive:setString(T("你被其他玩家吃掉了，是否立即复活？"))
    self.textbtn:setString(T("立即复活"))
    --TODO读取配置表完成
    self:get_layer():DelayCall(1, function()
        self.reviveFrame = self.reviveFrame - 1
        if self.reviveFrame > 0 then
           self.labelLeftTime:SetString(tostring(self.reviveFrame))
           return 1
        else
          self:onRevive()
           return
        end
    end)
end


function Panel:onRevive()
    self:close_panel()
end

function Panel:on_before_destroy()
    close_loading_panel(loading_key)
end

function Panel:esc_key_close()
    -- if g_native_conf['debug_control'].balls_debug_mode then
    --     self:close_panel()
    -- end
end