BBCCServerToClientComponent = CreateClass()

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBCCServerToClientComponent:__init__(mainPanel)
	self._mainPanel = mainPanel
    self._frameCache = {}
end

function BBCCServerToClientComponent:StartNewFrame()
    self._frameCache = {}
end

function BBCCServerToClientComponent:EndNewFrame()
    local oldCache = self._frameCache
    self._frameCache = {}
    local delayTime = constant_ballsbattle_cc.SimulateDelay / 2000
    local time1 = utils_get_tick()
    delay_call(1 / 60, function()
        if not self._mainPanel:is_valid() then
            return
        end
        if utils_get_tick() - time1 > delayTime then
            for _, callback in ipairs(oldCache or {}) do
                callback()
            end
            return
        end
        return 1 / 60
    end)
end

function BBCCServerToClientComponent:_syncCallback(callback)
    -- delay_call(constant_ballsbattle_cc.SimulateDelay / 2 / 1000, function()
    --     if not self._mainPanel:is_valid() then
    --         return
    --     end
    --     callback()
    -- end)
    table.insert(self._frameCache, callback)
    -- callback()
end

function BBCCServerToClientComponent:SyncFirstData(data)
    self:_syncCallback(function()
        self._mainPanel.infoManager:SyncFirstData(data)
    end)
end

function BBCCServerToClientComponent:SyncNewPlayers(data)
    self:_syncCallback(function()
        self._mainPanel.infoManager:SyncNewPlayers(data)
    end)
end

function BBCCServerToClientComponent:NewFoods(foodInfos)
    
    self:_syncCallback(function()
        self._mainPanel.infoManager:NewFoods(foodInfos)
    end)
end

function BBCCServerToClientComponent:NewSpores(sporeInfos)
    
    self:_syncCallback(function()
        self._mainPanel.infoManager:NewSpores(sporeInfos)
    end)
end

function BBCCServerToClientComponent:NewSpikys(spikyInfos)
    
    self:_syncCallback(function()
        self._mainPanel.infoManager:NewSpikys(spikyInfos)
    end)
end

function BBCCServerToClientComponent:SyncFoodEatInfo(foodEatInfos)
    
    self:_syncCallback(function()
        self._mainPanel.infoManager:SyncFoodEatInfo(foodEatInfos)
    end)
end

function BBCCServerToClientComponent:SyncSporeEatInfo(sporeEatInfos)
    
    self:_syncCallback(function()
        self._mainPanel.infoManager:SyncSporeEatInfo(sporeEatInfos)
    end)
end

function BBCCServerToClientComponent:SyncSpikyEatInfos(spikyEatInfos)
    
    self:_syncCallback(function()
        self._mainPanel.infoManager:SyncSpikyEatInfos(spikyEatInfos)
    end)
end

function BBCCServerToClientComponent:SyncEatNode(nodeEatInfos)
    
    self:_syncCallback(function()
        self._mainPanel.infoManager:SyncEatNode(nodeEatInfos)
    end)
end

function BBCCServerToClientComponent:UpdateServerAck(serverAcks)
    
    self:_syncCallback(function()
        self._mainPanel.infoManager:UpdateServerAck(serverAcks)
    end)
end

function BBCCServerToClientComponent:SyncCommands(frameCommands)
    
    self:_syncCallback(function()
        self._mainPanel.infoManager:SyncCommands(frameCommands)
    end)
end

function BBCCServerToClientComponent:SyncPlayerInfos(newPlayerInfos)
    
    self:_syncCallback(function()
        self._mainPanel.infoManager:SyncPlayerInfos(newPlayerInfos)
    end)
end


function BBCCServerToClientComponent:ServerChangeFakeNode(playerNodeId, x, y, mass)
    self:_syncCallback(function()
        self._mainPanel.playerManager:ServerChangeFakeNode(playerNodeId, x, y, mass)
    end)
end