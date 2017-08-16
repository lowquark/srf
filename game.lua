
local gfx = require 'gfx'
local input = require 'input'

local guy_x = 1
local guy_y = 1

-- objects
local tree_ctors = {}
function tree_ctors.nice()
  return {
    hp = 20,
    df = 20,
    color = { r = 0x33, g = 0x66, b = 0x33 },
    glyph = 6 + 4*16,
  }
end
function tree_ctors.mean()
  return {
    hp = 30,
    df = 30,
    color = { r = 0x22, g = 0x55, b = 0x33 },
    glyph = 5 + 4*16,
  }
end
local function Tree(type)
  return tree_ctors[type]()
end

-- tiles
local function GrassTile()
  return {
    color = { r = 0x22, g = 0x55, b = 0x33 },
    bgcolor = { r = 0x11, g = 0x22, b = 0x22 },
    glyph = 5 + 6*16,
  }
end
local function PathTile()
  return {
    color = { r = 0x44, g = 0x44, b = 0x22 },
    bgcolor = { r = 0x11, g = 0x22, b = 0x22 },
    glyph = 5 + 6*16,
  }
end
local function BadnessTile()
  return {
    color = { r = 0xBB, g = 0x22, b = 0x11 },
    bgcolor = { r = 0x88, g = 0x44, b = 0x22 },
    glyph = 9 + 7*16,
    bad = true,
  }
end

local hl_terrain = require('Map')(32, 32)
hl_terrain:each(function(i, j, value)
  local p = 20
  if i > 10 then
    p = 2
  end

  if math.random(p) ~= 1 then
    hl_terrain:set(i, j, 'forest')
  end
end)

--local tree_field = require('Field')(0, 0, 15, 15, tree_indices[1])
local tiles = hl_terrain:copy()
tiles:map(function(val)
  if val == 'forest' then
    local r = math.random(6)
    if r <= 3 then
      return GrassTile()
    else
      return PathTile()
    end
  else
    return BadnessTile()
  end
end)

local object_field = hl_terrain:copy()
object_field:map(function(val)
  if val == 'forest' then
    local r = math.random(6)
    if r <= 3 then
      return Tree('nice')
    elseif r <= 4 then
      return Tree('mean')
    end
  end
end)

local tm = gfx.Tilemap(32, 32)
tm:set_tileset('file://super_tiles')

local function draw()
  for y=0,31 do
    for x=0,31 do
      local object = object_field:get(x, y)
      local tile = tiles:get(x, y)

      if tile then
        if object then
          tm:set_index(x, y, object.glyph)
          tm:set_foreground(x, y, object.color.r, object.color.g, object.color.b)
        else
          tm:set_index(x, y, tile.glyph)
          tm:set_foreground(x, y, tile.color.r, tile.color.g, tile.color.b)
        end

        tm:set_background(x, y, tile.bgcolor.r, tile.bgcolor.g, tile.bgcolor.b)
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
  print('game: handle keydown!')
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

  local tile = tiles:get(guy_x, guy_y)
  if not tile or tile.bad then
    print('you fuckin died in the badness')
    run_state 'menu'
    return
  end

  draw()
end
function input_events.quit()
  print('game: handle quit!')
end

return function()
  print('game: initializing!')
  guy_x = 1
  guy_y = 1

  listen_events()

  draw()
end

