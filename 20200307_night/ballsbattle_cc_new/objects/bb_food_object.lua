local BBBaseObject = import('logic.dialog.ballsbattle_cc_new.objects.bb_base_object').BBBaseObject
FoodObject = CreateClass(BBBaseObject)

local BBCCUtils = import("logic.dialog.ballsbattle_cc_new.ballsbattle_cc_utils").BBCCUtils

function FoodObject:__init__(mainPanel)
	self._mainPanel = mainPanel
	local food = g_uisystem.create_item(mainPanel.foodTemplate)
	self.displayObject = food

	
	food.mask.OnClick = function()
	    food.label:setVisible(not food.label:isVisible())
	end

	self.isDestory = false
	food:setVisible(false)
	self._mainPanel.foodContainer:AddChild(nil, self.displayObject)
end

function FoodObject:OnJoinGame(x, y, key)
	self.displayObject:SetPosition(x, y)
	
	self.displayObject.label:SetString(tostring(x.."_"..y))
	self.initX, self.initY = x, y
	self.positionX, self.positionY = x, y
	self.objectIdx = key
end

function FoodObject:SimulateHide(nodeIdx)
	local node = self._mainPanel.playerManager.allPlayerNodes[nodeIdx]
	self:Fly(node, function()
		self:Hide()
		-- self.displayObject:setVisible(false)
	end)
end

function FoodObject:Remove(nodeIdx)
	self.isDestory = true
	self.displayObject:stopAllActions()
	self._mainPanel.foodManager:RemoveFoodFromMap(self)
	local node = self._mainPanel.playerManager.allPlayerNodes[nodeIdx]
	self:Fly(node, function()
		self:Hide()
		self._mainPanel.foodManager:RemoveFromPool(self)
		-- self.displayObject:removeFromParent()
	end)
end

function FoodObject:Fly(node, callback)
	if node and node.protect and node.protect > 0 and self.displayObject:isVisible() and not node.isDestory then
		local speed = 20
		self.displayObject:stopAllActions()
		self.displayObject:DelayCall(0, function()
			if node.isDestory then
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

function FoodObject:Hide()
	if not self.displayObject:isVisible() then
		return 
	end
	BBBaseObject.Hide(self)
	local actionObj = self.displayObject.bg
	actionObj:stopAllActions()
end

function FoodObject:Show()
	if self.displayObject:isVisible() then
		return 
	end
	BBBaseObject.Show(self)
	local actionObj = self.displayObject.bg
	actionObj:stopAllActions()
	actionObj:DelayCall(math.random() * 2, function() 
	    local opacityRandom = math.random(100,255)
	    actionObj:setOpacity(opacityRandom)
	    local FadeTo = cc.FadeTo:create(1, 0)
	    local FadeTo2 = cc.FadeTo:create(1, opacityRandom)
	    local repeatForever = cc.RepeatForever:create(cc.Sequence:create(FadeTo, FadeTo2))
	    actionObj:runAction(repeatForever)
	end)
	self.displayObject:SetPosition(self.initX, self.initY)
	self.positionX = self.initX
	self.positionY = self.initY
end