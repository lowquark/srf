
local object = {}

function object.create()
  return { }
end

local function pack_noempty(...)
  if select('#', ...) == 0 then
    return true
  else
    return {...}
  end
end

function object.save(o)
  if o.save then
    local state = { }
    for k,fn in pairs(o.save) do
      state[k] = pack_noempty(fn())
    end
    return state
  else
    return nil
  end
end
function object.load(ostate, part_fn)
  local o = object.create()
  for pname,pstate in pairs(ostate) do
    local ctor = part_fn(pname)
    if ctor then
      if type(pstate) == 'table' then
        ctor(o, unpack(pstate))
      else
        ctor(o)
      end
    else
      print('warning: part `'..pname..'` not found')
    end
  end
  return o
end

function object.message(o, ...)
  if o.message then
    for _,fn in pairs(o.message) do
      fn(...)
    end
  end
end

return object

