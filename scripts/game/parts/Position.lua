
return function(o, x, y)
  local pos = { x = x, y = y }
  o.position = pos

  local p = {}

  function p:save()
    return pos.x, pos.y
  end

  return p
end

