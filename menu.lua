
local gfx = require 'gfx'
local input = require 'input'

local tm = gfx.Tilemap(16, 16)
tm:set_tileset('file://super_tiles')

local menu_background = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,0,1,0,1,1,1,0,1,0,0,1,0,1,0,1,
  1,1,1,0,1,0,0,0,1,1,0,1,0,1,0,1,
  1,0,1,0,1,1,0,0,1,0,1,1,0,1,0,1,
  1,0,1,0,1,0,0,0,1,0,0,1,0,1,0,1,
  1,0,1,0,1,0,0,0,1,0,0,1,0,1,0,1,
  1,0,1,0,1,1,1,0,1,0,0,1,0,1,1,1,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,
  0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,
  0,0,0,0,0,0,1,0,1,0,0,0,0,2,0,0,
  0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,
  0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
}

local function draw()
  for j=0,15 do
    for i=0,15 do
      local val = menu_background[i + j * 16 + 1]
      if val == 0 then
        tm:set_index(i, j, 25)
        tm:set_foreground(i, j, 0x55, 0x55, 0x55)
        tm:set_background(i, j, 0x11, 0x22, 0x22)
      elseif val == 1 then
        tm:set_index(i, j, 24)
        tm:set_foreground(i, j, 0x77, 0x77, 0x77)
        tm:set_background(i, j, 0x33, 0x22, 0x22)
      elseif val == 2 then
        tm:set_index(i, j, 2 + 0*16)
        tm:set_foreground(i, j, 0xFF, 0xCC, 0x99)
        tm:set_background(i, j, 0x11, 0x22, 0x22)
      end
    end
  end

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
  print('menu: handle keydown!')
  if key == input.scancode.n then
    run_state 'game'
    return
  end
  draw()
end
function input_events.quit()
  print('menu: handle quit!')
end

return function()
  print('menu: initializing!')

  listen_events()

  draw()
end
