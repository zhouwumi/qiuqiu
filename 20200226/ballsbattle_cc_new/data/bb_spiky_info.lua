BBCCSpikyInfo = CreateClass()

-- fromId, uid, speedX, speedY, x, y, initSpeed, initDeltaSpeed, initStopFrame, cd

function BBCCSpikyInfo:__init__(serverData)
	if not serverData then
		return
	end
	self.idx = serverData.idx
	self.mass = serverData.mass
	-- self.pos = serverData.pos
	self.x = serverData.x
	self.y = serverData.y
end
