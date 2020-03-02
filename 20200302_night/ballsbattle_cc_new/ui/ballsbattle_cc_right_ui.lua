BBCCRightUI = CreateClass()

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
function BBCCRightUI:__init__(mainPanel)
	self._mainPanel = mainPanel

    mainPanel.btnPlayerSplit.OnClick = function()
        local moveManager = self._mainPanel:GetMoveManager()
        moveManager:Split()
    end
    mainPanel.btnPlayerShoot.OnClick = function()
        local moveManager = self._mainPanel:GetMoveManager()
        moveManager:Shoot()
    end
    mainPanel.btnReEnter.OnClick = function()
   		mainPanel:ChangeDesk() 	
	end
end