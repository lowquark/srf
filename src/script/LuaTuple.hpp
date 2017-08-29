#ifndef SCRIPT_LUATUPLE_HPP
#define SCRIPT_LUATUPLE_HPP

#include <lua5.1/lua.hpp>
#include <script/LuaRef.hpp>
#include <string>

namespace script {
  class BaseLuaTuple {
    public:
    virtual int push(lua_State * L) const { return 0; };
  };

  template <typename ... T>
  class LuaTuple;

  // In the future, will need to come up with some variadic metawitchcraft to
  // handle any number of arguments. For now, just hardcode ;)
  template <>
  class LuaTuple <bool> : public BaseLuaTuple {
    bool b;
    public:
    LuaTuple(bool b) : b(b) {}
    int push(lua_State * L) const override {
      lua_pushboolean(L, b);
      return 1;
    };
  };
  template <>
  class LuaTuple <int> : public BaseLuaTuple {
    int i;
    public:
    LuaTuple(int i) : i(i) {}
    int push(lua_State * L) const override {
      lua_pushinteger(L, i);
      return 1;
    };
  };
  template <>
  class LuaTuple <nullptr_t> : public BaseLuaTuple {
    public:
    LuaTuple(nullptr_t) {}
    int push(lua_State * L) const override {
      lua_pushnil(L);
      return 1;
    };
  };
  template <>
  class LuaTuple <std::string> : public BaseLuaTuple {
    std::string str;
    public:
    LuaTuple(const std::string & str) : str(str) {}
    LuaTuple(std::string && str) : str((std::string &&)str) {}
    int push(lua_State * L) const override {
      lua_pushlstring(L, str.c_str(), str.size());
      return 1;
    };
  };
}

#endif
