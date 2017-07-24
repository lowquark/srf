
#include "luaref.h"

#include <assert.h>

void luaref_clear(luaref_t * ref) {
  if(ref->L) {
    luaL_unref(ref->L, LUA_REGISTRYINDEX, ref->i);
    ref->L = NULL;
    ref->i = LUA_NOREF;
  }
}
void luaref_set(luaref_t * ref, lua_State * L, int idx) {
  luaref_clear(ref);

  lua_pushvalue(L, idx);
  ref->i = luaL_ref(L, LUA_REGISTRYINDEX);
  ref->L = L;
}

void luaref_copy(luaref_t * dst, const luaref_t * src) {
  luaref_clear(dst);

  if(src->L) {
    lua_rawgeti(src->L, LUA_REGISTRYINDEX, src->i);
    dst->i = luaL_ref(src->L, LUA_REGISTRYINDEX);
    dst->L = src->L;
  }
}

lua_State * luaref_push(const luaref_t * ref) {
  if(ref->L) {
    lua_rawgeti(ref->L, LUA_REGISTRYINDEX, ref->i);
    return ref->L;
  } else {
    return NULL;
  }
}

