
return function(o, color, index)
  o.message = o.message or {}

  function o.message.Glyph(op, result)
    if op == 'glyph' then
      result.color = color
      result.index = index
    end
  end

  local p = {}

  function p:save()
    return color, index
  end

  return p
end

