BBCCGridManager = CreateClass()

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils
local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local winSize = g_director:getWinSize()
local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBCCGridManager:__init__(mainPanel)
	self._mainPanel = mainPanel

	self.gridSize = 450 --大格子的w
	self.gridCount = constant_ballsbattle.BG_SIZE[1] / self.gridSize --格子的横竖数量
	self:ReEnter()
end

function BBCCGridManager:ReEnter()
	self.nowRect = {}
	self.oldRect = nil
end

function BBCCGridManager:OnUpdateGridVisible(force)
	if not force and self.updateTime and utils_get_tick() - self.updateTime <= 0.1 then
		return
	end
	self.updateTime = utils_get_tick()
	local minLocalPoint, maxLocalPoint = self._mainPanel.bgComponent:GetVisibleRect()
	local minGridX = math.ceil(minLocalPoint.x / self.gridSize)
	minGridX = math.max(minGridX, 1)
	local maxGridX = math.ceil(maxLocalPoint.x  / self.gridSize)
	maxGridX = math.min(maxGridX, self.gridCount)

	local minGridY = math.ceil(minLocalPoint.y / self.gridSize)
	minGridY = math.max(minGridY, 1)
	local maxGridY = math.ceil(maxLocalPoint.y / self.gridSize)
	maxGridY = math.min(maxGridY, self.gridCount)

	self.nowRect[1] = minGridX
	self.nowRect[2] = maxGridX
	self.nowRect[3] = minGridY
	self.nowRect[4] = maxGridY


	local removeList, addList
	if not self.oldRect then
		addList  = {}
		for i = minGridX, maxGridX do
			for j = minGridY, maxGridY do
				table.insert(addList,i)
				table.insert(addList,j)
			end
		end
		self.oldRect = {minGridX, maxGridX, minGridY, maxGridY}
	else
		removeList, addList = BBCCUtils.GetChangeIndexTableVersion2(self.nowRect, self.oldRect, self.gridSize)
		-- if removeList then
			-- print(self.nowRect, self.oldRect, removeList, minLocalPoint, maxLocalPoint)
		-- end
		-- print(self.nowRect)
		self.oldRect[1] = minGridX
		self.oldRect[2] = maxGridX
		self.oldRect[3] = minGridY
		self.oldRect[4] = maxGridY
	end
	return removeList, addList
end