
#include <SDL2/SDL_events.h>

#include <script/script_private.h>
#include <script/luaref.h>
#include <script/event.h>

/*
#define INPUT_KEY_ESCAPE SDL_SCANCODE_ESCAPE

#define INPUT_KEY_A      SDL_SCANCODE_A
#define INPUT_KEY_B      SDL_SCANCODE_B
#define INPUT_KEY_C      SDL_SCANCODE_C
#define INPUT_KEY_D      SDL_SCANCODE_D
#define INPUT_KEY_E      SDL_SCANCODE_E
#define INPUT_KEY_F      SDL_SCANCODE_F
#define INPUT_KEY_G      SDL_SCANCODE_G
#define INPUT_KEY_H      SDL_SCANCODE_H
#define INPUT_KEY_I      SDL_SCANCODE_I
#define INPUT_KEY_J      SDL_SCANCODE_J
#define INPUT_KEY_K      SDL_SCANCODE_K
#define INPUT_KEY_L      SDL_SCANCODE_L
#define INPUT_KEY_M      SDL_SCANCODE_M
#define INPUT_KEY_N      SDL_SCANCODE_N
#define INPUT_KEY_O      SDL_SCANCODE_O
#define INPUT_KEY_P      SDL_SCANCODE_P
#define INPUT_KEY_Q      SDL_SCANCODE_Q
#define INPUT_KEY_R      SDL_SCANCODE_R
#define INPUT_KEY_S      SDL_SCANCODE_S
#define INPUT_KEY_T      SDL_SCANCODE_T
#define INPUT_KEY_U      SDL_SCANCODE_U
#define INPUT_KEY_V      SDL_SCANCODE_V
#define INPUT_KEY_W      SDL_SCANCODE_W
#define INPUT_KEY_X      SDL_SCANCODE_X
#define INPUT_KEY_Y      SDL_SCANCODE_Y
#define INPUT_KEY_Z      SDL_SCANCODE_Z

#define INPUT_KEY_0      SDL_SCANCODE_0
#define INPUT_KEY_1      SDL_SCANCODE_1
#define INPUT_KEY_2      SDL_SCANCODE_2
#define INPUT_KEY_3      SDL_SCANCODE_3
#define INPUT_KEY_4      SDL_SCANCODE_4
#define INPUT_KEY_5      SDL_SCANCODE_5
#define INPUT_KEY_6      SDL_SCANCODE_6
#define INPUT_KEY_7      SDL_SCANCODE_7
#define INPUT_KEY_8      SDL_SCANCODE_8
#define INPUT_KEY_9      SDL_SCANCODE_9
*/

event_t quit_event;
event_t keydown_event;
event_t keyup_event;

static int push_i(lua_State * L, void * _) {
  lua_pushinteger(L, *(int *)_);
  return 1;
}

static void handle_quit(void * _) {
  event_emit(&quit_event);
  free(_);
}
static void handle_keydown(void * _) {
  event_emit_args(&keydown_event, push_i, _);
  free(_);
}
static void handle_keyup(void * _) {
  event_emit_args(&keyup_event, push_i, _);
  free(_);
}

int lapi_input_on(lua_State * L) {
  const char * events[] = {
    "quit", "keydown", "keyup", NULL
  };

  int index = luaL_checkoption(L, 1, NULL, events);

  if(index == 0) {
    event_add_listener(&quit_event, L, 2);
  } else if(index == 1) {
    event_add_listener(&keydown_event, L, 2);
  } else if(index == 2) {
    event_add_listener(&keyup_event, L, 2);
  }

  return 0;
}

static const char * get_key_name(SDL_Scancode code) {
  switch(code) {
    case SDL_SCANCODE_ESCAPE:
      return "escape";

    case SDL_SCANCODE_A: return "a";
    case SDL_SCANCODE_B: return "b";
    case SDL_SCANCODE_C: return "c";
    case SDL_SCANCODE_D: return "d";
    case SDL_SCANCODE_E: return "e";
    case SDL_SCANCODE_F: return "f";
    case SDL_SCANCODE_G: return "g";
    case SDL_SCANCODE_H: return "h";
    case SDL_SCANCODE_I: return "i";
    case SDL_SCANCODE_J: return "j";
    case SDL_SCANCODE_K: return "k";
    case SDL_SCANCODE_L: return "l";
    case SDL_SCANCODE_M: return "m";
    case SDL_SCANCODE_N: return "n";
    case SDL_SCANCODE_O: return "o";
    case SDL_SCANCODE_P: return "p";
    case SDL_SCANCODE_Q: return "q";
    case SDL_SCANCODE_R: return "r";
    case SDL_SCANCODE_S: return "s";
    case SDL_SCANCODE_T: return "t";
    case SDL_SCANCODE_U: return "u";
    case SDL_SCANCODE_V: return "v";
    case SDL_SCANCODE_W: return "w";
    case SDL_SCANCODE_X: return "x";
    case SDL_SCANCODE_Y: return "y";
    case SDL_SCANCODE_Z: return "z";

    case SDL_SCANCODE_KP_0:
      return "numpad_0";
    case SDL_SCANCODE_0:
      return "0";
    case SDL_SCANCODE_KP_1:
      return "numpad_1";
    case SDL_SCANCODE_1:
      return "1";
    case SDL_SCANCODE_KP_2:
      return "numpad_2";
    case SDL_SCANCODE_2:
      return "2";
    case SDL_SCANCODE_KP_3:
      return "numpad_3";
    case SDL_SCANCODE_3:
      return "3";
    case SDL_SCANCODE_KP_4:
      return "numpad_4";
    case SDL_SCANCODE_4:
      return "4";
    case SDL_SCANCODE_KP_5:
      return "numpad_5";
    case SDL_SCANCODE_5:
      return "5";
    case SDL_SCANCODE_KP_6:
      return "numpad_6";
    case SDL_SCANCODE_6:
      return "6";
    case SDL_SCANCODE_KP_7:
      return "numpad_7";
    case SDL_SCANCODE_7:
      return "7";
    case SDL_SCANCODE_KP_8:
      return "numpad_8";
    case SDL_SCANCODE_8:
      return "8";
    case SDL_SCANCODE_KP_9:
      return "numpad_9";
    case SDL_SCANCODE_9:
      return "9";

    default: return NULL;
  }
}

