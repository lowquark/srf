
local tiles = {}

function tiles.Grass()
  return {
    color = { r = 0x22, g = 0x55, b = 0x33 },
    bgcolor = { r = 0x11, g = 0x22, b = 0x22 },
    glyph = 5 + 6*16,
  }
end
function tiles.Path()
  return {
    color = { r = 0x44, g = 0x44, b = 0x22 },
    bgcolor = { r = 0x11, g = 0x22, b = 0x22 },
    glyph = 5 + 6*16,
  }
end
function tiles.Badness()
  return {
    color = { r = 0xBB, g = 0x22, b = 0x11 },
    bgcolor = { r = 0x88, g = 0x44, b = 0x22 },
    glyph = 9 + 7*16,
    bad = true,
  }
end

return tiles

