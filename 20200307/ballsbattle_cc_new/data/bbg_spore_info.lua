BBCCSporeInfo = CreateClass()

-- fromId, uid, speedX, speedY, x, y, initSpeed, initDeltaSpeed, initStopFrame, cd

function BBCCSporeInfo:__init__(serverData)
	if not serverData then
		return
	end
	self.idx = serverData.idx
	self.uid = serverData.uid
	self.fromId = serverData.fromId
	self.speedX = serverData.speedX
	self.speedY = serverData.speedY
	self.x = serverData.serverData.nmass
	self.y = serverData.serverData.nextNodeIdx
	self.initSpeed = serverData.initSpeed
	self.initDeltaSpeed = serverData.initDeltaSpeed
	self.initStopFrame = serverData.initStopFrame
	self.cd = serverData.cd
end
