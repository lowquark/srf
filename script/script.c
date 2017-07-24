
#include "script.h"
#include "script_private.h"

#include <SDL2/SDL.h>
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
#include <assert.h>

#include <queue.h>

// event queue and safety mechanism
queue_t script_event_queue = { 0 };
SDL_mutex * script_event_queue_lock = NULL;
SDL_cond * script_event_queue_signal = NULL;

// thread safe event enqueuing
void script_emit(const script_event_t * event) {
  script_event_t * newevent = malloc(sizeof(*newevent));
  *newevent = *event;

  SDL_LockMutex(script_event_queue_lock);
  queue_push(&script_event_queue, newevent);
  SDL_CondSignal(script_event_queue_signal);
  SDL_UnlockMutex(script_event_queue_lock);
}

static int script_task_run = 0;

static void stop_handler(void * _) {
  script_task_run = 0;
}

extern void script_input_deinit();

extern int lapi_open_gfx(lua_State * L);
extern int lapi_open_input(lua_State * L);
extern int lapi_open_timer(lua_State * L);

int script_task_fn(void * _) {
  const char * script_name = (const char *)_;

  printf("script: starting with script `%s`\n", script_name);

  lua_State * L = luaL_newstate();
  luaL_openlibs(L);

  lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
      lua_pushcfunction(L, lapi_open_input); lua_setfield(L, -2, "input");
      lua_pushcfunction(L, lapi_open_gfx); lua_setfield(L, -2, "gfx");
      lua_pushcfunction(L, lapi_open_timer); lua_setfield(L, -2, "timer");
      lua_pop(L, 2);

  if(luaL_dofile(L, script_name)) {
    printf("%s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  }

  script_task_run = 1;
  while(script_task_run) {
    void * ev = NULL;

    SDL_LockMutex(script_event_queue_lock);
    while(queue_pop(&script_event_queue, &ev) == 0) {
      SDL_CondWait(script_event_queue_signal, script_event_queue_lock);
    }
    SDL_UnlockMutex(script_event_queue_lock);

    script_event_t * event = (script_event_t *)ev;
    event->handler(event->data);
    free(event);
  }

  printf("script: stopping\n");

  script_input_deinit();

  lua_close(L);
  L = NULL;

  return 0;
}

SDL_Thread * thread = NULL;

void script_init() {
  assert(thread == NULL);

  script_event_queue_lock = SDL_CreateMutex();
  script_event_queue_signal = SDL_CreateCond();

  thread = SDL_CreateThread(script_task_fn, "script", "main.lua");
}
void script_deinit() {
  script_event_t stop_event;
  SDL_zero(stop_event);
  stop_event.handler = stop_handler;
  stop_event.data = NULL;
  script_emit(&stop_event);

  SDL_WaitThread(thread, NULL);
  thread = NULL;

  SDL_DestroyCond(script_event_queue_signal);
  script_event_queue_signal = NULL;
  SDL_DestroyMutex(script_event_queue_lock);
  script_event_queue_lock = NULL;

  queue_clear(&script_event_queue);
}

