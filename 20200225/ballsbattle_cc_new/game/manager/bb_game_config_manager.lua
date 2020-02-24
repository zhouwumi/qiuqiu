BBGameConfigManager = CreateClass()

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')

function BBGameConfigManager:__init__()

end

function BBGameConfigManager:SetClientDebug(isDebug)
	constant_ballsbattle_cc.BBConfigManager.clientDebug = isDebug;
end

function BBGameConfigManager:SetFixLength(cnt)
	constant_ballsbattle_cc.BBConfigManager.fixLength = cnt;
end

function BBGameConfigManager:SetSpikyTime(cnt)
	constant_ballsbattle_cc.BBConfigManager.spikyTime = cnt;
end

function BBGameConfigManager:SetGameWH(w, h)
	constant_ballsbattle_cc.BBConfigManager.gameWidth = w;
	constant_ballsbattle_cc.BBConfigManager.gameHeight = h
end

function BBGameConfigManager:SetMaxBallCount(cnt)
	constant_ballsbattle_cc.BBConfigManager.maxBallCount = cnt;
end

function BBGameConfigManager:SetFoodCount(cnt)
	constant_ballsbattle_cc.BBConfigManager.foodCount = cnt;
end

function BBGameConfigManager:SetFoodMass(mass)
	constant_ballsbattle_cc.BBConfigManager.foodMass = mass;
end

function BBGameConfigManager:SetSpikyCount(cnt)
	constant_ballsbattle_cc.BBConfigManager.spikyCount = cnt;
end

function BBGameConfigManager:SetInitBallMass(mass)
	constant_ballsbattle_cc.BBConfigManager.initBallMass = mass;
end

function BBGameConfigManager:SetMinSpikyMass(mass)
	constant_ballsbattle_cc.BBConfigManager.minSpikyMass = mass;
end

function BBGameConfigManager:SetMaxSpikyMass(mass)
	constant_ballsbattle_cc.BBConfigManager.maxSpikyMass = mass;
end

function BBGameConfigManager:SetMaxSpikySplitChildCount(cnt)
	constant_ballsbattle_cc.BBConfigManager.maxSpikySplitChildCount = cnt;
end

function BBGameConfigManager:SetMinShootBallMass(mass)
	constant_ballsbattle_cc.BBConfigManager.minShootBallMass = mass;
end

function BBGameConfigManager:SetSporeMass(mass)
	constant_ballsbattle_cc.BBConfigManager.sporeMass = mass;
end

function BBGameConfigManager:SetSporeStopFrame(frame)
	constant_ballsbattle_cc.BBConfigManager.sporeStopFrame = frame;
end

function BBGameConfigManager:SetSporeInitSpeed(speed)
	constant_ballsbattle_cc.BBConfigManager.sporeInitSpeed = speed;
end

function BBGameConfigManager:SetSporeCdTime(time)
	constant_ballsbattle_cc.BBConfigManager.sporeCdTime = time;
end

function BBGameConfigManager:SetSplitStopFrame(frame)
	constant_ballsbattle_cc.BBConfigManager.splitStopFrame = frame;
end

function BBGameConfigManager:SetMinSplitMass(mass)
	constant_ballsbattle_cc.BBConfigManager.minSplitMass = mass;
end

function BBGameConfigManager:SetSplitInitSpeed(speed)
	constant_ballsbattle_cc.BBConfigManager.splitInitSpeed = speed;
end

function BBGameConfigManager:SetBallMoveRate(rate)
	constant_ballsbattle_cc.BBConfigManager.ballMoveRate = rate;
end

function BBGameConfigManager:SetBallEatRate(rate)
	constant_ballsbattle_cc.BBConfigManager.ballEatRate = rate;
end

function BBGameConfigManager:SetLogState(open)
	constant_ballsbattle_cc.BBConfigManager.isOpenLog = open;
end

function BBGameConfigManager:SetMaxPredictionCnt(cnt)
	constant_ballsbattle_cc.BBConfigManager.maxPredictionCnt = cnt;
end

function BBGameConfigManager:SetMaxOtherStepCommandCnt(cnt)
	constant_ballsbattle_cc.BBConfigManager.maxOtherStepCommandCnt = cnt;
end

function BBGameConfigManager:SetMaxCommandNoAck(cnt)
	constant_ballsbattle_cc.BBConfigManager.maxCommandNoAck = cnt;
end

function BBGameConfigManager:SetMinCatchUpTicks(cnt)
	constant_ballsbattle_cc.BBConfigManager.minCatchUpTicks = cnt;
end

function BBGameConfigManager:SetMaxCatchUpTicks(cnt)
	constant_ballsbattle_cc.BBConfigManager.maxCatchUpTicks = cnt;
end

function BBGameConfigManager:SetProtectTime(cnt)
	constant_ballsbattle_cc.BBConfigManager.protectTime = cnt;
end

function BBGameConfigManager:SetProtectGripRadius(radius)
	constant_ballsbattle_cc.BBConfigManager.protectGripRadius = radius;
end

function BBGameConfigManager:SetMergeCdTime(cnt)
	constant_ballsbattle_cc.BBConfigManager.mergeCdTime = cnt;
end

function BBGameConfigManager:SetDebugMyBallMass(cnt)
	constant_ballsbattle_cc.BBConfigManager.debugMyBallMass = cnt;
end

function BBGameConfigManager:SetDebugOtherBallMas(cnt)
	constant_ballsbattle_cc.BBConfigManager.debugOtherBallMass = cnt;
end

function BBGameConfigManager:SetOtherTooSlowTicks(cnt)
	constant_ballsbattle_cc.BBConfigManager.otherTooSlowTicks = cnt;
end