Panel = g_panel_mgr.new_panel_class('ballsbattle_cc/ballsbattle_cc_debug_panel')

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function Panel:init_panel()
	self.btnClose.OnClick = function()
		self:close_panel()
	end

	self.ballsBattleNativeConf = g_conf_mgr.get_native_conf('ballsbattle_cc_native_data')
	self:_setCheckAndKey(self.checkUseLua, 'useLua')
	self:_setCheckAndKey(self.checkMeNetWorkShake, "isMeNetWorkShake")
	self:_setCheckAndKey(self.checkOtherNetWorkShake, "isOtherNetWorkShake")
	self:_setCheckAndKey(self.checkCloseGC, "closeGc")
	self.editRobotCnt.OnEditReturn = function()
		local value = tonumber(self.editRobotCnt:GetString())
		if value  then
			value = math.max(value, 0)
			self.ballsBattleNativeConf.robotCnt = value
		end
	end
	self.editRobotCnt:SetString(tostring(self.ballsBattleNativeConf.robotCnt))
end


function Panel:_setCheckAndKey(checkBtn, key, callback)
	checkBtn.OnChecked = function()
		g_conf_mgr.set_native_conf_k_v('ballsbattle_cc_native_data', key, checkBtn:GetCheck())
		if callback then
			callback()
		end
	end
	checkBtn:SetCheck(self.ballsBattleNativeConf[key], false)
end