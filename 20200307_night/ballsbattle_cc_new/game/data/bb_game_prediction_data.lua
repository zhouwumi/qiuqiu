BBGameSimplePrediction = CreateClass()

function BBGameSimplePrediction:__init__()
	self.angle = 0
	self.pressure = 0
	self.crc = 0
end

BBGamePredictionData = CreateClass()

function BBGamePredictionData:__init__()
	self.mPredictionRight = true
	self.canSkipSync = true
	self.lastAngle = 0
	self.lastPressure = 0
	self.lastRunGameFrame = 0
	self.predictionCommands = {}
end

function BBGamePredictionData:ClearData()
	self.lastAngle = 0
	self.lastAngle = 0
	self.predictionCommands = {}
end