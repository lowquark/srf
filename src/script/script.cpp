
#include "script.hpp"
#include "script_private.hpp"

#include <util/AsyncContext.hpp>

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
  namespace db {
    int luaopen(lua_State * L);
    //void clear_lua_refs();
  }

  /*
  namespace image {
    int luaopen(lua_State * L);
    void clear_lua_refs();
  }
  namespace font {
    int luaopen(lua_State * L);
    void clear_lua_refs();
  }
  */

  AsyncContext * actx = nullptr;
  lua_State * L = NULL;

  // TODO: Remove this
  void emit(Action * event) {
    actx->defer([=](){
      (*event)();
      delete event;
    });
  }
  void defer(std::function<void()> && action) {
    actx->defer(std::forward<decltype(action)>(action));
  }

  static luaL_Reg module_loaders[] = {
    { "srf.input", input::luaopen },
    { "srf.gfx",   gfx::luaopen   },
    { "srf.timer", timer::luaopen },
    { "srf.db",    db::luaopen },
    //{ "srf.image", image::luaopen },
    //{ "srf.font",  font::luaopen  },
    { NULL, NULL },
  };

  static int traceback (lua_State *L) {
    if (!lua_isstring(L, 1))  /* 'message' not a string? */
      return 1;  /* keep it intact */
    lua_getfield(L, LUA_GLOBALSINDEX, "debug");
    if (!lua_istable(L, -1)) {
      lua_pop(L, 1);
      return 1;
    }
    lua_getfield(L, -1, "traceback");
    if (!lua_isfunction(L, -1)) {
      lua_pop(L, 2);
      return 1;
    }
    lua_pushvalue(L, 1);  /* pass error message */
    lua_pushinteger(L, 2);  /* skip this function and traceback */
    lua_call(L, 2, 1);  /* call debug.traceback */
    return 1;
  }

  void pcall(lua_State * L, int narg, int nresult) {
    lua_pushcfunction(L, traceback);
    lua_insert(L, -narg-2);
    if(lua_pcall(L, narg, nresult, -narg-2)) {
      printf("script error: %s\n", lua_tostring(L, -1));
      lua_pop(L, 2);
    } else {
      lua_remove(L, -nresult-1);
    }
  }

  static void close() {
    if(L) {
      printf("script: stopping\n");

      input::clear_lua_refs();
      gfx::clear_lua_refs();
      timer::clear_lua_refs();
      //image::clear_lua_refs();
      //font::clear_lua_refs();

      lua_close(L);
      L = NULL;
    }
  }
  static void open(const std::string & module_name) {
    close();

    printf("script: starting with module `%s`\n", module_name.c_str());

    L = luaL_newstate();
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
    lua_pushstring(L, module_name.c_str());
    pcall(L, 1, 0);
  }

  void init() {
    actx = new AsyncContext;
    actx->defer([](){
      open("main");
    });
  }
  void deinit() {
    actx->defer([](){
      close();
    });
    delete actx;
    actx = nullptr;
  }

  bool handle_sdl_event(const SDL_Event * event) {
    if(input::handle_sdl_event(event)) { return true; }
    else if(gfx::handle_sdl_event(event)) { return true; }
    else { return false; }
  }
}

