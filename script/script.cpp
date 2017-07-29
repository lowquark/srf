
#include "script.hpp"
#include "script_private.hpp"

#include <cassert>
#include <queue>

#include <SDL2/SDL.h>
#include <lua5.1/lua.hpp>

namespace script {
  namespace input {
    int luaopen(lua_State * L);
    void clear_lua_refs();
  }
  namespace gfx {
    int luaopen(lua_State * L);
    void clear_lua_refs();
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

  // event queue and safety mechanism
  static std::queue<Event *> events;
  static SDL_mutex * event_queue_lock = NULL;
  static SDL_cond * event_queue_signal = NULL;

  // thread safe event enqueuing
  void emit(Event * event) {
    SDL_LockMutex(event_queue_lock);
    events.push(event);
    SDL_CondSignal(event_queue_signal);
    SDL_UnlockMutex(event_queue_lock);
  }

  static bool task_run = false;
  struct StopEvent : public Event {
    void emit() override { task_run = false; }
  };

  static luaL_Reg module_loaders[] = {
    { "input", input::luaopen },
    {   "gfx", gfx::luaopen   },
    { "timer", timer::luaopen },
    { "image", image::luaopen },
    {  "font", font::luaopen  },
    { NULL, NULL },
  };

  static int task_fn(void * _) {
    const char * script_name = (const char *)_;

    printf("script: starting with script `%s`\n", script_name);

    lua_State * L = luaL_newstate();
    luaL_openlibs(L);

    lua_getglobal(L, "package");
      lua_getfield(L, -1, "preload");
        luaL_register(L, NULL, module_loaders);
        lua_pop(L, 2);

    if(luaL_dofile(L, script_name)) {
      printf("%s\n", lua_tostring(L, -1));
      lua_pop(L, 1);
    }

    task_run = true;
    while(task_run) {
      SDL_LockMutex(event_queue_lock);
      while(events.size() == 0) {
        SDL_CondWait(event_queue_signal, event_queue_lock);
      }
      SDL_UnlockMutex(event_queue_lock);

      Event * event = events.front();
      events.pop();
      event->emit();
      delete event;
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

    event_queue_lock = SDL_CreateMutex();
    event_queue_signal = SDL_CreateCond();

    thread = SDL_CreateThread(task_fn, "script", (void *)"main.lua");
  }
  void deinit() {
    emit(new StopEvent);

    SDL_WaitThread(thread, NULL);
    thread = NULL;

    SDL_DestroyCond(event_queue_signal);
    event_queue_signal = NULL;
    SDL_DestroyMutex(event_queue_lock);
    event_queue_lock = NULL;

    while(events.size()) {
      delete events.front();
      events.pop();
    }
  }
}

