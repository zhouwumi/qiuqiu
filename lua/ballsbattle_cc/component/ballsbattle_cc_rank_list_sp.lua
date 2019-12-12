local BallsBattleService = import('logic.dialog.ballsbattle.manager.ballsbattle_service')
RankList = CreateClass()

function RankList:__init__(sp)
    self.playerList = sp.playerList
    self.myInfo = sp.myInfo
    local color = cc.c4b(253,253,83,255)
    self.myInfo.rank:setTextColor(color)
    self.myInfo.name:setTextColor(color)

    self:update()
end

function RankList:update()
    local list = BallsBattleService.get_mass_sort_player_list()
    local num = math.min(#list,9)
    self.playerList:SetInitCount(num)

    for i=1, #list do
        if i <= num then
            item = self.playerList:GetItem(i)
            self:setItem(item, list[i], i)
        end
        --设置自己的信息
        if list[i].uid == g_user_info.get_user_info().uid then
            self:setItem(self.myInfo, list[i], i)
        end
    end
end

function RankList:setItem(item, playerInfo, index)
    local nickName = playerInfo.nickname
    -- if string.utf8len(nickName) > 10 then
    --     nickName = string.utf8_sub(nickName, 0, 10)
    --     nickName = nickName .. '...'
    -- end
    item.name:setString(nickName)
    local textRank
    if index > 3 then
        textRank = str(index)
    else
        textRank = '#C[image]gui/badam_ui/ballsbattle/n'..index..'.png*1#n'
    end
    item.rank:setString(textRank)
end