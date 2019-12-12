local BallsBattleService = import('logic.dialog.ballsbattle.manager.ballsbattle_service')
LeftTimeSp = CreateClass()

function LeftTimeSp:__init__(sp)
    self.label = sp.lefttimeLabel
    self.finishTime = BallsBattleService.get_finish_time()
end

function LeftTimeSp:update()
    local lefttime = self.finishTime - GetServerTimeStamp()
    -- if lefttime > 600 and g_native_conf['debug_control'].balls_debug_mode then
    --     __G__TRACKBACK__('时间戳存在问题'..self.finishTime)
    -- end

    if lefttime > 0 then
        local minute = math.floor(lefttime/60)
        local second = lefttime%60
        self.label:setString(string.format('%01d:%02d', minute, second))
    end
end