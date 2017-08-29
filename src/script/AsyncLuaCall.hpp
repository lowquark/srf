#ifndef SCRIPT_CALL_HPP
#define SCRIPT_CALL_HPP

#include <script/LuaRef.hpp>
#include <script/LuaTuple.hpp>
#include <util/AsyncContext.hpp>

namespace script {
  template <typename ... TArgs>
  class AsyncLuaCall : public AsyncContext::Action {
    LuaTuple<typename std::decay<TArgs>::type ...> args;
    LuaRef fn;

    public:
    AsyncLuaCall(LuaRef && fn, TArgs ... args)
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
  class AsyncLuaCall <> : public AsyncContext::Action {
    LuaRef fn;

    public:
    AsyncLuaCall(LuaRef && fn)
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
