BBGameMathUtils = CreateClass()

local constant_ballsbattle_cc = g_conf_mgr.get_constant('constant_ballsbattle_cc')
local BBGameRect = import('logic.dialog.ballsbattle_cc_new.game.base.bb_game_rect').BBGameRect

function BBGameMathUtils.__init__()
end

--右为0
function BBGameMathUtils.GetAngle(x, y)
	local angle = 0
	if y == 0 then
		angle = 0
		if x < 0 then
			angle = 180
		end
	else
		local degree = math.atan(math.abs(y)/math.abs(x))
		angle = degree / math.pi * 180
		if deltaX < 0 then
			angle = 180 - angle
		end
		if deltaY < 0 then
			angle = 360 - angle
		end
	end
	return angle
end

function BBGameMathUtils.GetFixedVetor2(vec, length)
	if length == 0 then
		return 0, 0
	end
	if vec.x == 0 and vec.y == 0 then
		return 0, 0
	end
	local len = math.sqrt(vec.x * vec.x + vec.y * vec.y)
	local rateX = vec.x / len
	local rateY = vec.y / len
	return rateX * length, rateY * length
end

function BBGameMathUtils.GetFixedVetor2ByXY(x, y, length)
	if length == 0 then
		return 0, 0
	end
	if x == 0 and y == 0 then
		return 0, 0
	end
	local len = math.sqrt(x * x + y * y)
	local rateX = x / len
	local rateY = y / len
	return rateX * length, rateY * length
end


function BBGameMathUtils.GetCRC32(nums)
	local tab = constant_ballsbattle_cc.CRC32_TABLE
	local crc = 0xffffffff
	for index = 1, #nums do
		local num = nums[index]
		for i = 1, 4 do
			local num1 = bit.band(bit.rshift(num, (i - 1) * 8), 0xff)
			local v1 = bit.band(bit.bxor(crc, num1), 0xff) + 1
			local v2 = tab[v1]
			local v3 = bit.bxor(v2, bit.rshift(crc, 8))
			crc = v3
		end
	end
	if crc < 0 then
		crc = crc + 0xffffffff + 1
	end
	return crc
end

