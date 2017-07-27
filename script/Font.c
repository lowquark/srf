
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>

int lapi_Font(lua_State * L) {
  lua_pushnil(L);
  return 1;
}

int lapi_open_Font(lua_State * L) {
  lua_pushcfunction(L, lapi_Font);
  return 1;
}

