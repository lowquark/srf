
Map_mt = { __index = {} }

function Map_mt.__index.contains(self, i, j)
  local w = self._w
  local h = self._h
  return i >= 0 and i < w and j >= 0 and j < h
end
function Map_mt.__index.get(self, i, j)
  local w = self._w
  local h = self._h

  if i >= 0 and i < w and
     j >= 0 and j < h then
    return self[i + j * w]
  else
    return nil
  end
end
function Map_mt.__index.set(self, i, j, val)
  local w = self._w
  local h = self._h

  if i >= 0 and i < w and
     j >= 0 and j < h then
    self[i + j * w] = val
  else
    error(string.format('cannot set invalid Map index (%d, %d)', i, j))
  end
end
function Map_mt.__index.each(self, fn)
  local idx = 0
  for j=0,self._h-1 do
    for i=0,self._w-1 do
      fn(i, j, self[idx])
      idx = idx + 1
    end
  end
end
function Map_mt.__index.map(self, fn)
  for idx=0,self._w*self._h-1 do
    self[idx] = fn(self[idx])
  end
end
function Map_mt.__index.copy(self, fn)
  local w, h = self._w, self._h

  local m = {
    _w = w,
    _h = h,
  }

  for idx=0,w*h-1 do
    m[idx] = self[idx]
  end

  return setmetatable(m, Map_mt)
end

return function(w, h, fill)
  local m = {
    _w = w,
    _h = h,
  }

  if fill then
    for idx=0,w*h-1 do
      m[idx] = fill
    end
  end

  return setmetatable(m, Map_mt)
end

