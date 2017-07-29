#ifndef LUAREF_H
#define LUAREF_H

#include <lua5.1/lua.hpp>

namespace script {
  class LuaRef {
    public:
    LuaRef() = default;
    LuaRef(lua_State * L, int idx);
    LuaRef(const LuaRef & other);
    LuaRef & operator=(const LuaRef & other);
    LuaRef(LuaRef && other);
    LuaRef & operator=(LuaRef && other);
    ~LuaRef();

    void clear();
    void set(lua_State * L, int idx);

    lua_State * push() const;

    private:
    int i = LUA_NOREF;
    lua_State * L = NULL;
  };
}

#endif
