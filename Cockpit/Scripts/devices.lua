local count = 0
local function counter()
	count = count + 1
	return count
end
-------DEVICE ID-------
devices = {}
devices["ELECTRIC_SYSTEM"]	= counter()
devices["WEAPON_SYSTEM"]	= counter()
devices["RWR"] 				= counter()
devices["EXTLIGHTS"]		= counter()
devices["UHF_RADIO"]		= counter()
devices["INTERCOM"]			= counter()
devices["HELMET_DEVICE"] 	= counter() 
devices["AVIONICS"]			= counter()
devices["DIGITAL_CLOCK"]	= counter() 
devices["KNEEBOARD"]		= counter() 
devices["EFM_HELPER"]		= counter() 
