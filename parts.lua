
local parts = {}
local mp = require 'MessagePack'

function parts.BasicGlyph(state)
  local p = {}

  assert(state[1] == 'BasicGlyph')
  local glyph = state[2]

  function p:message(op, result)
    if op == 'glyph' then
      result.color = glyph.color
      result.index = glyph.index
    end
  end
  function p:save_state()
    return { 'BasicGlyph', glyph }
  end
  return p
end

function parts.BasicTileGlyph(state)
  local p = {}

  assert(state[1] == 'BasicTileGlyph')
  local tile_glyph = state[2]

  function p:message(op, result)
    if op == 'tile_glyph' then
      result.color = tile_glyph.color
      result.bgcolor = tile_glyph.bgcolor
      result.index = tile_glyph.index
    end
  end
  function p:save_state()
    return { 'BasicTileGlyph', tile_glyph }
  end
  return p
end

function parts.Bad(state)
  local p = {}
  assert(state[1] == 'Bad')
  function p:message(op, result)
    if op == 'bad' then
      result.bad = true
    end
  end
  function p:save_state()
    return { 'Bad' }
  end
  return p
end

function parts.Part(state)
  local ctor = parts[state[1]]

  if ctor then
    local p = ctor(state)
    return p
  end

  return nil
end

return parts

