BBCCBgComponent = CreateClass()

local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local winSize = g_director:getWinSize()

function BBCCBgComponent:__init__(mainPanel, node)
    self._mainPanel = mainPanel
    
    self.winW, self.winH = winSize.width, winSize.height

    local scaleWidth, scaleHight = winSize.width / constant_ballsbattle.DESIGN_SIZE[1], winSize.height / constant_ballsbattle.DESIGN_SIZE[2]
    self.begainScale = math.min(scaleWidth,scaleHight)
    self.curScale = self.begainScale
    self.targetScale = 1
    self:_updateVisibleRect()
end

function BBCCBgComponent:Update()
	local targetScale, x, y = self:getTargetScale()
	if x ~= nil and self.targetScale ~= targetScale then
	    self.targetScale = targetScale
	    if targetScale > self.curScale then
	        self.delatScale = 1.005
	    else
	        self.delatScale = 0.995
	    end
	end

	if self.delatScale then
	    local newScale = self.curScale * self.delatScale
	    if (self.targetScale >= self.curScale and newScale >= self.targetScale) or
	       (self.targetScale <= self.curScale and newScale <= self.targetScale) then
	        self.delatScale = nil
	        newScale = self.targetScale
	    end
	    local playerObject = self._mainPanel.playerManager:GetMyPlayerObject()
	    if playerObject and playerObject:IsNewBirth() then
	    	self.curScale = self.targetScale
	    else
	    	self.curScale = newScale
	    end
	    
	    self._mainPanel.bg_container:setScale(self.curScale)
	end

	if x ~= nil then
	    local point = cc.p(x / constant_ballsbattle.BG_SIZE[1],y / constant_ballsbattle.BG_SIZE[2])
	    self._mainPanel.bg_container:setAnchorPoint(point)
	    self._mainPanel.bg_container:setPosition(winSize.width/2, winSize.height/2)
	    self:_updateVisibleRect()
	end
end

function BBCCBgComponent:getTargetScale()
	local playerObject = self._mainPanel.playerManager:GetMyPlayerObject()
	if playerObject then
		local startx, starty, endx, endy = playerObject:GetRectCenter()
		--人物死亡，无球状态
		if endx == 0 then
		    return  1
		end
		local width = (endx - startx)
		local hight = (endy - starty)
		local length = math.sqrt(width * width + hight * hight)/2
		-- print(length, self.begainScale, 1/(0.003132 * length + 0.90604) * self.begainScale)
		return 1/(0.003132 * length + 0.90604) * self.begainScale, (startx + endx)/2, (starty + endy)/2
	end
	return 1
end

function BBCCBgComponent:_updateVisibleRect()
	local minLocalPoint = self._mainPanel.bg_container:convertToNodeSpace(ccp(0, 0))
	local maxLocalPoint = self._mainPanel.bg_container:convertToNodeSpace(ccp(self.winW, self.winH))
	local bgW, bgH = self._mainPanel.bg_container:GetContentSize()
	minLocalPoint.x = math.max(minLocalPoint.x, 0)
	minLocalPoint.y = math.max(minLocalPoint.y, 0)
	maxLocalPoint.x = math.min(maxLocalPoint.x, bgW)
	maxLocalPoint.y = math.min(maxLocalPoint.y, bgH)

	self.minLocalPoint = minLocalPoint
	self.maxLocalPoint = maxLocalPoint
end

function BBCCBgComponent:GetVisibleRect()
	return self.minLocalPoint, self.maxLocalPoint
end