
local gfx = require 'gfx'
local input = require 'input'

local guy_x = 1
local guy_y = 1

local objects = require 'objects'
local tiles = require 'tiles'


local function Glyph()
  return { 
    color = { r = 0xFF, g = 0xFF, b = 0xFF },
    index = 8,
  }
end
local function TileGlyph()
  return { 
    color = { r = 0x33, g = 0x33, b = 0x33 },
    bgcolor = { r = 0x00, g = 0x00, b = 0x22 },
    index = 7,
  }
end

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

local tm = gfx.Tilemap(32, 32)
tm:set_tileset('file://super_tiles')

local function draw()
  for y=0,31 do
    for x=0,31 do
      local object = object_field:get(x, y)
      local tile = tile_field:get(x, y) or tiles.Badness()

      if tile then
        local tile_glyph = tile:message('tile_glyph', TileGlyph())
        if object then
          local glyph = object:message('glyph', Glyph())
          tm:set_index(x, y, glyph.index)
          tm:set_foreground(x, y, glyph.color.r,
                                  glyph.color.g,
                                  glyph.color.b)
          tm:set_background(x, y, tile_glyph.bgcolor.r,
                                  tile_glyph.bgcolor.g,
                                  tile_glyph.bgcolor.b)
        else
          tm:set_index(x, y, tile_glyph.index)
          tm:set_foreground(x, y, tile_glyph.color.r,
                                  tile_glyph.color.g,
                                  tile_glyph.color.b)
          tm:set_background(x, y, tile_glyph.bgcolor.r,
                                  tile_glyph.bgcolor.g,
                                  tile_glyph.bgcolor.b)
        end
      end
    end
  end
  tm:set_index(guy_x, guy_y, 2 + 0*16)
  tm:set_foreground(guy_x, guy_y, 0xFF, 0xCC, 0x99)

  gfx.draw_tilemap(tm)

  gfx.flush()
end

local input_events = {}

local function unlisten_events()
  for k,v in pairs(input_events) do
    input.off(k, v)
  end
end
local function listen_events()
  for k,v in pairs(input_events) do
    input.on(k, v)
  end
end

local function run_state(name)
  unlisten_events()
  require(name)()
end

function input_events.keydown(key)
  if key == input.scancode.right then
    guy_x = guy_x + 1
  elseif key == input.scancode.up then
    guy_y = guy_y - 1
  elseif key == input.scancode.left then
    guy_x = guy_x - 1
  elseif key == input.scancode.down then
    guy_y = guy_y + 1
  elseif key == input.scancode.m then
    run_state 'menu'
    return
  end

  local tile = tile_field:get(guy_x, guy_y)
  if tile and tile:message('bad').bad then
    print('you fuckin died in the badness')
    run_state 'menu'
    return
  end

  draw()
end
function input_events.quit()
end

return function()
  print('game: initializing!')
  guy_x = 1
  guy_y = 1

  listen_events()

  draw()
end

