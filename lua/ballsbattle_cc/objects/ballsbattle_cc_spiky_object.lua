local Object,Super = import('ballsbattle_cc_object').Object
local Live2DSprite = import('dialog/ballsbattle_cc/component/ballsbattle_cc_live_2d').Live2DSprite

Spiky_Object,Super = CreateClass(Object)

function Spiky_Object:_initView()
    local spikySp = Live2DSprite:New('spiky')
    self.ballSp = spikySp.sp
    self._spriteWidth = 50
    if cc.Live2DSprite then
        self._spriteWidth = self._spriteWidth * 0.85
    end
end

-- function Spiky_Object:_initInfo(info)
--     Super._initInfo(self,info)

--     if self.label == nil then
--         label = cc.RichLabel:Create(str(self.idx), 25)
--         self.ballSp:addChild(label)
--         label:SetPosition(25,25)
--         self.label = label
--     else
--         label:SetString(str(self.idx))
--     end
-- end