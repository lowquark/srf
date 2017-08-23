
local state = require 'game.state'

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
return function(w, h, tiles, objects)
  return setmetatable({
    width = w or 0,
    height = h or 0,
    tiles = tiles or {},
    objects = objects or {},
  }, Level_mt)
end

