#ifndef SCRIPT_CALL_HPP
#define SCRIPT_CALL_HPP

#include <script/luaref.hpp>
#include <util/AsyncContext.hpp>

namespace script {
  class BaseLuaTuple {
    public:
    virtual int push(lua_State * L) const { return 0; };
  };

  template <typename ... T>
  class LuaTuple : public BaseLuaTuple {
  };

  template <>
  class LuaTuple <> : public BaseLuaTuple {
  };
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

  template <typename ... TArgs>
  class Call : public AsyncContext::Action {
    LuaTuple<typename std::decay<TArgs>::type ...> args;
    LuaRef fn;

    public:
    Call(LuaRef && fn, TArgs ... args)
      : args(std::forward<TArgs...>(args...))
      , fn((LuaRef &&)fn) {}

    void operator()() override {
      lua_State * L = fn.push();
      if(L) {
        int nargs = args.push(L);
        pcall(L, nargs, 0);
        fn.clear();
      }
    }
  };
  template <>
  class Call <> : public AsyncContext::Action {
    LuaRef fn;

    public:
    Call(LuaRef && fn)
      : fn((LuaRef &&)fn) {}

    void operator()() override {
      lua_State * L = fn.push();
      if(L) {
        pcall(L, 0, 0);
        fn.clear();
      }
    }
  };
}

#endif