local SIN_VALUE = {
	0, 0.0175, 0.0349, 0.0523, 0.0698, 0.0872, 0.1045, 0.1219, 0.1392, 0.1564, 
	0.1736, 0.1908, 0.2079, 0.2250, 0.2419, 0.2588, 0.2756, 0.2924, 0.3090, 0.3256, 
	0.3420, 0.3584, 0.3746, 0.3907, 0.4067, 0.4226, 0.4384, 0.4540, 0.4695, 0.4848, 0.5, 0.5150, 0.5299, 0.5446, 0.5592, 0.5736, 0.5878, 0.6018, 0.6157, 0.6293, 0.6428, 0.6561, 0.6691, 0.6820, 0.6947, 0.7071, 0.7193, 0.7314, 0.7431, 0.7547, 0.7660, 0.7771, 0.7880, 0.7986, 0.8090, 0.8192, 0.8290, 0.8387, 0.8480, 0.8572, 0.8660, 0.8746, 0.8829, 0.8910, 0.8988, 0.9063, 0.9135, 0.9205, 0.9272, 0.9336, 0.9397, 0.9455, 0.9511, 0.9563, 0.9613, 0.9659, 0.9703, 0.9744, 0.9781, 0.9816, 0.9848, 0.9877, 0.9903, 0.9925, 0.9945, 0.9962, 0.9976, 0.9986, 0.9994, 0.9998, 1, 0.9998, 0.9994, 0.9986, 0.9976, 0.9962, 0.9945, 0.9925, 0.9903, 0.9877, 0.9848, 0.9816, 0.9781, 0.9744, 0.9703, 0.9659, 0.9613, 0.9563, 0.9511, 0.9455, 0.9397, 0.9336, 0.9272, 0.9205, 0.9135, 0.9063, 0.8988, 0.8910, 0.8829, 0.8746, 0.8660, 0.8572, 0.8480, 0.8387, 0.8290, 0.8192, 0.8090, 0.7986, 0.7880, 0.7771, 0.7660, 0.7547, 0.7431, 0.7314, 0.7193, 0.7071, 0.6947, 0.6820, 0.6691, 0.6561, 0.6428, 0.6293, 0.6157, 0.6018, 0.5878, 0.5736, 0.5592, 0.5446, 0.5299, 0.5150, 0.5, 0.4848, 0.4695, 0.4540, 0.4384, 0.4226, 0.4067, 0.3907, 0.3746, 0.3584, 0.3420, 0.3256, 0.3090, 0.2924, 0.2756, 0.2588, 0.2419, 0.2250, 0.2079, 0.1908, 0.1736, 0.1564, 0.1392, 0.1219, 0.1045, 0.0872, 0.0698, 0.0523, 0.0349, 0.0175, 0, -0.0175, -0.0349, -0.0523, -0.0698, -0.0872, -0.1045, -0.1219, -0.1392, -0.1564, -0.1736, -0.1908, -0.2079, -0.2250, -0.2419, -0.2588, -0.2756, -0.2924, -0.3090, -0.3256, -0.3420, -0.3584, -0.3746, -0.3907, -0.4067, -0.4226, -0.4384, -0.4540, -0.4695, -0.4848, -0.5, -0.5150, -0.5299, -0.5446, -0.5592, -0.5736, -0.5878, -0.6018, -0.6157, -0.6293, -0.6428, -0.6561, -0.6691, -0.6820, -0.6947, -0.7071, -0.7193, -0.7314, -0.7431, -0.7547, -0.7660, -0.7771, -0.7880, -0.7986, -0.8090, -0.8192, -0.8290, -0.8387, -0.8480, -0.8572, -0.8660, -0.8746, -0.8829, -0.8910, -0.8988, -0.9063, -0.9135, -0.9205, -0.9272, -0.9336, -0.9397, -0.9455, -0.9511, -0.9563, -0.9613, -0.9659, -0.9703, -0.9744, -0.9781, -0.9816, -0.9848, -0.9877, -0.9903, -0.9925, -0.9945, -0.9962, -0.9976, -0.9986, -0.9994, -0.9998, -1, -0.9998, -0.9994, -0.9986, -0.9976, -0.9962, -0.9945, -0.9925, -0.9903, -0.9877, -0.9848, -0.9816, -0.9781, -0.9744, -0.9703, -0.9659, -0.9613, -0.9563, -0.9511, -0.9455, -0.9397, -0.9336, -0.9272, -0.9205, -0.9135, -0.9063, -0.8988, -0.8910, -0.8829, -0.8746, -0.8660, -0.8572, -0.8480, -0.8387, -0.8290, -0.8192, -0.8090, -0.7986, -0.7880, -0.7771, -0.7660, -0.7547, -0.7431, -0.7314, -0.7193, -0.7071, -0.6947, -0.6820, -0.6691, -0.6561, -0.6428, -0.6293, -0.6157, -0.6018, -0.5878, -0.5736, -0.5592, -0.5446, -0.5299, -0.5150, -0.5, -0.4848, -0.4695, -0.4540, -0.4384, -0.4226, -0.4067, -0.3907, -0.3746, -0.3584, -0.3420, -0.3256, -0.3090, -0.2924, -0.2756, -0.2588, -0.2419, -0.2250, -0.2079, -0.1908, -0.1736, -0.1564, -0.1392, -0.1219, -0.1045, -0.0872, -0.0698, -0.0523, -0.0349, -0.0175
};

