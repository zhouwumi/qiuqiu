--一些通用的工具类
BBCCUtils = CreateClass()

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBCCUtils:__init__()
end

function BBCCUtils.UnPackServerLocation(location)
    return bit.rshift(location,16),bit.band(location,65535)
end

function BBCCUtils.ClientPackLocation(x, y)
    return bit.bor(bit.lshift(x,16),y)
end

function BBCCUtils.get_radian_by_vector(deltaX, deltaY)
    local radian =  math.atan2(math.abs(deltaY),math.abs(deltaX))
    if  deltaX >= 0 and deltaY <= 0 then 
        radian = 2 * math.pi - radian
    elseif deltaX <= 0 and deltaY <= 0 then
        radian = math.pi + radian
    elseif deltaX <= 0 and deltaY >= 0 then
        radian = math.pi - radian
    end
    return radian
end

local massName = {'mg','g','kg','t'}
function BBCCUtils.get_mass_displayname(mass)
    local displayMass = 10 * math.pow(mass/10,6)
    local desName = massName[#massName]
    for index,name in ipairs(massName) do
        if displayMass < 1000 or index == #massName then
            desName = name
            break
        else   
            displayMass = displayMass/1000
        end 
    end
    if displayMass < 1000 then
        displayMass = string.format('%.2f', displayMass)
    else
        displayMass = math.floor(displayMass)
    end
    return displayMass.." "..desName
end

--根据先后矩形，获取改变列表(此处先后命名反了)
function BBCCUtils.GetChangeIndexTable(now, before)
    if now[1] == before[1] and now[2] == before[2] and now[3] == before[3] and now[4] == before[4] then
        return
    end
    local removeList = {}
    local addList = {}
    if now[1] < before[1] then
        for i = now[1],before[1] - 1 do
            for j = now[3],now[4] do
                table.insert(addList,i)
                table.insert(addList,j)
            end
        end
    elseif now[1] > before[1] then
        for i = before[1], now[1] - 1 do
            for j = before[3] ,before[4] do
                table.insert(removeList,i)
                table.insert(removeList,j)
            end
        end
    end

    if now[3] < before[3] then
        for i = now[1],now[2] do
            for j = now[3],before[3] - 1 do
                table.insert(addList,i)
                table.insert(addList,j)
            end
        end
    elseif now[3] > before[3] then
        for i = before[1] ,before[2] do
            for j = before[3], now[3] - 1 do
                table.insert(removeList,i)
                table.insert(removeList,j)
            end
        end
    end

    if now[2] < before[2] then
        for i = now[2] + 1,before[2] do
            for j = before[3],before[4] do
                table.insert(removeList,i)
                table.insert(removeList,j)
            end
        end
    elseif now[2] > before[2] then
        for i = before[2] + 1, now[2] do
            for j = now[3] ,now[4] do
                table.insert(addList,i)
                table.insert(addList,j)
            end
        end
    end

    if now[4] < before[4] then
        for i = before[1],before[2] do
            for j = now[4]+ 1,before[4] do
                table.insert(removeList,i)
                table.insert(removeList,j)
            end
        end
    elseif now[4] > before[4] then
        for i = now[1], now[2] do
            for j = before[4] + 1, now[4] do
                table.insert(addList,i)
                table.insert(addList,j)
            end
        end
    end
    return removeList,addList
end

local Temp_Interact_Rect = {0, 0, 0, 0}
function BBCCUtils.GetChangeIndexTableVersion2(now, before)
    if now[1] == before[1] and now[2] == before[2] and now[3] == before[3] and now[4] == before[4] then
        return
    end
    local minInteractX = math.max(now[1], before[1])
    local maxInteractX = math.min(now[2], before[2])
    local minInteractY = math.max(now[3], before[3])
    local maxInteractY = math.min(now[4], before[4])
    local addList = nil
    local removeList = nil

    --现在的完全在以前的内部
    if now[1] == minInteractX and now[2] == maxInteractX and now[3] == minInteractY and now[4] == maxInteractY then
        addList = nil
        removeList = BBCCUtils.GetAroundDiff(before, now)
    --以前的完全在现在的内部
    elseif before[1] == minInteractX and before[2] == maxInteractX and before[3] == minInteractY and before[4] == maxInteractY then
        removeList = nil
        addList = BBCCUtils.GetAroundDiff(now, before)
    else --有加有减
        Temp_Interact_Rect[1] = minInteractX
        Temp_Interact_Rect[2] = maxInteractX
        Temp_Interact_Rect[3] = minInteractY
        Temp_Interact_Rect[4] = maxInteractY

        removeList = BBCCUtils.GetAroundDiff(before, Temp_Interact_Rect)
        addList = BBCCUtils.GetAroundDiff(now, Temp_Interact_Rect)
    end
     
    return removeList, addList
    --add

end

function BBCCUtils.GetAroundDiff(before, innerRect)
    local ret = nil
    --remove
    --left
    for i = before[1], innerRect[1] - 1 do
        for j = before[3], before[4] do
            if ret == nil then
                ret = {}
            end
            table.insert(ret, i)
            table.insert(ret, j)
        end
    end

    --right
    for i = innerRect[2] + 1, before[2] do
        for j = before[3], before[4] do
            if ret == nil then
                ret = {}
            end
            table.insert(ret, i)
            table.insert(ret, j)
        end 
    end

    --top
    for j = innerRect[4] + 1, before[4]  do
        for i = innerRect[1], innerRect[2] do
            if ret == nil then
                ret = {}
            end
            table.insert(ret, i)
            table.insert(ret, j)
        end
    end 
    
    --down
    for j = before[3], innerRect[3] - 1  do
        for i = innerRect[1], innerRect[2] do
            if ret == nil then
                ret = {}
            end
            table.insert(ret, i)
            table.insert(ret, j)
        end
    end
    return ret
end