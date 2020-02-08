BBCCRightUI = CreateClass()

function BBCCRightUI:__init__(mainPanel)
	self._mainPanel = mainPanel
	mainPanel.btnSpawnFood.OnClick = function()
        -- self.isSpawnFood = true
        -- mainPanel.objectManager:GenerateFoodAndSpiky()
    end
    mainPanel.btnSpawnPlayer.OnClick = function()
        mainPanel.gameManager:AddOperatePlayerJoin(mainPanel.currentPlayerIdx)
    end

    mainPanel.btnPlayerSplit.OnClick = function()
        g_net_mgr.get_net('ballsbattlecc'):Request_CMD_SLIME_SPLIT_REQ()
        -- mainPanel.gameManager:AddOperatePlayerSplit(mainPanel.currentPlayerIdx)
    end
    mainPanel.btnPlayerShoot.OnClick = function()
        g_net_mgr.get_net('ballsbattlecc'):Request_CMD_SLIME_SHOOT_REQ()
        -- mainPanel.gameManager:AddOperatePlayerShoot(mainPanel.currentPlayerIdx)
    end
end