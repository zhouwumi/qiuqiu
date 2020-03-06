BBCCClientSendToServerComponent = CreateClass()

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')
local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils

function BBCCClientSendToServerComponent:__init__(mainPanel)
	self._mainPanel = mainPanel
	
end

function BBCCClientSendToServerComponent:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, commandIdx, checkSum)
	local delayTime = constant_ballsbattle_cc.SimulateDelay / 2000
	local time1 = utils_get_tick()
	delay_call(1 / 60, function()
		if not self._mainPanel:is_valid() then
			return
		end
		if utils_get_tick() - time1 > delayTime then
			-- print("客户端发送操作命令成功 ", utils_get_tick() - time1, delayTime)
			self._mainPanel.serverComponent:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, commandIdx, checkSum)
			return
		end
		return 1 / 60
	end)
end

function BBCCClientSendToServerComponent:RequestServerData(uid)
	local delayTime = constant_ballsbattle_cc.SimulateDelay / 2000
	local time1 = utils_get_tick()
	delay_call(1 / 60, function()
		if not self._mainPanel:is_valid() then
			return
		end
		if utils_get_tick() - time1 > delayTime then
			-- print("客户端发送请求数据命令成功 ", utils_get_tick() - time1, delayTime)
		self._mainPanel.serverComponent:RequestData(uid)
			return
		end
		return 1 / 60
	end)
end