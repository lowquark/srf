
local Object_mt = { __index = {} }

function Object_mt.__index.message(self, op, result)
  result = result or {}

  for i,v in ipairs(self) do
    if v.message then
      v:message(op, result)
    end
  end

  return result
end

function Object_mt.__index.attach(self, p)
  self:remove(p.type)

  table.insert(self, p)
  self[p.type] = p
end
function Object_mt.__index.remove(self, type)
  local toremove = self[type]
  if toremove then
    self[type] = nil

    local toremove_idx
    for i,p in ipairs(self) do
      if p == toremove then
        toremove_idx = i
        break
      end
    end
    if toremove_idx then
      table.remove(self, toremove_idx)
    end
  end
end

return function(...)
  local o = {...}
  for i,p in ipairs(o) do
    o[p.type] = p
  end
  return setmetatable(o, Object_mt)
end

