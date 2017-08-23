
local Object_mt = { __index = {} }

function Object_mt.__index:attach(p)
  table.insert(self, p)
end
function Object_mt.__index:notify(...)
  for i,v in ipairs(self) do
    if v.notify then
      v:notify(...)
    end
  end
end
function Object_mt.__index:query(...)
  local results = {}

  for i,v in ipairs(self) do
    if v.query then
      table.insert(results, v:query(...))
    end
  end

  return results
end
function Object_mt.__index:remove(...)
  local to_remove = {}

  for i,v in ipairs(self) do
    if v.remove then
      if v:remove(...) then
        table.insert(to_remove, i)
      end
    end
  end

  for k,i in ipairs(to_remove) do
    table.remove(self, i)
  end
end
function Object_mt.__index:save_state()
  local state = {}

  for i,p in ipairs(self) do
    state[i] = p:save_state()
  end

  return state
end

return function(state)
  if state then
    local o = {}
    for i,s in ipairs(state) do
      o[i] = parts.Part(s)
    end
    return setmetatable(o, Object_mt)
  else
    return setmetatable({}, Object_mt)
  end
end

