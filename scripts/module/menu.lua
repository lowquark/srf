
local gfx = require 'srf.gfx'
local input = require 'srf.input'

local module = require 'module'
local gamesaves = require 'gamesaves'

local tm = gfx.Tilemap(32, 32)
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
      local x = i + 8
      local y = j + 8

      local val = menu_background[i + j * 16 + 1]
      if val == 0 then
        tm:set_index(x, y, 25)
        tm:set_foreground(x, y, 0x55, 0x55, 0x55)
        tm:set_background(x, y, 0x11, 0x22, 0x22)
      elseif val == 1 then
        tm:set_index(x, y, 24)
        tm:set_foreground(x, y, 0x77, 0x77, 0x77)
        tm:set_background(x, y, 0x33, 0x22, 0x22)
      elseif val == 2 then
        tm:set_index(x, y, 2 + 0*16)
        tm:set_foreground(x, y, 0xFF, 0xCC, 0x99)
        tm:set_background(x, y, 0x11, 0x22, 0x22)
      end
    end
  end

  gfx.draw_tilemap(tm)

  gfx.flush()
end

local function handle_keydown(key)
  if key == input.scancode.n then
    module.next(require 'module.game', gamesaves.load('dracula'))
    return
  end
  draw()
end
local function handle_quit()
  module.next(nil)
end

local menu = {}
function menu:init()
  print('menu:init()')

  input.on('keydown', handle_keydown)
  input.on('quit', handle_quit)

  draw()
end
function menu:deinit()
  print('menu:deinit()')

  input.off('keydown', handle_keydown)
  input.off('quit', handle_quit)
end

return menu

