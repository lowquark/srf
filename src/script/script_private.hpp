#ifndef SCRIPT_PRIVATE_HPP
#define SCRIPT_PRIVATE_HPP

#include <lua5.1/lua.hpp>
#include <utility>
#include <functional>

namespace script {
  struct Action {
    public:
    virtual ~Action() = default;
    virtual void operator()() {}
  };

  template <typename T, typename ... Args>
  void emit(Args ... args) {
    extern void emit(Action * event);
    emit(new T(std::forward<Args>(args)...));
  }

  extern void pcall(lua_State * L, int narg, int nresult);
  extern void defer(std::function<void()> && action);
}

#endif
