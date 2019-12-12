BallsBattleCCDataManager = CreateClass()

function BallsBattleCCDataManager:__init__(mainPanel)
	self._mainPanel = mainPanel
end

function BallsBattleCCDataManager:SetData(data)
	self.gameData = data
	self.tickDatas = {}
end

function BallsBattleCCDataManager:OnRestartGame()
end

function BallsBattleCCDataManager:OnGetGameData()
	
end

function BallsBattleCCDataManager:OnUpdate()
end

function BallsBattleCCDataManager:Destory()
end

function BallsBattleCCDataManager:PushTickData(data)
	table.insert(self.tickDatas, data)
end