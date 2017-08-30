
local level = {}

function level.save(l, save_object)
  local tst = {}
  local ost = {}

  -- save all tiles
  for i=1,l.width*l.height do
    local t = l.tiles[i]
    if t then
      tst[i] = save_object(l, t)
    end
  end

  -- save all objects
  for i,o in ipairs(l.objects) do
    ost[i] = save_object(l, o)
  end

  return {
    width = l.width,
    height = l.height,
    -- indexed list of tiles, Object[x + width * y]
    tiles = tst,
    -- simple list of game objects, Object[]
    objects = ost,
  }
end
function level.load(lstate, load_object)
  if lstate.width and lstate.height and
     lstate.tiles and
     lstate.objects then
    local w = lstate.width
    local h = lstate.height

    local l = level.create(w, h)

    for i=1,w*h do
      local tstate = lstate.tiles[i]
      if tstate then
        l.tiles[i] = load_object(l, tstate)
      end
    end

    for i,ostate in ipairs(lstate.objects) do
      l.objects[i] = load_object(l, ostate)
    end

    return l
  else
    error 'invalid level state'
  end
end

function level.create(w, h)
  return {
    width = w,
    height = h,
    tiles = {},
    objects = {},
  }
end

return level

