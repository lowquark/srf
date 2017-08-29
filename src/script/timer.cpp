
#include <script/script_private.hpp>
#include <script/luaref.hpp>

#include <SDL2/SDL_timer.h>
#include <vector>
#include <memory>

namespace script {
  namespace timer {
    class Timer;
    struct FireTimer : public AsyncContext::Action {
      Timer * timer;
      FireTimer(Timer * timer) : timer(timer) {}
      void operator()() override;
    };

    class Timer {
      public:
      ~Timer() {
        cancel();
      }

      // officially start the timer, looping or oneshot
      void set_looping(unsigned int T_ms) {
        loop = true;
        timer_id = SDL_AddTimer(T_ms, loop_callback, this);
      }
      void set_oneshot(unsigned int T_ms) {
        loop = false;
        timer_id = SDL_AddTimer(T_ms, oneshot_callback, this);
      }

      void set_callback(lua_State * L, int idx) {
        callback.set(L, idx);
      }
      void clear_callback() {
        callback.clear();
      }

      void cancel() {
        // try to prevent the interrupt from firing (but it's possible it already has)
        SDL_RemoveTimer(timer_id);
        timer_id = 0;
        clear_callback();
      }

      bool canceled() { return timer_id == 0; }

      void fire() {
        if(!canceled()) {
          lua_State * L = callback.push();
          if(L) {
            pcall(L, 0, 0);
          }
        }
      }

      private:
      LuaRef callback;
      Uint32 timer_id = 0;
      bool loop = false;

      // ISR, should not access member fields
      static Uint32 loop_callback(Uint32 interval, void * param) {
        script::actx->enqueue(new FireTimer((Timer *)param));
        return interval; // call again
      }
      // ISR, should not access member fields
      static Uint32 oneshot_callback(Uint32 interval, void * param) {
        script::actx->enqueue(new FireTimer((Timer *)param));
        return 0; // do not call again
      }
    };

    std::vector<std::shared_ptr<Timer>> timers;
    void clear_lua_refs() {
      for(auto & t : timers) {
        t->clear_callback();
      }
      timers.clear();
    }

    struct LuaTimer {
      std::shared_ptr<Timer> timer;
    };

    void FireTimer::operator()() {
      timer->fire();
    }

    static int loop(lua_State * L) {
      int interval = luaL_checkint(L, 1);
      luaL_checktype(L, 2, LUA_TFUNCTION);

      if(interval < 0) { interval = 0; }

      LuaTimer * lua_timer = (LuaTimer *)lua_newuserdata(L, sizeof(*lua_timer));
        luaL_getmetatable(L, "timer");
          lua_setmetatable(L, -2);

      new (lua_timer) LuaTimer;

      auto timer = std::make_shared<Timer>();
      timer->set_callback(L, 2);
      timer->set_looping(interval);
      timers.push_back(timer);

      lua_timer->timer = timer;

      return 1;
    }
    static int oneshot(lua_State * L) {
      int interval = luaL_checkint(L, 1);
      luaL_checktype(L, 2, LUA_TFUNCTION);

      if(interval < 0) { interval = 0; }

      LuaTimer * lua_timer = (LuaTimer *)lua_newuserdata(L, sizeof(*lua_timer));
        luaL_getmetatable(L, "timer");
          lua_setmetatable(L, -2);

      new (lua_timer) LuaTimer;

      auto timer = std::make_shared<Timer>();
      timer->set_callback(L, 2);
      timer->set_looping(interval);
      timers.push_back(timer);

      lua_timer->timer = timer;

      return 1;
    }

    static int Timer_gc(lua_State * L) {
      LuaTimer * lua_timer = (LuaTimer *)luaL_checkudata(L, 1, "timer");
      lua_timer->~LuaTimer();
      return 0;
    }
    static int Timer_cancel(lua_State * L) {
      LuaTimer * lua_timer = (LuaTimer *)luaL_checkudata(L, 1, "timer");
      lua_timer->timer->cancel();
      return 0;
    }

    int luaopen(lua_State * L) {
      lua_newtable(L);

        lua_pushcfunction(L, loop);
          lua_setfield(L, -2, "loop");
        lua_pushcfunction(L, oneshot);
          lua_setfield(L, -2, "oneshot");

      if(luaL_newmetatable(L, "timer")) {
        lua_newtable(L);
          lua_pushcfunction(L, Timer_cancel);
            lua_setfield(L, -2, "cancel");
          lua_setfield(L, -2, "__index");

        // because the timer contains a ref to its userdata, this shouldn't
        // actually be called.
        lua_pushcfunction(L, Timer_gc);
          lua_setfield(L, -2, "__gc");

        lua_pop(L, 1);
      } else {
        lua_pop(L, 1);
      }

      return 1;
    }
  }
}

