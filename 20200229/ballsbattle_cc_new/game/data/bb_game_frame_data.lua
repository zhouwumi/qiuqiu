BBGameBallDeltaData = CreateClass()

function BBGameBallDeltaData:__init__()
	self.wrapTicks = 0
	self.location = {x = 0, y = 0}
	self.wrapLocationOffset = {x = 0, y = 0}
	self.fixedLocation = {x = 0, y = 0}
end