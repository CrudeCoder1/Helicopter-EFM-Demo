dofile("Aircraft/Planes/AH6JPlane.lua")

AH6J = plane:new()

dofile("Aircraft/Engines/AH6J_Engine.lua")

function AH6J:createEngines()
    self.engines[1] = engine:new()
    self.engines[1]:init(1, host)
    self.engines[1]:initCptNames()
end

AH6J:createEngines()

function onUpdate(params)
    AH6J:onUpdate(params)
end
