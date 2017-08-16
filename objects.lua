
local objects = {}

function objects.NiceTree()
  return {
    hp = 20,
    df = 20,
    color = { r = 0x33, g = 0x66, b = 0x33 },
    glyph = 6 + 4*16,
  }
end
function objects.MeanTree()
  return {
    hp = 30,
    df = 30,
    color = { r = 0x22, g = 0x55, b = 0x33 },
    glyph = 5 + 4*16,
  }
end

return objects

