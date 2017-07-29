#!/usr/bin/luajit

-- This is the ideal main body. You may not like it, but this is what peak performance looks like.

local EventEmitter = require 'EventEmitter'

local timer = require 'timer'
local gfx = require 'gfx'
local input = require 'input'

local image = require 'image'
local font = require 'font'

local Vec2 = function(x, y) return { x = x, y = y } end

local tile_map = {
  tiles = 'file://super_tiles',
  tile_size = Vec2(16, 16),
  size = Vec2(2, 2),
  tiles = { 1, 2, 3, 4, },
  fg_colors = { 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 
                0xFF, 0xFF, 0xFF, 0xFF, 
                0xFF, 0xFF, 0xFF, 0xFF, },
  bg_colors = { 0xFF, 0x7F, 0x00, 0xFF,
                0xFF, 0x7F, 0x00, 0xFF, 
                0xFF, 0x7F, 0x00, 0xFF, 
                0xFF, 0x7F, 0x00, 0xFF, },
}

local label = {
  font = 'file://KingThings-Exeter/18',
  x = 70,
  y = 20,
  text = 'asdfasdf',
  fg_color = { 0xFF, 0xFF, 0xFF, 0xFF, },
  bg_color = { 0x00, 0x00, 0x00, 0xFF, },
  bg_padding = 5,
}

function draw()
  gfx.clip(0, 0, 50, 50)
  gfx.draw_tile_map(tile_map)
  gfx.unclip()

  gfx.draw_label(label)

  --gfx.flush(draw)
  gfx.flush(function() print('window flushed!') end)
end

gfx.create_window(16*16, 16*16, false, function (status)
  if not status then print('failed to create window...!') return end

  print('window created!')

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

