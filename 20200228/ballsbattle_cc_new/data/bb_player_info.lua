BBCCPlayerInfo = CreateClass()

function BBCCPlayerInfo:__init__(serverData)
	if not serverData then
		return
	end
	self.playerId = serverData.uid
	self.playerIdx = serverData.playerIdx
	self.directionX = serverData.directionX
	self.directionY = serverData.directionY
	self.nmass = serverData.nmass
	self.nextNodeIdx = serverData.nextNodeIdx
	self.ackCommand = serverData.ackCommand
	self.finalPointX = serverData.finalPointX
	self.finalPointY = serverData.finalPointY
	self.stopped = serverData.stopped
end

function BBCCPlayerInfo:AddNodeInfo(node)
	if not self.nodes then
		self.nodes = {}
	end
	table.insert(self.nodes, node)
end