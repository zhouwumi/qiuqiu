BBGameClzManager = CreateClass()

local ObjectPool = import('logic.dialog.ballsbattle_cc_new.bb_object_pool').ObjectPool
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBGameClzManager:__init__()
	self.predictionPool = ObjectPool:New(constant_ballsbattle_cc.BBClassTypes.BBGameSimplePrediction)
	self.predictionPool:CreateInitObjectCount(100)

	self.playerNodePool = ObjectPool:New(constant_ballsbattle_cc.BBClassTypes.BBGamePlayerNodeObject)
	self.playerNodePool:CreateInitObjectCount(100)
end

function BBGameClzManager:GetSimplePrediction()
	return self.predictionPool:GetOrCreateObect()
end

function BBGameClzManager:ReturnSimplePrediction(prediction)
	self.predictionPool:ReturnObject(prediction)
end

function BBGameClzManager:GetPlayerNode()
	return self.playerNodePool:GetOrCreateObect()
end

function BBGameClzManager:ReturnPlayerNode(node)
	self.playerNodePool:ReturnObject(node)
end