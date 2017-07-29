
#include "event.hpp"

#include <stdlib.h>

namespace script {
  void LuaEventEmitter::clear() {
    for(auto & ref : listeners) {
      ref.clear();
    }
  }
  void LuaEventEmitter::add_listener(lua_State * L, int idx) {
    listeners.push_back(LuaRef(L, idx));
  }
  void LuaEventEmitter::emit() {
    for(auto & ref : listeners) {
      lua_State * L = ref.push();
      if(L) {
        lua_call(L, 0, 0);
      }
    }
  }
  void LuaEventEmitter::emit(const LuaPusher & pusher) {
    for(auto & ref : listeners) {
      lua_State * L = ref.push();
      if(L) {
        int nargs = pusher.push(L);
        lua_call(L, nargs, 0);
      }
    }
  }
}

