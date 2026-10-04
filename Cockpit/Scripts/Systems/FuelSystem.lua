dofile(LockOn_Options.script_path.."command_defs.lua")

local dev = GetSelf()
local update_time_step = 0.25 --update will be called 10 times per second
make_default_activity(update_time_step)

--local cockpitDev = GetDevice(0)

-- TODO "A" letter for aft tank, find out what FTI switch position does (maybe stops flashing FTI light?)

-- From EFM
DCbusVoltage  = get_param_handle("DC_Bus_Voltage")
MainFuelTank_lb = get_param_handle("MainFuelTank_lb")
AftFuelTank_lb = get_param_handle("AftFuelTank_lb")

-- Set here
FQI_Brightness  = get_param_handle("FQI_Brightness")
FQI_Quantity  = get_param_handle("FQI_Quantity")
FQI_FTI  = get_param_handle("FQI_FTI")
FQI_AftSel  = get_param_handle("FQI_AftSel")

local FQIbrt = 1
local DayNightVal = 1
local selectSw = 0


function post_initialize()
	local birth = LockOn_Options.init_conditions.birth_place
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then 	 
		dev:performClickableAction(device_commands.FuelPumpSw, 1)
    elseif birth=="GROUND_COLD" then
		
    end
	dev:performClickableAction(device_commands.FQIbrtKnob, 0.9)
	dev:performClickableAction(device_commands.FQIdayNhtSw, 1)
end


function SetCommand(command,value)
    if command == device_commands.FQIbrtKnob then
        FQIbrt = value		
	elseif command == device_commands.FQIdayNhtSw then
		DayNightVal = value/2+0.5
	elseif command == device_commands.FQIselectSw then
		selectSw = value
		FQI_AftSel:set(value==0.25 and 1 or 0)
	end
	FQI_Brightness:set(FQIbrt*DayNightVal)
end


function RoundToNearest5(n)
    return math.floor(n / 5 + 0.5) * 5
end

function update()
	if selectSw==0 then
		FQI_Quantity:set(RoundToNearest5(MainFuelTank_lb:get()))
	elseif selectSw==0.25 then
		FQI_Quantity:set(RoundToNearest5(AftFuelTank_lb:get()))
	else
		FQI_Quantity:set(0)
	end

	if MainFuelTank_lb:get() < 250 then
		FQI_FTI:set(1-FQI_FTI:get())
	else
		FQI_FTI:set(0)
	end
end


need_to_be_closed = false