local COS_VALUE = {
	1,0.9998,0.9994,0.9986,0.9976,0.9962,0.9945,0.9925,0.9903,0.9877,0.9848,0.9816,0.9781,0.9744,0.9703,0.9659,0.9613,0.9563,0.9511,0.9455,0.9397,0.9336,0.9272,0.9205,0.9135,0.9063,0.8988,0.8910,0.8829,0.8746,0.8660,0.8572,0.8480,0.8387,0.8290,0.8192,0.8090,0.7986,0.7880,0.7771,0.7660,0.7547,0.7431,0.7314,0.7193,0.7071,0.6947,0.6820,0.6691,0.6561,0.6428,0.6293,0.6157,0.6018,0.5878,0.5736,0.5592,0.5446,0.5299,0.5150,0.5,0.4848,0.4695,0.4540,0.4384,0.4226,0.4067,0.3907,0.3746,0.3584,0.3420,0.3256,0.3090,0.2924,0.2756,0.2588,0.2419,0.2250,0.2079,0.1908,0.1736,0.1564,0.1392,0.1219,0.1045,0.0872,0.0698,0.0523,0.0349,0.0175,0,-0.0175,-0.0349,-0.0523,-0.0698,-0.0872,-0.1045,-0.1219,-0.1392,-0.1564,-0.1736,-0.1908,-0.2079,-0.2250,-0.2419,-0.2588,-0.2756,-0.2924,-0.3090,-0.3256,-0.3420,-0.3584,-0.3746,-0.3907,-0.4067,-0.4226,-0.4384,-0.4540,-0.4695,-0.4848,-0.5,-0.5150,-0.5299,-0.5446,-0.5592,-0.5736,-0.5878,-0.6018,-0.6157,-0.6293,-0.6428,-0.6561,-0.6691,-0.6820,-0.6947,-0.7071,-0.7193,-0.7314,-0.7431,-0.7547,-0.7660,-0.7771,-0.7880,-0.7986,-0.8090,-0.8192,-0.8290,-0.8387,-0.8480,-0.8572,-0.8660,-0.8746,-0.8829,-0.8910,-0.8988,-0.9063,-0.9135,-0.9205,-0.9272,-0.9336,-0.9397,-0.9455,-0.9511,-0.9563,-0.9613,-0.9659,-0.9703,-0.9744,-0.9781,-0.9816,-0.9848,-0.9877,-0.9903,-0.9925,-0.9945,-0.9962,-0.9976,-0.9986,-0.9994,-0.9998,-1,-0.9998,-0.9994,-0.9986,-0.9976,-0.9962,-0.9945,-0.9925,-0.9903,-0.9877,-0.9848,-0.9816,-0.9781,-0.9744,-0.9703,-0.9659,-0.9613,-0.9563,-0.9511,-0.9455,-0.9397,-0.9336,-0.9272,-0.9205,-0.9135,-0.9063,-0.8988,-0.8910,-0.8829,-0.8746,-0.8660,-0.8572,-0.8480,-0.8387,-0.8290,-0.8192,-0.8090,-0.7986,-0.7880,-0.7771,-0.7660,-0.7547,-0.7431,-0.7314,-0.7193,-0.7071,-0.6947,-0.6820,-0.6691,-0.6561,-0.6428,-0.6293,-0.6157,-0.6018,-0.5878,-0.5736,-0.5592,-0.5446,-0.5299,-0.5150,-0.5,-0.4848,-0.4695,-0.4540,-0.4384,-0.4226,-0.4067,-0.3907,-0.3746,-0.3584,-0.3420,-0.3256,-0.3090,-0.2924,-0.2756,-0.2588,-0.2419,-0.2250,-0.2079,-0.1908,-0.1736,-0.1564,-0.1392,-0.1219,-0.1045,-0.0872,-0.0698,-0.0523,-0.0349,-0.0175,0,0.0175,0.0349,0.0523,0.0698,0.0872,0.1045,0.1219,0.1392,0.1564,0.1736,0.1908,0.2079,0.2250,0.2419,0.2588,0.2756,0.2924,0.3090,0.3256,0.3420,0.3584,0.3746,0.3907,0.4067,0.4226,0.4384,0.4540,0.4695,0.4848,0.5,0.5150,0.5299,0.5446,0.5592,0.5736,0.5878,0.6018,0.6157,0.6293,0.6428,0.6561,0.6691,0.6820,0.6947,0.7071,0.7193,0.7314,0.7431,0.7547,0.7660,0.7771,0.7880,0.7986,0.8090,0.8192,0.8290,0.8387,0.8480,0.8572,0.8660,0.8746,0.8829,0.8910,0.8988,0.9063,0.9135,0.9205,0.9272,0.9336,0.9397,0.9455,0.9511,0.9563,0.9613,0.9659,0.9703,0.9744,0.9781,0.9816,0.9848,0.9877,0.9903,0.9925,0.9945,0.9962,0.9976,0.9986,0.9994,0.9998
};

