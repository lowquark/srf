
local mp = require 'MessagePack'

local gamesaves = {}

function gamesaves.save(name, gamestate)
  print('saving `'..name..'`...')
  local f = io.open('saves/' .. name, 'w')
  if f then
    f:write(mp.pack(gamestate))
    f:close()
    return true
  else
    return false
  end
end
function gamesaves.load(name)
  print('loading `'..name..'`...')
  local f = io.open('saves/' .. name, 'r')
  if f then
    local gamestate = mp.unpack(f:read('*a'))
    f:close()

    return gamestate
  end

  return nil
end
function gamesaves.delete(name)
  print('ahahaha permadeath!')
  print('deleting `'..name..'`...')

  os.remove('saves/' .. name)
end

return gamesaves

