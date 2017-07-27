
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
static int create_window(lua_State * L) {
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
static int flush(lua_State * L) {
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
static int clear(lua_State * L) {
  return 0;
}

typedef struct gfx_load_texture_state {
  unsigned int tex_id;
  luaref_t callback;
} gfx_load_texture_state_t;

static void on_load_texture(void * ud) {
  gfx_load_texture_state_t * state = (gfx_load_texture_state_t *)ud;

  lua_State * L = luaref_push(&state->callback);
  lua_pushinteger(L, state->tex_id);
  lua_call(L, 1, 0);

  luaref_clear(&state->callback);
  free(state);
}
static void gfx_load_texture_cb(unsigned int tex_id, void * ud) {
  gfx_load_texture_state_t * state = (gfx_load_texture_state_t *)ud;

  state->tex_id = tex_id;

  script_event_t ev;
  SDL_zero(ev);
  ev.handler = on_load_texture;
  ev.data = (void *)state;

  script_emit(&ev);
}
// gfx.load_texture
static int load_texture(lua_State * L) {
  int width = 2;
  int height = 2;

  uint8_t rgba_pixels[] = {
    0x00, 0x00, 0x00, 0xFF,  0xFF, 0x00, 0x00, 0xFF, 
    0x00, 0xFF, 0x00, 0xFF,  0xFF, 0xFF, 0x00, 0xFF, 
  };

  luaL_checktype(L, 2, LUA_TFUNCTION);
  gfx_load_texture_state_t * state = calloc(1, sizeof(*state));
  luaref_set(&state->callback, L, 2);

  gfx_load_texture_rgba(width,
                        height,
                        rgba_pixels,
                        gfx_load_texture_cb,
                        state);

  return 0;
}
// gfx.load_texture
static int unload_texture(lua_State * L) {
  unsigned int tex_id = (unsigned int)luaL_checkint(L, 1);
  gfx_unload_texture(tex_id);
  return 0;
}

// gfx.clip
static int clip(lua_State * L) {
  return 0;
}
// gfx.unclip
static int unclip(lua_State * L) {
  return 0;
}

// gfx.draw_tile_map
static int draw_tile_map(lua_State * L) {
  return 0;
}
// gfx.draw_label
static int draw_label(lua_State * L) {
  return 0;
}

static luaL_Reg functions[] = {
  {  "create_window", create_window  },
  {          "flush", flush          },
  {          "clear", clear          },
  {   "load_texture", load_texture   },
  { "unload_texture", unload_texture },
  {           "clip", clip           },
  {         "unclip", unclip         },
  {  "draw_tile_map", draw_tile_map  },
  {     "draw_label", draw_label     },
  { NULL, NULL },
};

int lapi_open_gfx(lua_State * L) {
  lua_newtable(L);
  luaL_register(L, NULL, functions);

  return 1;
}

