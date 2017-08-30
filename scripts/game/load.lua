
local gload = {}

local parts = require 'game.parts'

function gload.object(state)
  local o = {}
  for ptype,pstate in pairs(state) do
    local ctor = parts[ptype]
    if ctor then
      ctor(o, unpack(pstate))
    end
  end
  return o
end

return gload
