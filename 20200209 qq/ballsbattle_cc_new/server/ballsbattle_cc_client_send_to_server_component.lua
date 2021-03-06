BBCCClientSendToServerComponent = CreateClass()

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBCCClientSendToServerComponent:__init__(mainPanel)
	self._mainPanel = mainPanel
	
end

function BBCCClientSendToServerComponent:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, commandIdx, checkSum)
	delay_call(constant_ballsbattle_cc.SimulateDelay / 1000, function()
		if not self._mainPanel:is_valid() then
			return
		end
		self._mainPanel.serverComponent:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, commandIdx, checkSum)
	end)
end