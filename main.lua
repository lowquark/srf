#!/usr/bin/luajit

-- This is the ideal main body. You may not like it, but this is what peak performance looks like.

local EventEmitter = require 'EventEmitter'

local timer = require 'timer'
local gfx = require 'gfx'
local input = require 'input'

local image = require 'image'
local font = require 'font'

local Vec2 = function(x, y) return { x = x, y = y } end

local tree_indices = { 5 + 4*16,
                       6 + 4*16,
                       7 + 4*16 }

local trees = {}
for i=0,16*16-1 do
  trees[i] = tree_indices[math.random(#tree_indices)]
end

local guy_x = 1
local guy_y = 1

--[[
local lab = gfx.Label()
lab:set_font('file://KingThings-Exeter/18')
lab:set_position(70, 20)
lab:set_text('asdfasdf')
lab:set_foreground(0xFF, 0xFF, 0xFF, 0xFF)
lab:set_background(0x00, 0x00, 0x00, 0x00)
lab:set_padding(5)
]]--

local tm = gfx.Tilemap(16, 16)
tm:set_tileset('file://super_tiles')

function draw()
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

  gfx.draw_label(label)

  gfx.flush()
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
  if key == input.scancode.numpad_6 then
    print('6!')
    guy_x = guy_x + 1
  elseif key == input.scancode.numpad_8 then
    print('8!')
    guy_y = guy_y - 1
  elseif key == input.scancode.numpad_4 then
    print('4!')
    guy_x = guy_x - 1
  elseif key == input.scancode.numpad_2 then
    print('2!')
    guy_y = guy_y + 1
  end
  draw()
end)

input.on('keyup', function (key)
end)

--[[
timer.loop(160, function ()
  print('~160ms have elapsed')
end)

for i=1,100 do
  local oneshot_timer = timer.oneshot(0, function ()
    print(i, 'hahaha, the timer already fired!')
  end)
  oneshot_timer:cancel()
end
]]

