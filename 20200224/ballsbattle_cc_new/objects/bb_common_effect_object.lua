local BBBaseObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_base_object').BBBaseObject
BBCommonEffectObject = CreateClass(BBBaseObject)
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBCommonEffectObject:__init__(mainPanel, type, pool)
	self._mainPanel = mainPanel
	self._type = type
	self._objectPool = pool
	if type == constant_ballsbattle_cc.BBObjectTypes.SliderEffect then
		self.displayObject = g_uisystem.load_template_create('ballsbattle_cc/bb_slider_effect')
		self._mainPanel.bg_container:AddChild(nil, self.displayObject)
		self.displayObject:setVisible(false)
	end
end

function BBCommonEffectObject:Show()

	if self._type == constant_ballsbattle_cc.BBObjectTypes.SliderEffect then
		self.displayObject:setVisible(true)
		self.displayObject.animation:Play(1, 0.03)
		self.displayObject:DelayCall(self.displayObject.animation:GetPlayDuration(), function() 
		    self.displayObject:setVisible(false) 
		    self._objectPool:ReturnObject(self)
		end)
	end
end