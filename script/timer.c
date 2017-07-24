
#include <script/script_private.h>
#include <script/luaref.h>

#include <SDL2/SDL_timer.h>

typedef struct lapi_timer {
  luaref_t self;
  luaref_t callback;
  Uint32 timer_id;
} lapi_timer_t;

void handle_loop_timer(void * _) {
  lapi_timer_t * timer = (lapi_timer_t *)_;

  // it is possible the timer has been cancelled by this point, so make sure
  // the callback hasn't been cleared
  lua_State * L = luaref_push(&timer->callback);
  if(L) {
    lua_call(L, 0, 0);
  }
}
void handle_oneshot_timer(void * _) {
  lapi_timer_t * timer = (lapi_timer_t *)_;

  // it is possible the timer has been cancelled by this point, so make sure
  // the callback hasn't been cleared
  lua_State * L = luaref_push(&timer->callback);
  if(L) {
    lua_call(L, 0, 0);

    luaref_clear(&timer->self);
    luaref_clear(&timer->callback);
    timer->timer_id = 0;
  }
}

Uint32 loop_callback(Uint32 interval, void * param) {
  // enqueue call onto script event queue
  script_event_t ev;
  SDL_zero(ev);
  ev.handler = handle_loop_timer;
  ev.data = param;

  script_emit(&ev);

  return interval;
}
Uint32 oneshot_callback(Uint32 interval, void * param) {
  // enqueue call onto script event queue
  script_event_t ev;
  SDL_zero(ev);
  ev.handler = handle_oneshot_timer;
  ev.data = param;

  script_emit(&ev);

  return 0;
}

static int lapi_timer_cancel(lua_State * L) {
  lapi_timer_t * timer = (lapi_timer_t *)luaL_checkudata(L, 1, "timer");

  // try to prevent the interrupt from firing (but it's possible it already has)
  SDL_RemoveTimer(timer->timer_id);

  // clear the callbacks, this will prevent the handler (same thread) from executing
  luaref_clear(&timer->self);
  luaref_clear(&timer->callback);
  timer->timer_id = 0;

  return 0;
}

static int lapi_timer_loop(lua_State * L) {
  int interval = luaL_checkint(L, 1);
  luaL_checktype(L, 2, LUA_TFUNCTION);

  if(interval < 0) { interval = 0; }

  lapi_timer_t * timer = lua_newuserdata(L, sizeof(*timer));
    luaL_getmetatable(L, "timer");
      lua_setmetatable(L, -2);

  SDL_zero(*timer);
  luaref_set(&timer->self, L, -1);
  luaref_set(&timer->callback, L, 2);
  timer->timer_id = SDL_AddTimer(interval, loop_callback, timer);

  return 1;
}
static int lapi_timer_oneshot(lua_State * L) {
  int interval = luaL_checkint(L, 1);
  luaL_checktype(L, 2, LUA_TFUNCTION);

  if(interval < 0) { interval = 0; }

  lapi_timer_t * timer = lua_newuserdata(L, sizeof(*timer));
    luaL_getmetatable(L, "timer");
      lua_setmetatable(L, -2);

  SDL_zero(*timer);
  luaref_set(&timer->self, L, -1);
  luaref_set(&timer->callback, L, 2);
  timer->timer_id = SDL_AddTimer(interval, oneshot_callback, timer);

  return 1;
}

int lapi_open_timer(lua_State * L) {
  lua_newtable(L);

    lua_pushcfunction(L, lapi_timer_loop);
      lua_setfield(L, -2, "loop");
    lua_pushcfunction(L, lapi_timer_oneshot);
      lua_setfield(L, -2, "oneshot");
    lua_pushcfunction(L, lapi_timer_cancel);
      lua_setfield(L, -2, "cancel");

  if(luaL_newmetatable(L, "timer")) {
    lua_newtable(L);
      lua_pushcfunction(L, lapi_timer_cancel);
        lua_setfield(L, -2, "cancel");
      lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, lapi_timer_cancel);
      lua_setfield(L, -2, "__gc");

    lua_pop(L, 1);
  } else {
    lua_pop(L, 1);
  }

  return 1;
}

void script_timer_emit(void * data) {
}

