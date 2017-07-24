#ifndef LUAREF_H
#define LUAREF_H

#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>

typedef struct luaref {
  int i;
  lua_State * L;
} luaref_t;

void luaref_clear(luaref_t * ref);
void luaref_set(luaref_t * ref, lua_State * L, int idx);

void luaref_copy(luaref_t * dst, const luaref_t * src);

lua_State * luaref_push(const luaref_t * ref);

#endif
