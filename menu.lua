
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

local stop

local function handle_keydown(key)
  print('Menu/handle_keydown()!')
  if key == input.scancode.n then
    stop()
    require'game'()
    return
  end
  draw()
end

stop = function()
  print('Menu/stop()!')
  gfx.off('exposed', draw)
  input.off('keydown', handle_keydown)
end

return function()
  print('Menu/start()!')

  input.on('keydown', handle_keydown)
  gfx.on('exposed', draw)
  draw()
end

