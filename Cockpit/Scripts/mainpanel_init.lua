shape_name  	 			 = "Cockpit_AH-6"
draw_pilot					 = false


day_texture_set_value   = 0.0
night_texture_set_value = 0.1
dusk_border					 = 0.4
--external_model_canopy_arg	 = 38
--render_debug_info = false


local controllers = LoRegisterPanelControls()


IASneedle							= CreateGauge()
IASneedle.arg_number				= 100
IASneedle.input						= {0,41.156,77.167} -- knots to m/s
IASneedle.output					= {0,0.5,1}
IASneedle.controller				= controllers.base_gauge_IndicatedAirSpeed --m/s

AltNeedle							= CreateGauge("parameter")
AltNeedle.arg_number				= 101
AltNeedle.input						= {0,1000}
AltNeedle.output					= {0,1}
AltNeedle.parameter_name			= "ALT_100"

RadarAltNeedle						= CreateGauge("parameter")
RadarAltNeedle.arg_number			= 102
RadarAltNeedle.input				= {0, 200, 1500}
RadarAltNeedle.output				= {0,0.5, 1}
RadarAltNeedle.parameter_name		= "CURRENT_RALT"

RadarAltOff							= CreateGauge("parameter")
RadarAltOff.arg_number				= 117
RadarAltOff.input					= {0,1}
RadarAltOff.output					= {0,1}
RadarAltOff.parameter_name			= "RALT_OFF"

VVneedle							= CreateGauge()
VVneedle.arg_number					= 103
VVneedle.input						= {-30.48,-20.32,-10.16,-5.08,0,5.08,10.16,20.32,30.48} --1000,2000,4000,6000 ft/min converted to m/s
VVneedle.output						= {-1,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1}
VVneedle.controller					= controllers.base_gauge_VerticalVelocity --m/s

ADIPitch							= CreateGauge()
ADIPitch.arg_number					= 104
ADIPitch.input						= {-math.rad(90),0, math.rad(90)}
ADIPitch.output						= {-1,0, 1}
ADIPitch.controller					= controllers.base_gauge_Pitch

ADIBank								= CreateGauge()
ADIBank.arg_number					= 105
ADIBank.input						= {-math.pi, math.pi}
ADIBank.output						= {1, -1}
ADIBank.controller					= controllers.base_gauge_Roll

ADIslip								= CreateGauge()
ADIslip.arg_number					= 123
ADIslip.input						= {-1, 1}
ADIslip.output						= {-1, 1}
ADIslip.controller					= controllers.base_gauge_AngleOfSlide

Compass								= CreateGauge("parameter")
Compass.arg_number					= 106
Compass.input						= {0,360}
Compass.output						= {0,1}
Compass.parameter_name				= "CURRENT_HDG"

Alt1k								= CreateGauge("parameter")
Alt1k.arg_number					= 108
Alt1k.input							= {0,10000}
Alt1k.output						= {0,1}
Alt1k.parameter_name				= "ALT_1000"

Alt10k								= CreateGauge("parameter")
Alt10k.arg_number					= 107
Alt10k.input						= {0.0, 9900, 10000, 19900, 20000, 29900, 30000, 39900, 40000, 49900, 50000, 59900, 60000, 69900, 70000, 79900, 80000, 89900, 90000, 99900, 100000.0}--{0,100000}
Alt10k.output						= {0.0, 0.09, 0.1, 0.19, 0.2, 0.29, 0.3, 0.39, 0.4, 0.49, 0.5, 0.59, 0.6, 0.69, 0.7, 0.79, 0.8, 0.89, 0.9, 0.99, 1.0}--{0,1}
Alt10k.parameter_name				= "ALT_10000"

AltAdjINHG1							= CreateGauge("parameter")
AltAdjINHG1.arg_number				= 109
AltAdjINHG1.input					= {0,10}
AltAdjINHG1.output					= {0,1}
AltAdjINHG1.parameter_name			= "ALT_ADJ_Nxxx"

