local BBBaseObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_base_object').BBBaseObject
FoodObject = CreateClass(BBBaseObject)

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils

function FoodObject:__init__(mainPanel)
	self._mainPanel = mainPanel
	self._bbGameManager = mainPanel.gameManager
	local food = g_uisystem.create_item(mainPanel.foodTemplate)
	self.displayObject = food

	food.bg:DelayCall(math.random() * 2, function() 
	    local opacityRandom = math.random(100,255)
	    food.bg:setOpacity(opacityRandom)
	    local FadeTo = cc.FadeTo:create(1, 0)
	    local FadeTo2 = cc.FadeTo:create(1, opacityRandom)
	    local repeatForever = cc.RepeatForever:create(cc.Sequence:create(FadeTo, FadeTo2))
	    food.bg:runAction(repeatForever)
	end)
	food.mask.OnClick = function()
	    food.label:setVisible(not food.label:isVisible())
	end

	self.isDestory = false
	food:setVisible(false)
end

function FoodObject:SetPosition(x, y, key)
	self.displayObject:SetPosition(x, y)
	self._mainPanel.foodContainer:AddChild(nil, self.displayObject)
	self.displayObject.label:SetString(tostring(x.."_"..y))
	self.initX, self.initY = x, y
	self.positionX, self.positionY = x, y
	self.objectIdx = key

	--这种每个节点一个delayCall很卡
	-- local times = 0
	-- self.displayObject:DelayCall(1, function()
	-- 	local isInRect = self._mainPanel.foodManager:_isFoodInVisibleRect(self)
	-- 	if isInRect ~= self.displayObject:isVisible() then
	-- 		times = times + 1
	-- 		if times > 5 then
	-- 			times = 0
	-- 			print(x, y, self._mainPanel.foodManager.nowRect, self.objectIdx, self.gridX, self.gridY)
	-- 			__G__TRACKBACK__('ddd')
	-- 		end
	-- 	end
	-- 	return 1
	-- end)
	-- print('食物更新位置', x, y)
end

function FoodObject:SimulateHide(nodeIdx)
	local node = self._mainPanel.playerManager.allPlayerNodes[nodeIdx]
	self:Fly(node, function()
		self.displayObject:setVisible(false)
	end)
end

function FoodObject:MoveInScreen()
	self.displayObject:setVisible(true)
	self.displayObject:SetPosition(self.initX, self.initY)
	self.positionX = self.initX
	self.positionY = self.initY
end

function FoodObject:Remove(nodeIdx)
	self.isDestory = true
	self._mainPanel.foodManager:RemoveFood(self)
	local node = self._mainPanel.playerManager.allPlayerNodes[nodeIdx]
	self:Fly(node, function()
		self.displayObject:removeFromParent()
	end)
end

function FoodObject:Fly(node, callback)
	if node and node.protect and node.protect > 0 and self.displayObject:isVisible() then
		local speed = 20
		self.displayObject:stopAllActions()
		self.displayObject:DelayCall(0, function()
			if not node:IsValid() then
				if callback then
					callback()
				end
			else
				local x, y = node:GetDisplayPosition()
				local curX, curY = self:GetDisplayPosition()
	            local distancex = x - curX
	            local distancey = y - curY
	            local distancePow = math.pow(distancex,2) + math.pow(distancey,2)
	            local distance = math.sqrt(distancePow)
	            if distancePow > math.pow(node.radius, 2) then
	            	local rateX = distancex / distance
	            	local rateY = distancey / distance
	                curX = curX + rateX * speed
	                curY = curY + rateY * speed
	                self.displayObject:SetPosition(curX, curY)
	                speed = speed + 1

	                return 0.01
	            else
	            	if callback then
	            		callback()
	            	end
	            end
			end
		end)
	else
		if callback then
			callback()
		end
	end
end