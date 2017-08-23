
local gfx = require 'gfx'
local input = require 'input'
local module = require 'module'
local gamesaves = require 'gamesaves'
local state = require 'state'

local guy_x = 1
local guy_y = 1

--[[
-- tiles
local hl_terrain = require('Field')(0, 0, 31, 31, 'badness')
for y=0,31 do
  for x=0,31 do
    local p = 20
    if x > 10 then
      p = 2
    end

    if math.random(p) ~= 1 then
      hl_terrain:set(x, y, 'forest')
    end
  end
end

--local tree_field = require('Field')(0, 0, 15, 15, tree_indices[1])
local tile_field = hl_terrain:copy()
tile_field:map(function(val)
  if val == 'forest' then
    local r = math.random(6)
    if r <= 3 then
      return tiles.Grass()
    else
      return tiles.Path()
    end
  else
    return tiles.Badness()
  end
end)

local object_field = hl_terrain:copy()
object_field:map(function(val)
  if val == 'forest' then
    local r = math.random(6)
    if r <= 3 then
      return objects.NiceTree()
    elseif r <= 4 then
      return objects.MeanTree()
    end
  end
end)
]]

local Object_mt = { __index = {} }
function Object_mt.__index:attach(p)
  table.insert(self, p)
end
function Object_mt.__index:save_state()
  local st = state.Object()
  for i,p in ipairs(self) do
    st[i] = p
  end
  return st
end
local function Object()
  return setmetatable({}, Object_mt)
end

local Level_mt = { __index = {} }
function Level_mt.__index:save_state()
  local st = state.Level(self.width, self.height)

  -- save the state of all tiles
  for i=1,self.width*self.height do
    if self.tiles[i] then
      st.tiles[i] = self.tiles[i]:save_state()
    end
  end

  -- save the state of all objects
  for i,o in ipairs(self.objects) do
    st.objects[i] = o:save_state()
  end

  return st
end
local function Level(w, h, tiles, objects)
  return setmetatable({
    width = w or 0,
    height = h or 0,
    tiles = tiles or {},
    objects = objects or {},
  }, Level_mt)
end

local function load_state(st)
  if st.__type == 'Level' then
    local w = st.width
    local h = st.height
    local tiles = {}
    for i=1,w*h do
      if st.tiles[i] then
        tiles[i] = load_state(st.tiles[i])
      end
    end
    return Level(w, h, tiles)
  elseif st.__type == 'Object' then
    local o = Object()
    for k,part_st in ipairs(st) do
      o:attach(load_state(part_st))
    end
    return o
  else
    local t = {}
    for k,v in pairs(st) do
      t[k] = v
    end
    return t
  end
end

-- buffered gamestate
local gamestate = nil
-- currently loaded level
local level = nil

local tm = gfx.Tilemap(32, 32)
tm:set_tileset('file://super_tiles')

-- compiles a gamestate from loaded game
local function update_gamestate()
  gamestate.guy_x = guy_x
  gamestate.guy_y = guy_y

  gamestate.levels['forest'] = level:save_state()
end
local function save_game()
  update_gamestate()
  gamesaves.save('dracula', gamestate)
end
-- loads a gamestate
local function load_game(g)
  gamestate = g

  guy_x = gamestate.guy_x
  guy_y = gamestate.guy_y

  level = load_state(gamestate.levels['forest'])
end

-- creates a brand new gamestate
local function new_game()
  local g = state.Game()
  local l = state.Level(64, 64)
  for i=1,64*64 do
    l.tiles[i] = state.Object{
      state.TileGlyph(5 + 6*16, 0x22, 0x55, 0x33, 0x11, 0x22, 0x22),
    }
  end
  g.levels['forest'] = l

  g.guy_x = 1
  g.guy_y = 1

  return g
end

local function draw()
  if level then
    for y=1,32 do
      for x=1,32 do
        local idx = x + (y - 1) * level.width

        local tile_obj = level.tiles[idx]

        local tile_glyph
        if tile_obj then
          tile_glyph = tile_obj[1] --TODO: Not like this
        else
          tile_glyph = state.TileGlyph()
        end

        tm:set_index(x - 1, y - 1, tile_glyph.index)
        tm:set_foreground(x - 1, y - 1, tile_glyph.color.r,
                                        tile_glyph.color.g,
                                        tile_glyph.color.b)
        tm:set_background(x - 1, y - 1, tile_glyph.bgcolor.r,
                                        tile_glyph.bgcolor.g,
                                        tile_glyph.bgcolor.b)

        --[[
        local object = object_field:get(x, y)
        local tile = tile_field:get(x, y) or tiles.Badness()

        if tile then
          local tile_glyph = tile:message('tile_glyph', state.TileGlyph())
          if object then
            local glyph = object:message('glyph', state.Glyph())
            tm:set_index(x, y, glyph.index)
            tm:set_foreground(x, y, glyph.color.r,
                                    glyph.color.g,
                                    glyph.color.b)
            tm:set_background(x, y, tile_glyph.bgcolor.r,
                                    tile_glyph.bgcolor.g,
                                    tile_glyph.bgcolor.b)
          else
            tm:set_index(x, y, tile_glyph.index)
            tm:set_foreground(x, y, tile_glyph.color.r,
                                    tile_glyph.color.g,
                                    tile_glyph.color.b)
            tm:set_background(x, y, tile_glyph.bgcolor.r,
                                    tile_glyph.bgcolor.g,
                                    tile_glyph.bgcolor.b)
          end
        end
        ]]
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
    save_game()
    module.next(require 'menu')
    return
  end

  local tile = level.tiles[guy_x + guy_y * level.width + 1]

  if tile and tile[1] then
    tile[1].color.r = 0xFF
  end

  --[[
  local tile = tile_field:get(guy_x, guy_y)
  if tile and tile:message('bad').bad then
    print('you fuckin died in the badness')
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
  save_game()
end

local game = {}
function game:init(gamestate)
  print('game:init()')

  print('gamestate: '..tostring(gamestate))

  if gamestate then
    load_game(gamestate)
  else
    load_game(new_game())
  end

  input.on('keydown', handle_keydown)
  input.on('quit', handle_quit)

  draw()
end

function game:deinit()
  print('game:deinit()')

  input.off('keydown', handle_keydown)
  input.off('quit', handle_quit)
end

return game

