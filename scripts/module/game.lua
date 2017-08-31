
local gfx = require 'srf.gfx'
local input = require 'srf.input'
local db = require 'srf.db'
local log = require 'srf.log'
local mp = require 'MessagePack'
local module = require 'module'
local inspect = require 'inspect'

local object = require 'game.object'
local parts = require 'game.parts'
local gen = require 'game.gen'
local level = require 'game.level'

-- currently loaded level
local the_level = nil

local guy_x = nil
local guy_y = nil

local function save_object(o)
  return object.save(o)
end
local function load_object(ostate)
  return object.load(ostate, function(pname) return parts[pname] end)
end


local function save_game(world_save)
  log('saving game...')
  world_save:write('gamestate', mp.pack({ guy_x = guy_x, guy_y = guy_y }))

  local lstate = level.save(the_level, save_object)
  world_save:write('starting_area', mp.pack(lstate))
end
local function load_game(world_save, on_finish)
  log('loading game...')
  world_save:read('gamestate', function(str)
    local gamestate = mp.unpack(str)
    guy_x = gamestate.guy_x
    guy_y = gamestate.guy_y
  end)
  world_save:read('starting_area', function(str)
    the_level = level.load(mp.unpack(str), load_object)
    on_finish()
  end)
end
local function new_game(world_save)
  log('generating new game...')
  world_save:write('gamestate', mp.pack({ guy_x = 1, guy_y = 1 }))

  gen.generate_level(world_save, 'starting_area')
end

local world_save = db.KVStore()

local tm = gfx.Tilemap(32, 32)
tm:set_tileset('file://super_tiles')

local function draw()
  if the_level then
    for y=1,32 do
      for x=1,32 do
        local idx = x + (y - 1) * the_level.width

        local tile_obj = the_level.tiles[idx]

        local tile_glyph = {}
        object.message(tile_obj, 'tile_glyph', tile_glyph)

        if tile_glyph.index then
          tm:set_index(x - 1, y - 1, tile_glyph.index)
          tm:set_foreground(x - 1, y - 1, tile_glyph.color.r,
                                          tile_glyph.color.g,
                                          tile_glyph.color.b)
          tm:set_background(x - 1, y - 1, tile_glyph.bgcolor.r,
                                          tile_glyph.bgcolor.g,
                                          tile_glyph.bgcolor.b)
        end
      end
    end
  end

  for i,o in ipairs(the_level.objects) do
    if o.position then
      local x = o.position.x
      local y = o.position.y

      local glyph = {}
      object.message(o, 'glyph', glyph)

      if glyph.index then
        tm:set_index(x, y, glyph.index)
        tm:set_foreground(x, y, glyph.color.r, glyph.color.g, glyph.color.b)
      end
    end
  end

  tm:set_index(guy_x, guy_y, 2 + 0*16)
  tm:set_foreground(guy_x, guy_y, 0xFF, 0xCC, 0x99)

  gfx.draw_tilemap(tm)

  gfx.flush()
end

function handle_keydown(key)
  if key == input.scancode.right then
    guy_x = guy_x + 1
  elseif key == input.scancode.up then
    guy_y = guy_y - 1
  elseif key == input.scancode.left then
    guy_x = guy_x - 1
  elseif key == input.scancode.down then
    guy_y = guy_y + 1
  elseif key == input.scancode.m then
    save_game(world_save)
    module.next(require 'module.menu')
    return
  end

  if guy_x >= 0 and guy_x < the_level.width and
     guy_y >= 0 and guy_y < the_level.height then
    local idx = guy_x + guy_y * the_level.width + 1
    local tile = the_level.tiles[idx]

    if tile and tile[1] then
      tile[1].color.r = 0xFF
    end
  end

  --[[
  local tile = tile_field:get(guy_x, guy_y)
  if tile and tile:message('bad').bad then
    log('you fuckin died in the badness')
    -- delete save
    gamesaves.delete('dracula')
    -- create morgue file
    --...?
    module.next(require 'menu')
    return
  end
  ]]

  draw()
end
function handle_quit()
  save_game(world_save)
end

local game = {}
function game:init(save_name)
  log('game:init()')

  world_save:open('saves/'..save_name, function(success)
    if success then
      world_save:read('gamestate', function(str)
        if str then
          -- temporary
          new_game(world_save)
          --
          load_game(world_save, function()
            draw()
          end)
        else
          new_game(world_save)
          load_game(world_save, function()
            draw()
          end)
        end
      end)
    end
  end)

  input.on('keydown', handle_keydown)
  input.on('quit', handle_quit)
end

function game:deinit()
  log('game:deinit()')

  input.off('keydown', handle_keydown)
  input.off('quit', handle_quit)
end

return game

