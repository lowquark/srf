
#include "luaref.hpp"

namespace script {
  LuaRef::LuaRef(lua_State * L, int idx) {
    set(L, idx);
  }
  LuaRef::LuaRef(const LuaRef & other) {
    if(other.L) {
      lua_rawgeti(other.L, LUA_REGISTRYINDEX, other.i);
      this->i = luaL_ref(other.L, LUA_REGISTRYINDEX);
      this->L = other.L;
    }
  }
  LuaRef & LuaRef::operator=(const LuaRef & other) {
    if(this != &other) {
      clear();

      if(other.L) {
        lua_rawgeti(other.L, LUA_REGISTRYINDEX, other.i);
        this->i = luaL_ref(other.L, LUA_REGISTRYINDEX);
        this->L = other.L;
      }
    }

    return *this;
  }
  LuaRef::LuaRef(LuaRef && other) {
    this->i = other.i;
    this->L = other.L;
    other.i = LUA_NOREF;
    other.L = NULL;
  }
  LuaRef & LuaRef::operator=(LuaRef && other) {
    if(this != &other) {
      clear();

      this->i = other.i;
      this->L = other.L;
      other.i = LUA_NOREF;
      other.L = NULL;
    }

    return *this;
  }
  LuaRef::~LuaRef() {
    clear();
  }

  void LuaRef::clear() {
    if(L) {
      luaL_unref(L, LUA_REGISTRYINDEX, i);
      i = LUA_NOREF;
      L = NULL;
    }
  }
  void LuaRef::set(lua_State * L, int idx) {
    clear();

    lua_pushvalue(L, idx);
    this->i = luaL_ref(L, LUA_REGISTRYINDEX);
    this->L = L;
  }

  lua_State * LuaRef::push() const {
    if(L) {
      lua_rawgeti(L, LUA_REGISTRYINDEX, i);
      return L;
    } else {
      return NULL;
    }
  }

  bool LuaRef::equal(lua_State * L, int idx) {
    if(this->L == L) {
      lua_pushvalue(L, idx);
      push();
      if(lua_equal(L, -1, -2)) {
        lua_pop(L, 2);
        return true;
      } else {
        lua_pop(L, 2);
        return false;
      }
    } else {
      return false;
    }
  }
  bool LuaRef::operator==(const LuaRef & other) {
    if(L == other.L) {
      other.push();
      push();
      if(lua_equal(L, -1, -2)) {
        lua_pop(L, 2);
        return true;
      } else {
        lua_pop(L, 2);
        return false;
      }
    }
    return false;
  }
}

