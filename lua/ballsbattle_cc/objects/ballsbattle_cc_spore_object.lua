-- local Service = import('logic.dialog.ballsbattle.manager.ballsbattle_service')
local Move_Object,Super = import('dialog/ballsbattle_cc/component/ballsbattle_cc_move_object').Move_Object
Spore,Super = CreateClass(Move_Object)
local frame = 20

function Spore:_initInfo(info)
    Super._initInfo(self,info)
    self.cellKey = "spore"
end

function Spore:canBeEat()
    return self.changeSpeedCD < frame and not self.beEat
end

function Spore:simulateBeEat()
    self.ballSp:setVisible(false)
    self.beEat = true
end

function Spore:move()
    local isMove = Super.move(self)
    if isMove then
        -- Service.get_grid_manager():updateOjectPosition(self)
    end
    return isMove 
end


function Spore:show()
    self.isShow = true
    if not self.beEat then
        self.ballSp:setVisible(true)
    end
end