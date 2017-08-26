
#include "event.hpp"
#include <script/script_private.hpp>

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
  void LuaEventEmitter::remove_listener(lua_State * L, int idx) {
    for(auto it = listeners.begin() ;
        it != listeners.end() ; ) {
      if(it->equal(L, idx)) {
        it = listeners.erase(it);
      } else {
        it ++;
      }
    }
  }
  void LuaEventEmitter::emit() {
    for(auto & ref : listeners) {
      lua_State * L = ref.push();
      if(L) {
        pcall(L, 0, 0);
      }
    }
  }
  void LuaEventEmitter::emit(const LuaPusher & pusher) {
    for(auto & ref : listeners) {
      lua_State * L = ref.push();
      if(L) {
        int nargs = pusher.push(L);
        pcall(L, nargs, 0);
      }
    }
  }
}