function BBGameMathUtils.AngleToFixedVector(angle, length)
	local realAngle = angle % 360 + 1
	local x = COS_VALUE[realAngle] * length
	local y = SIN_VALUE[realAngle] * length
	return x, y
end

function BBGameMathUtils.FixPoint(rect, point)
	point.x = math.min(point.x, rect.minX)
	point.y = math.min(point.y, rect.minY)
	point.x = math.max(point.x, rect.maxX)
	point.y = math.max(point.y, rect.maxY)
end

function BBGameMathUtils.NeedRollback(node1, node2, delta)
	return node1:GetRadius() <= node2:GetRadius() * (1 + delta) and node2:GetRadius() <= node1:GetRadius() * (1 + delta)
end

function BBGameMathUtils.Mass2Radius(mass)
	return 4 + math.ceil(math.sqrt(mass) * 6)
end

function BBGameMathUtils.Mass2Speed(mass)
	return math.floor(8 * 1.6 * math.sqrt(20 / (mass + 10)))
end

function BBGameMathUtils.FixCircle(rect, circleX, circleY, radius)
	local isFix, fixedX, fixedY
	if circleX + radius > rect.maxX then
		isFix = true
		fixedX = rect.maxX - radius
	elseif circleX - radius < rect.minX then
		isFix = true
		fixedX = rect.minX + radius
	else
		fixedX = circleX
	end

	if circleY + radius > rect.maxY then
		isFix = true
		fixedY = rect.maxY - radius
	elseif circleY - radius < rect.minY then
		isFix = true
		fixedY = rect.minY + radius
	else
		fixedY = circleY
	end
	return isFix, fixedX, fixedY
end

local defaultRect = BBGameRect:New(0, 0, 4500, 4500)
function BBGameMathUtils.FixCircleDefaultRect(circleX, circleY, radius)
	return BBGameMathUtils.FixCircle(defaultRect, circleX, circleY, radius)
end

function BBGameMathUtils.CheckCircleHit(node1, circleX, circleY, radius)
	local deltaX = node1.location.x - circleX
	local deltaY = node1.location.y - circleY
	local totalCircle = node1:GetRadius() + radius
	return deltaX * deltaX + deltaY * deltaY < totalCircle * totalCircle
end

function BBGameMathUtils.bb_fix_float(value)
	return math.floor(value * 1000) / 1000
end

function BBGameMathUtils.bb_fix_bb_vector(vec)
	vec.x = BBGameMathUtils.bb_fix_float(vec.x)
	vec.y = BBGameMathUtils.bb_fix_float(vec.y)
end

function BBGameMathUtils.bb_float_to_int(value)
	return math.floor(value * 1000)
end

function BBGameMathUtils.bb_int_to_float(value)
	return value / 1000
end

function BBGameMathUtils.PressureToPercent(pressure)
	return pressure / 10
end

function BBGameMathUtils.bb_get_crc_best_value(value)
	local floorValue = math.round_number(value)
	local ret = math.floor(floorValue / 5) * 5
	-- if value - floorValue > 0.99 then
	-- 	print("细小的差别检测到了.... ", value)
	-- 	return floorValue + 1
	-- end
	return ret
end