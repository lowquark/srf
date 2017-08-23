
function EventEmitter ()
  local listeners = {}

  function on(self, name, cb)
    listeners[name] = listeners[name] or {}
    table.insert(listeners[name], cb)
  end
  function emit(self, name, ...)
    local cb_list = listeners[name]
    if cb_list then
      for _,cb in ipairs(cb_list) do
        cb(...)
      end
    end
  end

  return {
    on = on,
    emit = emit,
  };
end

return EventEmitter

