DirectorSp = CreateClass()
local state_open = 1
local state_close = 2
local add_interval = 30
local show_interval = 90
local desInfoConfig = g_conf_mgr.get_conf("ballsbattle_director_info")
local winSize = g_director:getWinSize()

function DirectorSp:__init__(sp)
    self.sp = sp
    self.count = 150
    self.sp.directoBtn.OnClick = function()
        self:_switchState()
    end
    self.messageList = {}
    self.cacheInfo = g_conf_mgr.get_native_conf('ballsbattle_director_info')
    self.state = self.cacheInfo.state or 1
    if self.state == state_close then
        self.sp.directoBtn:SetCheck(true)
    end
    self.isHigh = true
    self.showCount = show_interval

    self:addSpeicalMessage(200)
end

function DirectorSp:update()
    self.count = self.count - 1
    self.showCount = self.showCount + 1
    self:_showMsg()

    if self.count < 0 then
        self:addNormalMessage()
    end
end

function DirectorSp:checkId(messageId)
    --因为luaext_json_encode，这里保持一致，都是用string为key
    messageId = tostring(messageId)
    if self.cacheInfo[messageId] == nil then
        self.cacheInfo[messageId] = 0
    end

    local desInfo = desInfoConfig[tonumber(messageId)]
    if self.cacheInfo[messageId] >= desInfo.times then
        return nil
    elseif desInfo.times ~= 0 then
        self.cacheInfo[messageId] = self.cacheInfo[messageId] + 1
    end
    g_conf_mgr.set_native_conf_k_v('ballsbattle_director_info', messageId, self.cacheInfo[messageId])
    return messageId
end

function DirectorSp:addSpeicalMessage(messageId)
    if self.state ~= state_open and self.showCount < add_interval then
        return
    end
    messageId = self:checkId(messageId)
    if messageId then
        if #self.messageList > 0 then
            --一半的几率顶掉之前的
            if math.random() > 0.5 then
                local deleteMessageId = table.remove(self.messageList)
                if self.cacheInfo[deleteMessageId] ~= nil then
                    self.cacheInfo[deleteMessageId] = self.cacheInfo[deleteMessageId] - 1
                end
                table.insert(self.messageList,messageId)
            end
        else
            table.insert(self.messageList,messageId)
        end
    end
end

function DirectorSp:addNormalMessage()
    if self.state ~= state_open then
        return
    end
    if #desInfoConfig == 0 then
        return
    end
    local messageId
    while(messageId == nil)do
        messageId = self:checkId(math.random(1,#desInfoConfig))
    end
    table.insert(self.messageList,messageId)
end

function DirectorSp:_switchState()
    if self.state == state_open then
        self.messageList = {}
        -- self.sp:removeAllChildren()
        self.state = state_close
    else
        self.state = state_open
        self.count = 60
    end
    self.cacheInfo.state = self.state
    g_conf_mgr.set_native_conf_k_v('ballsbattle_director_info', 'state', self.state)
end

function DirectorSp:_showMsg()
    if #self.messageList > 0 and self.showCount > show_interval then
        self.showCount = 0
        local messageId = table.remove(self.messageList, 1)
        local message = desInfoConfig[tonumber(messageId)][g_conf_mgr.get_native_conf('cur_multilang_index')]
        local messageSp = g_uisystem.load_template_create('ballsbattle/ballsbattl_director_sp')

        self.sp:addChild(messageSp)
        local positionY
        if self.isHigh then
            positionY = 50
        else
            positionY = 120
        end

        self.isHigh = not self.isHigh

        messageSp.mesaageLabel:setString(message)

        local labelW = messageSp.mesaageLabel:GetContentSize() + 80
        messageSp:SetPosition(winSize.width + labelW, positionY)
        messageSp.bg:SetContentSize(labelW, 33)


        local moveTo = cc.MoveTo:create(12, cc.p(labelW * -1,positionY))
        local callFunc = cc.CallFunc:create(function() messageSp:removeFromParent() end)
        messageSp:runAction(cc.Sequence:create(moveTo, callFunc))

        self.count = math.random(17,20) * 30
    end
end