AltAdjINHG2							= CreateGauge("parameter")
AltAdjINHG2.arg_number				= 110
AltAdjINHG2.input					= {0,10}
AltAdjINHG2.output					= {0,1}
AltAdjINHG2.parameter_name			= "ALT_ADJ_xNxx"

AltAdjINHG3							= CreateGauge("parameter")
AltAdjINHG3.arg_number				= 111
AltAdjINHG3.input					= {0,10}
AltAdjINHG3.output					= {0,1}
AltAdjINHG3.parameter_name			= "ALT_ADJ_xxNx"

AltAdjINHG4							= CreateGauge("parameter")
AltAdjINHG4.arg_number				= 112
AltAdjINHG4.input					= {0,10}
AltAdjINHG4.output					= {0,1}
AltAdjINHG4.parameter_name			= "ALT_ADJ_xxxN"

AltAdjMB1							= CreateGauge("parameter")
AltAdjMB1.arg_number				= 113
AltAdjMB1.input						= {0,10}
AltAdjMB1.output					= {0,1}
AltAdjMB1.parameter_name			= "ALT_ADJ_MBNxxx"

AltAdjMB2							= CreateGauge("parameter")
AltAdjMB2.arg_number				= 114
AltAdjMB2.input						= {0,10}
AltAdjMB2.output					= {0,1}
AltAdjMB2.parameter_name			= "ALT_ADJ_MBxNxx"

AltAdjMB3							= CreateGauge("parameter")
AltAdjMB3.arg_number				= 115
AltAdjMB3.input						= {0,10}
AltAdjMB3.output					= {0,1}
AltAdjMB3.parameter_name			= "ALT_ADJ_MBxxNx"

AltAdjMB4							= CreateGauge("parameter")
AltAdjMB4.arg_number				= 116
AltAdjMB4.input						= {0,10}
AltAdjMB4.output					= {0,1}
AltAdjMB4.parameter_name			= "ALT_ADJ_MBxxxN"


need_to_be_closed = true -- close lua state after initialization 

--dofile(LockOn_Options.common_script_path.."tools.lua")

--[[ available functions 

 --base_gauge_RadarAltitude
 --base_gauge_BarometricAltitude
 --base_gauge_AngleOfAttack
 --base_gauge_AngleOfSlide
 --base_gauge_VerticalVelocity
 --base_gauge_TrueAirSpeed
 --base_gauge_IndicatedAirSpeed
 --base_gauge_MachNumber
 --base_gauge_VerticalAcceleration --Ny
 --base_gauge_HorizontalAcceleration --Nx
 --base_gauge_LateralAcceleration --Nz
 --base_gauge_RateOfRoll
 --base_gauge_RateOfYaw
 --base_gauge_RateOfPitch
 --base_gauge_Roll
 --base_gauge_MagneticHeading
 --base_gauge_Pitch
 --base_gauge_Heading
 --base_gauge_EngineLeftFuelConsumption
 --base_gauge_EngineRightFuelConsumption
 --base_gauge_EngineLeftTemperatureBeforeTurbine
 --base_gauge_EngineRightTemperatureBeforeTurbine
 --base_gauge_EngineLeftRPM
 --base_gauge_EngineRightRPM
 --base_gauge_WOW_RightMainLandingGear
 --base_gauge_WOW_LeftMainLandingGear
 --base_gauge_WOW_NoseLandingGear
 --base_gauge_RightMainLandingGearDown
 --base_gauge_LeftMainLandingGearDown
 --base_gauge_NoseLandingGearDown
 --base_gauge_RightMainLandingGearUp
 --base_gauge_LeftMainLandingGearUp
 --base_gauge_NoseLandingGearUp
 --base_gauge_LandingGearHandlePos
 --base_gauge_StickRollPosition
 --base_gauge_StickPitchPosition
 --base_gauge_RudderPosition
 --base_gauge_ThrottleLeftPosition
 --base_gauge_ThrottleRightPosition
 --base_gauge_CanopyPos
 --base_gauge_CanopyState
 --base_gauge_FlapsRetracted
 --base_gauge_SpeedBrakePos
 --base_gauge_FlapsPos
 --base_gauge_TotalFuelWeight
--]]
