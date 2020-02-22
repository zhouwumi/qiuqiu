BBCCServerToClientComponent = CreateClass()

function BBCCServerToClientComponent:__init__(mainPanel)
	self._mainPanel = mainPanel
end

function BBCCServerToClientComponent:SyncFirstData(data)
	self._mainPanel.infoManager:SyncFirstData(data)
end

function BBCCServerToClientComponent:SyncNewPlayers(data)
	self._mainPanel.infoManager:SyncNewPlayers(data)
end

function BBCCServerToClientComponent:NewFoods(foodInfos)
    self._mainPanel.infoManager:NewFoods(foodInfos)
end

function BBCCServerToClientComponent:NewSpores(sporeInfos)
    self._mainPanel.infoManager:NewSpores(sporeInfos)
end

function BBCCServerToClientComponent:NewSpikys(spikyInfos)
    self._mainPanel.infoManager:NewSpikys(spikyInfos)
end

function BBCCServerToClientComponent:SyncFoodEatInfo(foodEatInfos)
    self._mainPanel.infoManager:SyncFoodEatInfo(foodEatInfos)
end

function BBCCServerToClientComponent:SyncSporeEatInfo(sporeEatInfos)
    self._mainPanel.infoManager:SyncSporeEatInfo(sporeEatInfos)
end

function BBCCServerToClientComponent:SyncSpikyEatInfos(spikyEatInfos)
    self._mainPanel.infoManager:SyncSpikyEatInfos(spikyEatInfos)
end

function BBCCServerToClientComponent:SyncEatNode(nodeEatInfos)
    self._mainPanel.infoManager:SyncEatNode(nodeEatInfos)
end

function BBCCServerToClientComponent:UpdateServerAck(serverAcks)
    self._mainPanel.infoManager:UpdateServerAck(serverAcks)
end

function BBCCServerToClientComponent:SyncCommands(frameCommands)
    self._mainPanel.infoManager:SyncCommands(frameCommands)
end

function BBCCServerToClientComponent:SyncPlayerInfos(newPlayerInfos)
    self._mainPanel.infoManager:SyncPlayerInfos(newPlayerInfos)
end