
local state = {}

function state.Object(parts)
  local o = { __type = 'Object' }
  if parts then
    for i,v in ipairs(parts) do
      o[i] = v
    end
  end
  return o
end
function state.Level(w, h, tiles, objects)
  return {
    __type = 'Level',
    width = w,
    height = h,
    -- indexed list of tiles, Object[x + width * y]
    tiles = tiles or {},
    -- simple list of game objects, Object[]
    objects = objects or {},
  }
end

function state.Game()
  return {
    __type = 'Game',
    levels = {},
    player_object_id = nil,
  }
end

function state.Glyph(index, r, g, b)
  return { 
    __type = 'Glyph',
    color = { r = r or 0xFF, g = g or 0xFF, b = b or 0xFF },
    index = index or 8,
  }
end
function state.TileGlyph(index, fg_r, fg_g, fg_b, bg_r, bg_g, bg_b)
  return { 
    __type = 'TileGlyph',
    color = { r = fg_r or 0x33, g = fg_g or 0x33, b = fg_b or 0x33 },
    bgcolor = { r = bg_r or 0x00, g = bg_g or 0x00, b = bg_b or 0x22 },
    index = index or 7,
  }
end

return state

