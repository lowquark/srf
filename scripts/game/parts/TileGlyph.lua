
return function(o, color, bgcolor, index)
  o.message = o.message or {}

  function o.message.TileGlyph(op, result)
    if op == 'tile_glyph' then
      result.color = color
      result.bgcolor = bgcolor
      result.index = index
    end
  end

  local p = {}

  function p:save()
    return color, bgcolor, index
  end

  return p
end

