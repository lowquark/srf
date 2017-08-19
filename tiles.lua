
local tiles = {}

function assign(t, ...)
  for i=1,select('#', ...) do
    for k,v in pairs(select(i, ...)) do
      t[k] = v
    end
  end
  return t
end

local Object = require 'Object'
local parts = require 'parts'

function tiles.Grass(params)
  return Object(parts.BasicTileGlyph{
    color = { r = 0x22, g = 0x55, b = 0x33 },
    bgcolor = { r = 0x11, g = 0x22, b = 0x22 },
    index = 5 + 6*16,
  })
end
function tiles.Path(params)
  return Object(parts.BasicTileGlyph{
    color = { r = 0x44, g = 0x44, b = 0x22 },
    bgcolor = { r = 0x11, g = 0x22, b = 0x22 },
    index = 5 + 6*16,
  })
end
function tiles.Badness(params)
  return Object(parts.BasicTileGlyph{
    color = { r = 0xBB, g = 0x22, b = 0x11 },
    bgcolor = { r = 0x88, g = 0x44, b = 0x22 },
    index = 9 + 7*16,
  }, parts.Bad())
end

return tiles

