--Simple upper and lower limiter
function limit(input, lower_limit, upper_limit)
	if (input > upper_limit) then	
		return upper_limit	
	elseif (input < lower_limit) then
		return lower_limit
	else
		return input
	end
end

-- linear interpolate function that will output a value between lowerB and upperB that varies linearly based on lowerA->upperA
function LinInterp(inputA, lowerA, upperA, lowerB, upperB)
	local t = (inputA - lowerA) / (upperA - lowerA)--gives linear value 0-1
	local output = (upperB - lowerB) * t + lowerB

	if (upperB < lowerB) then
		output = limit(output, upperB, lowerB)
	else
		output = limit(output, lowerB, upperB)
	end

	return output
end

function isnan(x) return x ~= x end


