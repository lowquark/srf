
#include <script/script_private.h>
#include <script/luaref.h>
#include <gfx/gfx.h>

// actual in-thread callback
typedef struct gfx_create_window_state {
  int success;
  luaref_t callback;
} gfx_create_window_state_t;

static void on_window_created(void * ud) {
  gfx_create_window_state_t * state = (gfx_create_window_state_t *)ud;

  lua_State * L = luaref_push(&state->callback);
  lua_pushboolean(L, state->success);
  lua_call(L, 1, 0);

  luaref_clear(&state->callback);
  free(state);
}

static void gfx_create_window_cb(int succ, void * ud) {
  gfx_create_window_state_t * state = (gfx_create_window_state_t *)ud;

  state->success = succ;

  script_event_t ev;
  SDL_zero(ev);
  ev.handler = on_window_created;
  ev.data = (void *)state;

  script_emit(&ev);
}
// gfx.create_window
int lapi_gfx_create_window(lua_State * L) {
  int window_size_x = luaL_checkint(L, 1);
  int window_size_y = luaL_checkint(L, 2);
  int window_fullscreen = lua_toboolean(L, 3);

  if(lua_isfunction(L, 4)) {
    gfx_create_window_state_t * state = calloc(1, sizeof(*state));
    luaref_set(&state->callback, L, 4);

    gfx_create_window(window_size_x,
                      window_size_y,
                      window_fullscreen,
                      gfx_create_window_cb,
                      state);
  } else {
    gfx_create_window(window_size_x,
                      window_size_y,
                      window_fullscreen,
                      NULL,
                      NULL);
  }

  return 0;
}

typedef struct gfx_flush_state {
  luaref_t callback;
} gfx_flush_state_t;

static void on_flush(void * ud) {
  gfx_flush_state_t * state = (gfx_flush_state_t *)ud;

  lua_State * L = luaref_push(&state->callback);
  lua_call(L, 0, 0);

  luaref_clear(&state->callback);
  free(state);
}

// lua api callback translation, enqueues call event
static void gfx_flush_cb(void * ud) {
  gfx_flush_state_t * state = (gfx_flush_state_t *)ud;

  script_event_t ev;
  SDL_zero(ev);
  ev.handler = on_flush;
  ev.data = state;

  script_emit(&ev);
}
// gfx.flush
int lapi_gfx_flush(lua_State * L) {
  if(lua_isfunction(L, 1)) {
    gfx_flush_state_t * state = calloc(1, sizeof(*state));
    luaref_set(&state->callback, L, 1);

    gfx_flush(gfx_flush_cb, state);
  } else {
    gfx_flush(NULL, NULL);
  }

  return 0;
}

// gfx.clear
int lapi_gfx_clear(lua_State * L) {
  return 0;
}

// gfx.Font
int lapi_gfx_Font(lua_State * L) {
  lua_newtable(L);
  return 1;
}
// gfx.Label
int lapi_gfx_Label(lua_State * L) {
  lua_newtable(L);
  return 1;
}
// gfx.Texture
int lapi_gfx_Texture(lua_State * L) {
  lua_newtable(L);
  return 1;
}
// gfx.TileSet
int lapi_gfx_TileSet(lua_State * L) {
  lua_newtable(L);
  return 1;
}
// gfx.TileMap
int lapi_gfx_TileMap(lua_State * L) {
  lua_newtable(L);
  return 1;
}

int lapi_open_gfx(lua_State * L) {
  lua_newtable(L);

    lua_pushcfunction(L, lapi_gfx_create_window);
      lua_setfield(L, -2, "create_window");
    lua_pushcfunction(L, lapi_gfx_flush);
      lua_setfield(L, -2, "flush");
    lua_pushcfunction(L, lapi_gfx_clear);
      lua_setfield(L, -2, "clear");

    lua_pushcfunction(L, lapi_gfx_Font);
      lua_setfield(L, -2, "Font");
    lua_pushcfunction(L, lapi_gfx_Label);
      lua_setfield(L, -2, "Label");
    lua_pushcfunction(L, lapi_gfx_Texture);
      lua_setfield(L, -2, "Texture");
    lua_pushcfunction(L, lapi_gfx_TileSet);
      lua_setfield(L, -2, "TileSet");
    lua_pushcfunction(L, lapi_gfx_TileMap);
      lua_setfield(L, -2, "TileMap");

  return 1;
}

