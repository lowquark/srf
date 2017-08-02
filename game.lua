
local gfx = require 'gfx'
local input = require 'input'

local guy_x = 1
local guy_y = 1

local tm = gfx.Tilemap(16, 16)
tm:set_tileset('file://super_tiles')

local tree_indices = { 5 + 4*16,
                       6 + 4*16,
                       7 + 4*16 }

local trees = {}
for i=0,16*16-1 do
  trees[i] = tree_indices[math.random(#tree_indices)]
end

local function draw()
  for j=0,15 do
    for i=0,15 do
      local idx = i + j*16
      tm:set_index(i, j, trees[idx])
      tm:set_foreground(i, j, 0x33, 0x66, 0x33)
      tm:set_background(i, j, 0x11, 0x22, 0x22)
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
    print('6!')
    guy_x = guy_x + 1
  elseif key == input.scancode.up then
    print('8!')
    guy_y = guy_y - 1
  elseif key == input.scancode.left then
    print('4!')
    guy_x = guy_x - 1
  elseif key == input.scancode.down then
    print('2!')
    guy_y = guy_y + 1
  elseif key == input.scancode.m then
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

