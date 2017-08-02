#ifndef EVENT_H
#define EVENT_H

#include <script/luaref.hpp>
#include <vector>

namespace script {
  struct LuaPusher {
    virtual int push(lua_State * L) const { return 0; };
  };

  class LuaEventEmitter {
    public:
    void clear();
    void add_listener(lua_State * L, int idx);
    void remove_listener(lua_State * L, int idx);
    void emit();
    void emit(const LuaPusher & pusher);

    private:
    std::vector<LuaRef> listeners;
  };
}

#endif
