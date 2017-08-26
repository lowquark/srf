#!/usr/bin/luajit

local timer = require 'srf.timer'
local gfx = require 'srf.gfx'
local input = require 'srf.input'
local db = require 'srf.db' . KVStore()

db:open('test_save', function(succ)
  print('db:open(): '..tostring(succ))
end)
db:read('asdfasdf', function(value)
  print('db:read(`asdfasdf`): `'..tostring(value)..'`')
end)
db:write('somekey', 'something', function()
  print('db:write(...) finished!')
end)

local module = require 'module'

--[[
local lab = gfx.Label()
lab:set_font('file://KingThings-Exeter/18')
lab:set_position(70, 20)
lab:set_text('asdfasdf')
lab:set_foreground(0xFF, 0xFF, 0xFF, 0xFF)
lab:set_background(0x00, 0x00, 0x00, 0x00)
lab:set_padding(5)
]]--

gfx.create_window(32*16, 32*16, false, function (status)
  if not status then error('failed to create window...!') return end
  module.next(require 'module.menu')
end)

