local BallsBattleService = import('logic.dialog.ballsbattle.manager.ballsbattle_service')
KillSp = CreateClass()

function KillSp:__init__(sp)
    self.sp = sp
    self.isShow = false
end

function KillSp:update(sp)
    local list = BallsBattleService.get_kill_info_list()
    if #list > 0 and not self.isShow then
        local info = table.remove(list, 1)
        self:showKillMovie(info)
    end
end

function KillSp:showKillMovie(info)
    self.isShow = true
    if self.killSp == nil then
        self.killSp = g_uisystem.load_template_create('ballsbattle/ballsbattl_kill_sp')
        self.sp:addChild(self.killSp)
    else
        self.killSp:setVisible(true)
    end

    for i,player in pairs(info) do
        self.killSp['name'..i]:SetString(tostring(player.nickname))
        local path = 'gui/badam_ui/ballsbattle/live2d/'..player:getPlayerAvator()..'/breathe.png'
        self.killSp['ballsp'..i]:SetPath('', path)
    end
    self.killSp:PlayAnimation('kill')

    self.sp:DelayCall(1, function()
        self.isShow = false
        self.killSp:setVisible(false)

        self.killSp.hongSp:setOpacity(0)
        self.killSp.killSp:setOpacity(0)
        self.killSp.sp2:setPosition(-176,53)
        self.killSp.sp1:setPosition(1474,53)
    end)
end