#define LOAD_KEY(code) \
      lua_pushinteger(L, code); \
        lua_setfield(L, -2, get_key_name(code));

int lapi_open_input(lua_State * L) {
  lua_newtable(L);

    lua_pushcfunction(L, lapi_input_on);
      lua_setfield(L, -2, "on");

    lua_newtable(L);
      LOAD_KEY(SDL_SCANCODE_ESCAPE)

      LOAD_KEY(SDL_SCANCODE_A)
      LOAD_KEY(SDL_SCANCODE_B)
      LOAD_KEY(SDL_SCANCODE_C)
      LOAD_KEY(SDL_SCANCODE_D)
      LOAD_KEY(SDL_SCANCODE_E)
      LOAD_KEY(SDL_SCANCODE_F)
      LOAD_KEY(SDL_SCANCODE_G)
      LOAD_KEY(SDL_SCANCODE_H)
      LOAD_KEY(SDL_SCANCODE_I)
      LOAD_KEY(SDL_SCANCODE_J)
      LOAD_KEY(SDL_SCANCODE_K)
      LOAD_KEY(SDL_SCANCODE_L)
      LOAD_KEY(SDL_SCANCODE_M)
      LOAD_KEY(SDL_SCANCODE_N)
      LOAD_KEY(SDL_SCANCODE_O)
      LOAD_KEY(SDL_SCANCODE_P)
      LOAD_KEY(SDL_SCANCODE_Q)
      LOAD_KEY(SDL_SCANCODE_R)
      LOAD_KEY(SDL_SCANCODE_S)
      LOAD_KEY(SDL_SCANCODE_T)
      LOAD_KEY(SDL_SCANCODE_U)
      LOAD_KEY(SDL_SCANCODE_V)
      LOAD_KEY(SDL_SCANCODE_W)
      LOAD_KEY(SDL_SCANCODE_X)
      LOAD_KEY(SDL_SCANCODE_Y)
      LOAD_KEY(SDL_SCANCODE_Z)

      LOAD_KEY(SDL_SCANCODE_0)
      LOAD_KEY(SDL_SCANCODE_1)
      LOAD_KEY(SDL_SCANCODE_2)
      LOAD_KEY(SDL_SCANCODE_3)
      LOAD_KEY(SDL_SCANCODE_4)
      LOAD_KEY(SDL_SCANCODE_5)
      LOAD_KEY(SDL_SCANCODE_6)
      LOAD_KEY(SDL_SCANCODE_7)
      LOAD_KEY(SDL_SCANCODE_8)
      LOAD_KEY(SDL_SCANCODE_9)

      lua_setfield(L, -2, "scancode");

  return 1;
}

void script_input_deinit() {
  event_clear(&quit_event);
  event_clear(&keydown_event);
  event_clear(&keyup_event);
}

static void emit_quit() {
  script_event_t ev;
  SDL_zero(ev);
  ev.handler = handle_quit;
  ev.data = NULL;

  script_emit(&ev);
}
static void emit_keydown(unsigned int key) {
  unsigned int * state = (unsigned int *)calloc(1, sizeof(*state));

  *state = key;

  script_event_t ev;
  SDL_zero(ev);
  ev.handler = handle_keydown;
  ev.data = (void *)state;

  script_emit(&ev);
}
static void emit_keyup(unsigned int key) {
  unsigned int * state = (unsigned int *)calloc(1, sizeof(*state));

  *state = key;

  script_event_t ev;
  SDL_zero(ev);
  ev.handler = handle_keyup;
  ev.data = (void *)state;

  script_emit(&ev);
}

int script_handle_sdl_event(const SDL_Event * event) {
  if(event->type == SDL_QUIT) {
    emit_quit();
    return 1;
  } else if(event->type == SDL_KEYDOWN) {
    emit_keydown(event->key.keysym.scancode);
    return 1;
  } else if(event->type == SDL_KEYUP) {
    emit_keyup(event->key.keysym.scancode);
    return 1;
  }

  return 0;
}

