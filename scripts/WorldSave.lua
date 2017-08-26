
local mp = require 'MessagePack'

local base64 = {}

-- character table string
local b='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_'

-- encoding
function base64.encode(data)
    return ((data:gsub('.', function(x)
        local r,b='',x:byte()
        for i=8,1,-1 do r=r..(b%2^i-b%2^(i-1)>0 and '1' or '0') end
        return r;
    end)..'0000'):gsub('%d%d%d?%d?%d?%d?', function(x)
        if (#x < 6) then return '' end
        local c=0
        for i=1,6 do c=c+(x:sub(i,i)=='1' and 2^(6-i) or 0) end
        return b:sub(c+1,c+1)
    end)..({ '', '==', '=' })[#data%3+1])
end

-- decoding
function base64.decode(data)
    data = string.gsub(data, '[^'..b..'=]', '')
    return (data:gsub('.', function(x)
        if (x == '=') then return '' end
        local r,f='',(b:find(x)-1)
        for i=6,1,-1 do r=r..(f%2^i-f%2^(i-1)>0 and '1' or '0') end
        return r;
    end):gsub('%d%d%d?%d?%d?%d?%d?%d?', function(x)
        if (#x ~= 8) then return '' end
        local c=0
        for i=1,8 do c=c+(x:sub(i,i)=='1' and 2^(8-i) or 0) end
        return string.char(c)
    end))
end

function filename_encode(str)
  str = string.gsub(str, '([^%w%-%.%_ ])', function(c)
    return string.format('%%%02X', string.byte(c))
  end)
  str = string.gsub(str, ' ', '+')
  return str
end
function filename_decode(str)
  str = string.gsub(str, '+', ' ')
  str = string.gsub(str, '%%(%x%x)', function(d)
    return string.char(tonumber(d, 16))
  end)
  return str
end

return function(name)
  local meta_filename = 'world/'..filename_encode(name)

  local function page_filename(page_name)
    return 'world/'..filename_encode(name..'.'..page_name)
  end

  local function read(filename)
    local f = io.open(filename, 'r')

    if f then
      local data = mp.unpack(f:read('*a'))
      f:close()
      return data
    else
      return nil
    end
  end

  local function write(filename, data)
    local f = io.open(filename, 'w')
    if f then
      f:write(mp.pack(data))
      f:close()
      return true
    else
      return false
    end
  end

  local function meta_add_page(page_name)
    local meta = read(meta_filename)
    if not meta then
      meta = { pages = {} }
    end

    for k,v in ipairs(meta.pages) do
      if v == page_name then
        return
      end
    end

    table.insert(meta.pages, page_name)
    write(meta_filename, meta)
  end
  local function meta_remove_page(page_name)
    local meta = read(meta_filename)

    if meta then
      local idx = nil
      for i,name in ipairs(meta.pages) do
        if name == page_name then
          idx = i
          break
        end
      end

      if idx then
        table.remove(meta.pages, idx)
        write(meta_filename, meta)
      end
    end
  end

  local ws = {}

  function ws:read_page(page_name)
    return read(page_filename(page_name))
  end
  function ws:write_page(page_name, data)
    if write(page_filename(page_name), data) then
      meta_add_page(page_name)
      return true
    else
      return false
    end
  end
  function ws:delete_page(page_name)
    os.remove(page_filename(page_name))
    meta_remove_page(page_name)
  end
  function ws:delete()
    local meta = read(meta_filename)
    if meta then
      for i,page_name in ipairs(meta.pages) do
        os.remove(page_filename(page_name))
      end
      os.remove(meta_filename)
    end
  end

  return ws
end

