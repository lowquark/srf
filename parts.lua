
local parts = {}
local mp = require 'MessagePack'

function parts.BasicGlyph(glyph)
  local p = {}
  p.type = 'BasicGlyph'
  function p:message(op, result)
    if op == 'glyph' then
      result.color = glyph.color
      result.index = glyph.index
    end
  end
  function p:save()
    return mp.pack(glyph)
  end
  function p:load(str)
    glyph = mp.unpack(str)
  end
  return p
end

function parts.BasicTileGlyph(tile_glyph)
  local p = {}
  p.type = 'BasicTileGlyph'
  function p:message(op, result)
    if op == 'tile_glyph' then
      result.color = tile_glyph.color
      result.bgcolor = tile_glyph.bgcolor
      result.index = tile_glyph.index
    end
  end
  function p:save()
    return mp.pack(tile_glyph)
  end
  function p:load(str)
    tile_glyph = mp.unpack(str)
  end
  return p
end

function parts.Bad()
  local p = {}
  p.type = 'Bad'
  function p:message(op, result)
    if op == 'bad' then
      result.bad = true
    end
  end
  return p
end

return parts

