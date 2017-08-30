
local object = {}

function object.create()
  return { __parts = {} }
end

function object.load(ostate, load_part)
  if ostate.parts then
    local o = object.create()
    for i,pstate in ipairs(ostate.parts) do
      table.insert(o.__parts, load_part(o, pstate))
    end
    return o
  else
    error 'invalid object state'
  end
end
function object.save(o, save_part)
  local state = { parts = {} }
  for i,p in ipairs(o.__parts) do
    table.insert(state.parts, save_part(o, p))
  end
  return state
end

function object.message(o, op, ...)
  if o.message then
    for ptype,fn in pairs(o.message) do
      fn(op, ...)
    end
  end
end

return object

