#ifndef SCRIPT_PRIVATE_HPP
#define SCRIPT_PRIVATE_HPP

#include <lua5.1/lua.hpp>
#include <utility>
#include <util/AsyncContext.hpp>

namespace script {
  extern AsyncContext * actx;
  extern void pcall(lua_State * L, int narg, int nresult);
}

#endif
