#ifndef LUA_EVENTEMITTER_H
#define LUA_EVENTEMITTER_H

#include <script/LuaTuple.hpp>
#include <script/LuaRef.hpp>
#include <vector>

namespace script {
  class LuaEventEmitter {
    public:
    void clear();
    void add_listener(LuaRef && fn);
    void remove_listener(const LuaRef & fn);
    void emit();
    void emit(const BaseLuaTuple & args);

    private:
    std::vector<LuaRef> listeners;
  };
}

#endif
