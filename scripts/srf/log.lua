
local log = {}

local log_data_file = io.open('srf.log.data', 'w')
local log_file = io.open('srf.log', 'w')

local function percent_encode(str)
  return (string.gsub(str, '([;,%%])', function (c)
    return string.format('%%%02X', string.byte(c))
  end))
end
local function encode_tuple(...)
  local str = ''
  local n = select('#', ...)
  for i=1,n do
    local val = select(i, ...)
    if val then
      str = str..percent_encode(val)
    else
      str = str..'%'
    end
    if i ~= n then
      str = str..','
    end
  end
  str = str..';'
  return str
end

local function log_data(type, timestr, topic, str)
  log_data_file:write(encode_tuple(type, timestr, topic, str)..'\n')
end

local function _log(topic, str)
  local line
  if topic then
    line = os.date('%H:%M:%S')..' ['..topic..'] '..str
  else
    line = os.date('%H:%M:%S')..' '..str
  end
  log_file:write(line..'\r\n')
  print(line)
  log_data('log', os.time(), topic, str)
  return line
end
local function _warn(topic, str)
  local file_line, term_line
  if topic then
    file_line = os.date('%H:%M:%S')..' ['..topic..'] WARNING: '..str
    term_line = '\x1b[33m'..
                os.date('%H:%M:%S')..
                ' ['..
                topic..
                '] \x1b[33;1mWARNING:\x1b[0;33m '..
                str..
                '\x1b[0m'
  else
    file_line = os.date('%H:%M:%S')..' WARNING: '..str
    term_line = '\x1b[33m'..
                os.date('%H:%M:%S')..
                ' \x1b[33;1mWARNING:\x1b[0;33m '..
                str..
                '\x1b[0m'
  end
  log_file:write(file_line..'\r\n')
  print(term_line)
  log_data('warn', os.time(), topic, str)
  return line
end
local function _format(topic, fmt, ...)
  return _log(topic, string.format(fmt, ...))
end

local function Log(topic)
  local log = function(...)
    _log(topic, ...)
  end
  local warn = function(...)
    _warn(topic, ...)
  end
  local format = function(...)
    _format(topic, ...)
  end
  local __call = function(t, ...)
    _log(topic, ...)
  end
  return setmetatable({
    log = log,
    warn = warn,
    format = format,
  }, {
    __call = __call,
  })
end

local topics = {}

function log.topic(name)
  local t = topics[name]
  if not t then
    t = Log(name)
    topics[name] = t
  end
  return t
end
function log.log(...)
  _log(nil, ...)
end
function log.warn(...)
  _warn(nil, ...)
end
function log.format(...)
  _format(nil, ...)
end

local function __call(t, ...)
  _log(nil, ...)
end

return setmetatable(log, { __call = __call })

