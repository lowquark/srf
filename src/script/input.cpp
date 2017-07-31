
#include <SDL2/SDL_events.h>

#include <script/script_private.hpp>
#include <script/event.hpp>

namespace script {
  namespace input {
    static script::LuaEventEmitter quit_event;
    static script::LuaEventEmitter keydown_event;
    static script::LuaEventEmitter keyup_event;

    static int on(lua_State * L) {
      const char * events[] = {
        "quit", "keydown", "keyup", NULL
      };

      int index = luaL_checkoption(L, 1, NULL, events);

      if(index == 0) {
        quit_event.add_listener(L, 2);
      } else if(index == 1) {
        keydown_event.add_listener(L, 2);
      } else if(index == 2) {
        keyup_event.add_listener(L, 2);
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

        case SDL_SCANCODE_0:
          return "0";
        case SDL_SCANCODE_1:
          return "1";
        case SDL_SCANCODE_2:
          return "2";
        case SDL_SCANCODE_3:
          return "3";
        case SDL_SCANCODE_4:
          return "4";
        case SDL_SCANCODE_5:
          return "5";
        case SDL_SCANCODE_6:
          return "6";
        case SDL_SCANCODE_7:
          return "7";
        case SDL_SCANCODE_8:
          return "8";
        case SDL_SCANCODE_9:
          return "9";

        case SDL_SCANCODE_KP_0:
          return "numpad_0";
        case SDL_SCANCODE_KP_1:
          return "numpad_1";
        case SDL_SCANCODE_KP_2:
          return "numpad_2";
        case SDL_SCANCODE_KP_3:
          return "numpad_3";
        case SDL_SCANCODE_KP_4:
          return "numpad_4";
        case SDL_SCANCODE_KP_5:
          return "numpad_5";
        case SDL_SCANCODE_KP_6:
          return "numpad_6";
        case SDL_SCANCODE_KP_7:
          return "numpad_7";
        case SDL_SCANCODE_KP_8:
          return "numpad_8";
        case SDL_SCANCODE_KP_9:
          return "numpad_9";

        default: return NULL;
      }
    }

#define LOAD_KEY(code) lua_pushinteger(L, code); lua_setfield(L, -2, get_key_name(code));

    int luaopen(lua_State * L) {
      lua_newtable(L);

        lua_pushcfunction(L, on);
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

          LOAD_KEY(SDL_SCANCODE_KP_0)
          LOAD_KEY(SDL_SCANCODE_KP_1)
          LOAD_KEY(SDL_SCANCODE_KP_2)
          LOAD_KEY(SDL_SCANCODE_KP_3)
          LOAD_KEY(SDL_SCANCODE_KP_4)
          LOAD_KEY(SDL_SCANCODE_KP_5)
          LOAD_KEY(SDL_SCANCODE_KP_6)
          LOAD_KEY(SDL_SCANCODE_KP_7)
          LOAD_KEY(SDL_SCANCODE_KP_8)
          LOAD_KEY(SDL_SCANCODE_KP_9)

          lua_setfield(L, -2, "scancode");

      return 1;
    }

    void clear_lua_refs() {
      quit_event.clear();
      keydown_event.clear();
      keyup_event.clear();
    }

    struct EmitQuit : public script::Action {
      void operator()() override {
        input::quit_event.emit();
      }
    };
    struct EmitKeyDown : public script::Action {
      unsigned int key;
      EmitKeyDown(unsigned int key) : key(key) {}

      struct LuaPusher : public script::LuaPusher {
        unsigned int key;

        LuaPusher(unsigned int key) : key(key) {}
        int push(lua_State * L) const override {
          lua_pushinteger(L, key);
          return 1;
        }
      };

      void operator()() override {
        input::keydown_event.emit(LuaPusher(key));
      }
    };
    struct EmitKeyUp : public script::Action {
      unsigned int key;
      EmitKeyUp(unsigned int key) : key(key) {}

      struct LuaPusher : public script::LuaPusher {
        unsigned int key;

        LuaPusher(unsigned int key) : key(key) {}
        int push(lua_State * L) const override {
          lua_pushinteger(L, key);
          return 1;
        }
      };

      void operator()() override {
        input::keyup_event.emit(LuaPusher(key));
      }
    };

    // these come in from a different thread
    bool handle_sdl_event(const SDL_Event * event) {
      if(event->type == SDL_QUIT) {
        emit<EmitQuit>();
        return true;
      } else if(event->type == SDL_KEYDOWN) {
        emit<EmitKeyDown>(event->key.keysym.scancode);
        return true;
      } else if(event->type == SDL_KEYUP) {
        emit<EmitKeyUp>(event->key.keysym.scancode);
        return true;
      }

      return false;
    }
  }
}

