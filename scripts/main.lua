
--[[
local db = require 'srf.db' . KVStore()

db:open('test_save', function(succ)
  print('db:open(): '..tostring(succ))
  io.flush()
end)
db:write('asdfasdf', 'something', function()
  print('db:write(`asdfasdf`, `something`) finished!')
  io.flush()
end)
db:write('asdfasdf', 'something2', function()
  print('db:write(`asdfasdf`, `something2`) finished!')
  io.flush()
end)
db:write('asdfasdf', 'something3', function()
  print('db:write(`asdfasdf`, `something3`) finished!')
  io.flush()
end)
db:write('asdfasdf', 'something4', function()
  print('db:write(`asdfasdf`, `something4`) finished!')
  io.flush()
end)
db:read('asdfasdf', function(value)
  print('db:read(`asdfasdf`): `'..tostring(value)..'`')
  io.flush()
end)
db:close()
]]

local module = require 'module'
local gfx = require 'srf.gfx'

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

