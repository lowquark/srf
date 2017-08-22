
local module = {}

local current_module = nil
function module.next(m, ...)
  if current_module then
    current_module:deinit()
  end
  current_module = m
  if current_module then
    current_module:init(...)
  else
    -- exit
  end
end

return module

