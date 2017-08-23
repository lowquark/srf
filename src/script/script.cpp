
#include "script.hpp"
#include "script_private.hpp"

#include <cassert>
#include <queue>
#include <mutex>
#include <condition_variable>

#include <SDL2/SDL.h>
#include <lua5.1/lua.hpp>

namespace script {
  namespace input {
    int luaopen(lua_State * L);
    void clear_lua_refs();
    bool handle_sdl_event(const SDL_Event * event);
  }
  namespace gfx {
    int luaopen(lua_State * L);
    void clear_lua_refs();
    bool handle_sdl_event(const SDL_Event * event);
  }
  namespace timer {
    int luaopen(lua_State * L);
    void clear_lua_refs();
  }
  namespace image {
    int luaopen(lua_State * L);
    void clear_lua_refs();
  }
  namespace font {
    int luaopen(lua_State * L);
    void clear_lua_refs();
  }

  // action queue and safety mechanism
  static std::queue<Action *> actions;
  std::mutex actions_lock;
  std::condition_variable actions_cv;

  // thread safe action enqueuing
  void emit(Action * action) {
    std::unique_lock<std::mutex> lk(actions_lock);
    actions.push(action);
    lk.unlock();
    actions_cv.notify_one();
  }

  static bool task_run = false;
  struct StopAction : public Action {
    void operator()() override { task_run = false; }
  };

  static luaL_Reg module_loaders[] = {
    { "srf.input", input::luaopen },
    { "srf.gfx",   gfx::luaopen   },
    { "srf.timer", timer::luaopen },
    { "srf.image", image::luaopen },
    { "srf.font",  font::luaopen  },
    { NULL, NULL },
  };

  static int task_fn(void * _) {
    const char * script_name = (const char *)_;

    printf("script: starting with script `%s`\n", script_name);

    lua_State * L = luaL_newstate();
    luaL_openlibs(L);

    lua_getglobal(L, "package");
      lua_pushstring(L, "./scripts/?.lua");
        lua_setfield(L, -2, "path");
      lua_pushstring(L, "");
        lua_setfield(L, -2, "cpath");

      lua_getfield(L, -1, "preload");
        luaL_register(L, NULL, module_loaders);
        lua_pop(L, 2);

    lua_getglobal(L, "require");
    lua_pushstring(L, "main");
    lua_call(L, 1, 0);

    task_run = true;
    while(task_run) {
      std::unique_lock<std::mutex> lk(actions_lock);
      while(actions.empty()) {
        actions_cv.wait(lk);
      }
      lk.unlock();

      Action * action = actions.front();
      actions.pop();
      (*action)();
      delete action;
    }

    printf("script: stopping\n");

    input::clear_lua_refs();
    gfx::clear_lua_refs();
    timer::clear_lua_refs();
    image::clear_lua_refs();
    font::clear_lua_refs();

    lua_close(L);
    L = NULL;

    return 0;
  }

  SDL_Thread * thread = NULL;

  void init() {
    assert(thread == NULL);

    thread = SDL_CreateThread(task_fn, "script", (void *)"main.lua");
  }
  void deinit() {
    emit(new StopAction);

    SDL_WaitThread(thread, NULL);
    thread = NULL;

    while(actions.size()) {
      delete actions.front();
      actions.pop();
    }
  }

  bool handle_sdl_event(const SDL_Event * event) {
    if(input::handle_sdl_event(event)) { return true; }
    else if(gfx::handle_sdl_event(event)) { return true; }
    else { return false; }
  }
}

