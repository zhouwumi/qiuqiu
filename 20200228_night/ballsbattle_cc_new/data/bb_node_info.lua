BBCCNodeInfo = CreateClass()

function BBCCNodeInfo:__init__(serverData)
	if not serverData then
		return
	end
	self.uid = serverData.uid
	self.idx = serverData.idx
	self.mass = serverData.mass
	self.fromId = serverData.fromId
	self.x = serverData.x
	self.y = serverData.y
	self.cd = serverData.cd
	self.protect = serverData.protect
	self.initStopFrame = serverData.initStopFrame
	self.initSpeed = serverData.initSpeed
	self.initDeltaSpeed = serverData.initDeltaSpeed
	self.speedX = serverData.speedX
	self.speedY = serverData.speedY
end