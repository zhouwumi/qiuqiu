--一些通用的工具类
BBCCUtils = CreateClass()

function BBCCUtils:__init__()
end

function BBCCUtils.UnPackServerLocation(location)
    return bit.rshift(location,16),bit.band(location,65535)
end

function BBCCUtils.ClientPackLocation(x, y)
    return bit.bor(bit.lshift(x,16),y)
end

--根据先后矩形，获取改变列表(此处先后命名反了)
function BBCCUtils.GetChangeIndexTable(now, before)
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