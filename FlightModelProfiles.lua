-- Selection-specific aircraft metadata; the native selector uses the same IDs.
local selected = dofile(current_mod_path..'/FlightModelSelection.lua')
local ah6j_uh_efm = selected == 'ah6j_oh6_cr3144_derivative'
local profile = {id=ah6j_uh_efm and selected or 'ah6j'}

function profile.applyAircraft(aircraft)
    if not ah6j_uh_efm then return end
    -- Preserve all original mass, crew, payload and SoloFlight assumptions.
    aircraft.rotor_RPM = 477
    aircraft.tail_rotor_RPM = 2838
    aircraft.rotor_diameter = (27+4/12)*0.3048
    aircraft.rotor_MOI = 388.82247812815683 -- blade/pitchcase-equivalent mast inertia
end
return profile
