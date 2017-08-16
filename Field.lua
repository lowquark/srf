
Field_mt = { __index = {} }

local Map = require 'Map'

function Field_mt.__index.contains(self, x, y)
  return self._map:contains(x - self._xmin, y - self._ymin)
end
function Field_mt.__index.get(self, x, y)
  return self._map:get(x - self._xmin, y - self._ymin)
end
function Field_mt.__index.set(self, x, y, val)
  self._map:set(x - self._xmin, y - self._ymin, val)
end

function Map_mt.__index.each(self, fn)
  self._map:each(function(i, j, val)
    fn(self._xmin + i,
       self._ymin + j,
       val)
  end)
end
function Field_mt.__index.map(self, fn)
  self._map:map(fn)
end
function Field_mt.__index.copy(self)
  local f = {
    _xmin = self._xmin,
    _ymin = self._ymin,
    _map = self._map:copy(),
  }

  return setmetatable(f, Field_mt)
end

return function(xa, ya, xb, yb, fill)
  local xmin = math.min(xa, xb)
  local xmax = math.max(xa, xb)
  local ymin = math.min(ya, yb)
  local ymax = math.max(ya, yb)

  local w = xmax - xmin + 1
  local h = ymax - ymin + 1

  local f = {
    _xmin = xmin,
    _ymin = ymin,
    _map = Map(w, h, fill),
  }

  return setmetatable(f, Field_mt)
end

