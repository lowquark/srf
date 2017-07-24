#!/usr/bin/luajit

-- whether you like it or not this is what the ideal main body looks like

local EventEmitter = require 'EventEmitter'
local timer = require 'timer'
local gfx = require 'gfx'
local input = require 'input'

local font = gfx.Font('KingThings-Exeter.ttf')
local words = gfx.Label('asdfasdf', font)
local tile_set = gfx.TileSet(gfx.Texture('assets/tile_s.png'), 16, 16)
local tile_map = gfx.TileMap(64, 64, tile_set)

function draw()
  --[[
  tile_map:cursor(10, 10)
  tile_map:tile('@')
  tile_map:fg_color(0xFF, 0xFF, 0xFF)
  tile_map:bg_color(0x00, 0x00, 0x00)
  tile_map:position(0, 0)
  tile_map:draw() -- enqueues state

  words:text('asdfasdf2', font)
  words:position(22, 22)
  words:draw() -- enqueues state
  ]]

  --gfx.flush(draw)
  gfx.flush(function() print('window flushed!') end)
end

gfx.create_window(16*16, 16*16, false, function (status)
  if status then
    print('window created!')
  else
    print('failed to create window...!')
  end

  draw()
end)

input.on('quit', function ()
  print('input.on(\'quit\')!')
end)

input.on('keydown', function (key)
  if key == input.scancode.a then
    print('a!')
  elseif key == input.scancode.b then
    print('b!')
  end
end)

input.on('keyup', function (key)
end)

timer.loop(160, function ()
  print('~160ms have elapsed')
end)

for i=1,100 do
  local oneshot_timer = timer.oneshot(0, function ()
    print(i, 'hahaha, the timer already fired!')
  end)
  oneshot_timer:cancel()
end

