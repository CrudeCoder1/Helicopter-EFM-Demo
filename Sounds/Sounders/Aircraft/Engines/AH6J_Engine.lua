dofile("Tools.lua")

engine = {number = 0, EXT_ROTOR_IN_CPT = 0.35}

function engine:new()
    local o = {exterior = {}, cockpit = {}}
    setmetatable(o, self)
    self.__index = self
    return o
end

function engine:initNames()
    self.rotor_name = "Rotor"
    self.engine_name = "EngineTV3117L"
    self.engine_r_name = "EngineTV3117R"
    self.start_name = "EngineTV3117Start"
end

function engine:initCptNames()
    self.engine_l_cpt_name = "EngineTV3117InL"
    self.engine_r_cpt_name = "EngineTV3117InR"
end

function engine:init(number, host)
    self.number = number
    self:initNames()
    self:createSounds(host)
end

function engine:createSounds(host)
    self.exterior.rotor = ED_AudioAPI.createSource(host, self.rotor_name)
    -- self.exterior.engine = ED_AudioAPI.createSource(host, self.engine_name)
    -- self.exterior.engine_r = ED_AudioAPI.createSource(host, self.engine_r_name)
    -- self.exterior.start = ED_AudioAPI.createSource(host, self.start_name)
end

function engine:createSoundsCpt(hostCpt)
    -- self.cockpit.engine_l = ED_AudioAPI.createSource(hostCpt, self.engine_l_cpt_name)
    -- self.cockpit.engine_r = ED_AudioAPI.createSource(hostCpt, self.engine_r_cpt_name)
    self.cockpit.rotor = ED_AudioAPI.createSource(hostCpt, self.rotor_name)
end

function engine:destroySoundsCpt()
    for key, source in pairs(self.cockpit) do
        ED_AudioAPI.destroySource(source)
        self.cockpit[key] = nil
    end
end

function engine:DBGstop()
    for _, sources in ipairs({self.exterior, self.cockpit}) do
        for _, source in pairs(sources) do
            ED_AudioAPI.stopSource(source)
        end
    end
end

function engine:controlSound(source, pitch, gain)
    if source == nil then return end
    if gain < 0.01 then
        ED_AudioAPI.stopSource(source)
        return
    end
    ED_AudioAPI.setSourcePitch(source, math.max(0.05, pitch))
    ED_AudioAPI.setSourceGain(source, gain)
    if not ED_AudioAPI.isSourcePlaying(source) then
        ED_AudioAPI.playSourceLooped(source)
    end
end

function engine:update(coreRPM, fanRPM, turbPower, thrust, flame, vTrue)
    local core = math.max(0, coreRPM or 0)
    local rotor = math.max(0, fanRPM or 0)
    -- The EFM supplies N1 as core RPM and geared N2 as fan RPM.
    -- N2 follows the engine shaft; it can differ from rotor speed with the clutch disengaged.
    local rotorGain = math.min(1.2, rotor * rotor)
    local running = math.min(1, math.max(0, (core - 0.45) / 0.30))
    local engineGain = math.min(1.2, core) * running
    local startGain = math.min(1, core / 0.25) * (1 - running)

    self:controlSound(self.exterior.rotor, rotor, rotorGain)
    -- self:controlSound(self.exterior.engine, core, engineGain * 0.5)
    -- self:controlSound(self.exterior.engine_r, core, engineGain * 0.5)
    -- self:controlSound(self.exterior.start, 0.5 + 0.5 * core, startGain)
    -- self:controlSound(self.cockpit.engine_l, core, math.min(1, core) * 0.4)
    -- self:controlSound(self.cockpit.engine_r, core, math.min(1, core) * 0.4)
    self:controlSound(self.cockpit.rotor, rotor, rotorGain * self.EXT_ROTOR_IN_CPT)
end
