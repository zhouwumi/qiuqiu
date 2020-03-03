ObjectPool = CreateClass()

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

local PlayerNodeObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_player_object_node').PlayerNodeObject
local PlayerObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_player_object').PlayerObject
local SpikyObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_spiky_object').SpikyObject
local SporeObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_spore_object').SporeObject
local FoodObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_food_object').FoodObject
local BBCommonEffectObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_common_effect_object').BBCommonEffectObject

function ObjectPool:__init__(type, mainPanel)
	self._type = type
	self._mainPanel = mainPanel
	self._allUsingObject = {}
	self._allUsableObject = {}
end

function ObjectPool:SetType(type)
	self._type = type
end

function ObjectPool:GetType()
	return self._type
end

function ObjectPool:Clear()
	self._allUsableObject = {}
	self._allUsingObject = {}
end

function ObjectPool:_createObject(...)
	if self._type == constant_ballsbattle_cc.BBObjectTypes.Player then
		return PlayerObject:New(self._mainPanel, ...)
	elseif self._type == constant_ballsbattle_cc.BBObjectTypes.PlayerNode then
		return PlayerNodeObject:New(self._mainPanel, ...)
	elseif self._type == constant_ballsbattle_cc.BBObjectTypes.Spore then
		return SporeObject:New(self._mainPanel, ...)
	elseif self._type == constant_ballsbattle_cc.BBObjectTypes.Spiky then
		return SpikyObject:New(self._mainPanel, ...)
	elseif self._type == constant_ballsbattle_cc.BBObjectTypes.Food then
		return FoodObject:New(self._mainPanel, ...)
	elseif self._type == constant_ballsbattle_cc.BBObjectTypes.SliderEffect then
		return BBCommonEffectObject:New(self._mainPanel, ...)
	end
end

function ObjectPool:GetOrCreateObect(...)
	local retObject
	if #self._allUsableObject > 0 then
		retObject = table.remove(self._allUsableObject, 1)
	else
		retObject = self:_createObject(...)
	end
	self._allUsingObject[retObject] = true
	return retObject
end

function ObjectPool:ReturnObject(retObject)
	self._allUsingObject[retObject] = false
	self._allUsableObject[#self._allUsableObject + 1] = retObject
end

function ObjectPool:GetUsingObjects()
	return self._allUsingObject
end

function ObjectPool:CreateInitObjectCount(count, ...)
	for index = 1, count do
		local ret = self:_createObject(...)
		if ret.displayObject then
			ret.displayObject:setVisible(false)
		end
		table.insert(self._allUsableObject, ret)
	end
	self._allUsingObject = {}
end