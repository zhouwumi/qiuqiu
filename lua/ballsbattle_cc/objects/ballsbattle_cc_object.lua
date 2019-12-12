Object = CreateClass()
local constant_ballsbattle = g_conf_mgr.get_constant('constant_ballsbattle')
local ballsUtils = import('logic_data.game_ballsbattle.ballsbattle_utils')
local winSize = g_director:getWinSize()
local sporePng = 'ball_food/spore.png'
local Ball_Interpolation = import('ballsbattle_cc_ball_interpolation').Ball_Interpolation

function Object:__init__(info)
    -- self.cellList = {}
    self:_initView()
    self.ballInterpolation = Ball_Interpolation:New(self)
    self.cellKey = "Object"
    self:_initInfo(info)
end

function Object:_initInfo(info)
    self.lastPositionX, self.lastPositionY = ballsUtils.get_positon_by_locaiton(info.location)
    self.idx = info.idx
    self.mass = info.mass or 1
    self:caculateRadius()
    self:changeScale()
    self:changeZOrder()
    self.ballInterpolation:initPosition()
end

function Object:_initView()
    self.ballSp = cc.Sprite:Create(constant_ballsbattle.SLIME_FOOD_PLIST,sporePng)
    self._spriteWidth = self.ballSp:GetContentSize()/2
end

function Object:addToParent(container)
    container:AddChild(nil, self.ballSp)
    -- BallsBattleService.get_grid_manager():addObject(self)
end

function Object:caculateRadius()
    self.radius = 4 + math.sqrt(self.mass) * 6
end

function Object:dismiss()
    self:removeAllCell()
    self.rectTable = nil
    self.ballSp:setVisible(false)
    self.canUse = true
end


function Object:removeAllCell()
       --从场景网格中删除    
    -- for i=#self.cellList,1,-1 do
    --     local info = self.cellList[i]
    --     self:removeCell(info)
    -- end
end

function Object:changeScale()
    self.ballSp:setScale(self.radius / self._spriteWidth)
end

function Object:simulateBeEat()
    
end

function Object:changeZOrder()
    self.ballSp:setLocalZOrder(self.mass + 100)
end

function Object:updatePosition(dtPercent)
   --做插值运算
    self.ballInterpolation:setPosition(self.lastPositionX, self.lastPositionY, dtPercent)
end

function Object:reset(info)
    self.canUse = false
    self:_initInfo(info)
    -- BallsBattleService.get_grid_manager():addObject(self)
end

function Object:getDisplayPosition()
   return self.lastPositionX, self.lastPositionY
end

function Object:show()
    self.isShow = true
    self.ballSp:setVisible(true)
end

function Object:hide()
    if self.uid ~= g_user_info.get_user_info().uid  then
        self.isShow = false
        self.ballSp:setVisible(false)
    end
end

function Object:addCell(newCell)
    -- for i,cell in pairs(self.cellList) do
    --     if cell == newCell then
    --         return
    --     end
    -- end
    -- table.insert(self.cellList,newCell)
    -- if newCell[2][self.cellKey] == nil then
    --     newCell[2][self.cellKey] = {}
    -- end
    -- table.insert(newCell[2][self.cellKey],self)
end

function Object:removeCell(cell)
    -- for i,v in pairs(self.cellList) do
    --     if cell == v then
    --         table.remove(self.cellList,i)
    --         for j,curObject in pairs(cell[2][self.cellKey]) do
    --             if curObject == self then
    --                 table.remove(cell[2][self.cellKey],j)
    --                 return
    --             end
    --         end
    --     end
    -- end
end

function Object:checkVisible()
    -- for i,cell in pairs(self.cellList) do
    --     if cell[1] then
    --         self:show()
    --         return
    --     end
    -- end
    -- self:hide()
end