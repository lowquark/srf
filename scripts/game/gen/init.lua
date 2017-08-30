
--[[
-- tiles
local hl_terrain = require('Field')(0, 0, 31, 31, 'badness')
for y=0,31 do
  for x=0,31 do
    local p = 20
    if x > 10 then
      p = 2
    end

    if math.random(p) ~= 1 then
      hl_terrain:set(x, y, 'forest')
    end
  end
end

--local tree_field = require('Field')(0, 0, 15, 15, tree_indices[1])
local tile_field = hl_terrain:copy()
tile_field:map(function(val)
  if val == 'forest' then
    local r = math.random(6)
    if r <= 3 then
      return tiles.Grass()
    else
      return tiles.Path()
    end
  else
    return tiles.Badness()
  end
end)

local object_field = hl_terrain:copy()
object_field:map(function(val)
  if val == 'forest' then
    local r = math.random(6)
    if r <= 3 then
      return objects.NiceTree()
    elseif r <= 4 then
      return objects.MeanTree()
    end
  end
end)
]]


local mp = require 'MessagePack'

local gen = {}

function gen.generate_level(world_save, loc_name)
  local tstates = {}
  local ostates = {}

  for i=1,64*64 do
    tstates[i] = { parts = {
      { 'TileGlyph', { r = 0x22, g = 0x55, b = 0x33, },
                     { r = 0x11, g = 0x22, b = 0x22, },
                     5 + 6*16, },
      }
    }
  end

  local l = {
    width = 64,
    height = 64,
    tiles = tstates,
    objects = ostates,
  }

  local l_str = mp.pack(l)
  world_save:write(loc_name, mp.pack(l))
end

return gen

