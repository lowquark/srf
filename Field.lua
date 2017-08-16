
Field_mt = { __index = {} }

function Field_mt.__index.set(self, x, y, val)
  local x_idx = x - self._xmin
  local y_idx = y - self._ymin

  local w = self._w
  local h = self._h

  if x_idx >= 0 and x_idx < w and
     y_idx >= 0 and y_idx < h then
    local idx = x_idx + y_idx * w
    self[idx] = val
  else
    error(string.format('cannot set invalid Field index (%d, %d)', x, y))
  end
end
function Field_mt.__index.get(self, x, y)
  local x_idx = x - self._xmin
  local y_idx = y - self._ymin

  local w = self._w
  local h = self._h

  if x_idx >= 0 and x_idx < w and
     y_idx >= 0 and y_idx < h then
    local idx = x_idx + y_idx * w
    return self[idx]
  else
    return self._default
  end
end
function Field_mt.__index.each(self, fn)
  local idx = 0
  for y=self._ymin,self._ymax do
    for x=self._xmin,self._xmax do
      fn(x, y, self[idx])
      idx = idx + 1
    end
  end
end

return function(xa, ya, xb, yb, default)
  local xmin = math.min(xa, xb)
  local xmax = math.max(xa, xb)
  local ymin = math.min(ya, yb)
  local ymax = math.max(ya, yb)

  local w = xmax - xmin + 1
  local h = ymax - ymin + 1

  local f = {}
  for i=0,w*h-1 do
    f[i] = default
  end

  return setmetatable({
    _default = default,
    _xmin = xmin,
    _xmax = xmax,
    _ymin = ymin,
    _ymax = ymax,
    _w = w,
    _h = h,
  }, Field_mt)
end

