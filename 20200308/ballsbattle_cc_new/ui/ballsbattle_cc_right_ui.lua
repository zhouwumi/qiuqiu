BBCCRightUI = CreateClass()

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
function BBCCRightUI:__init__(mainPanel)
	self._mainPanel = mainPanel

    mainPanel.btnPlayerSplit.OnClick = function()
        local moveManager = self._mainPanel:GetMoveManager()
        moveManager:Split()
    end
    -- mainPanel.btnPlayerShoot.OnClick = function()
    --     local moveManager = self._mainPanel:GetMoveManager()
    --     moveManager:Shoot()
    -- end
    mainPanel.btnPlayerShoot.OnBegin = function()
        self.isTouch = true
        local moveManager = self._mainPanel:GetMoveManager()
        moveManager:Shoot()
        delay_call(1 / 6, function()
            if not self.isTouch then
                return
            end
            moveManager:Shoot()
            return 1 / 6
        end)
        return true
    end

    mainPanel.btnPlayerShoot.OnEnd = function()
        -- if not self.myPlayer:canDoFunction() then
        --     --球体质量不超过40全部
        --     g_panel_mgr.run_on_panel("ballsbattle.dlg_ballsbattle_main_panel", function(panel)
        --         panel:showDirectorMessage(214)
        --     end)
        -- else
        --     self:sendShoot()
        -- end
        self.isTouch = false
    end
    mainPanel.btnReEnter.OnClick = function()
   		mainPanel:ChangeDesk() 	
	end

    mainPanel.btnOpenDebug.OnClick = function()
        g_panel_mgr.show("ballsbattle_cc_new.panel.dlg_ballsbattle_debug_panel")
    end
end