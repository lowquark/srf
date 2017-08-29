
#include "LuaEventEmitter.hpp"
#include <script/script_private.hpp>

#include <stdlib.h>

namespace script {
  void LuaEventEmitter::clear() {
    for(auto & ref : listeners) {
      ref.clear();
    }
  }
  void LuaEventEmitter::add_listener(LuaRef && fn) {
    listeners.push_back((LuaRef &&)fn);
  }
  void LuaEventEmitter::remove_listener(const LuaRef & fn) {
    for(auto it = listeners.begin() ;
        it != listeners.end() ; ) {
      if(*it == fn) {
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
  void LuaEventEmitter::emit(const BaseLuaTuple & args) {
    for(auto & ref : listeners) {
      lua_State * L = ref.push();
      if(L) {
        int nargs = args.push(L);
        pcall(L, nargs, 0);
      }
    }
  }
}

