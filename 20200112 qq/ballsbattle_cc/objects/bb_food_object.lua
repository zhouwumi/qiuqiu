FoodObject = CreateClass()

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
	local idx = self._bbGameManager:GetFoodIdxByPos(key)
	self.displayObject.label:SetString(tostring(idx))
	self.positionX, self.positionY = x, y
	self.objectIdx = idx
	-- print('食物更新位置', x, y)
end

function FoodObject:Remove()
	self.isDestory = true
	self.displayObject:removeFromParent()
	self._mainPanel.foodManager:RemoveFood(self)
end
