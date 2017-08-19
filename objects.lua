
--[[
function assign(t, ...)
  for i=1,select('#', ...) do
    for k,v in pairs(select(i, ...)) do
      t[k] = v
    end
  end
  return t
end
]]

local objects = {}

local Object = require 'Object'
local parts = require 'parts'

function objects.NiceTree()
  return Object(parts.BasicGlyph{
    color = { r = 0x33, g = 0x66, b = 0x33 },
    index = 6 + 4*16,
  })
end
function objects.MeanTree(params)
  return Object(parts.BasicGlyph{
    color = { r = 0x22, g = 0x55, b = 0x33 },
    index = 5 + 4*16,
  })
end

return objects

