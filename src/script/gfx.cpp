
#include <gfx/gfx.hpp>

#include <script/script_private.hpp>
#include <script/luaref.hpp>

#include <memory>

namespace script {
  namespace gfx {
    struct BoolCall : public AsyncContext::Action {
      BoolCall(LuaRef && fn, bool b)
        : fn((LuaRef &&)fn)
        , b(b) {}

      void operator()() override {
        lua_State * L = fn.push();
        if(L) {
          lua_pushboolean(L, b);
          pcall(L, 1, 0);
          fn.clear();
        }
      }

      private:
      LuaRef fn;
      bool b = false;
    };
    struct Call : public AsyncContext::Action {
      Call(LuaRef && fn)
        : fn((LuaRef &&)fn) {}

      void operator()() override {
        lua_State * L = fn.push();
        if(L) {
          pcall(L, 0, 0);
          fn.clear();
        }
      }

      private:
      LuaRef fn;
    };

    struct CreateWindowCallback : public ::gfx::CreateWindowCallback {
      CreateWindowCallback(LuaRef && fn)
        : fn((LuaRef &&)fn) {}

      // gfx module callback
      virtual void operator()(bool success) {
        script::actx->enqueue(new BoolCall(std::move(fn), success));
      }

      private:
      LuaRef fn;
    };

    struct FlushCallback : public ::gfx::FlushCallback {
      FlushCallback(LuaRef && fn)
        : fn((LuaRef &&)fn) {}

      // gfx module callback
      void operator()() override {
        script::actx->enqueue(new Call(std::move(fn)));
      }

      private:
      LuaRef fn;
    };

    // gfx.create_window
    static int create_window(lua_State * L) {
      int window_size_x = luaL_checkint(L, 1);
      int window_size_y = luaL_checkint(L, 2);
      bool window_fullscreen = lua_toboolean(L, 3);

      if(lua_isfunction(L, 4)) {
        ::gfx::create_window(
            window_size_x,
            window_size_y,
            window_fullscreen,
            new CreateWindowCallback(LuaRef(L, 4)));
      } else {
        ::gfx::create_window(
            window_size_x,
            window_size_y,
            window_fullscreen);
      }

      return 0;
    }
    // gfx.flush
    static int flush(lua_State * L) {
      if(lua_isfunction(L, 1)) {
        ::gfx::flush(new FlushCallback(LuaRef(L, 1)));
      } else {
        ::gfx::flush();
      }

      return 0;
    }

    // gfx.clear
    static int clear(lua_State * L) {
      return 0;
    }

    /*
    // gfx.on
    static int on(lua_State * L) {
      const char * events[] = { "exposed", NULL };

      int index = luaL_checkoption(L, 1, NULL, events);

      if(index == 0) {
        exposed_event.add_listener(L, 2);
      }

      return 0;
    }
    // gfx.off
    static int off(lua_State * L) {
      const char * events[] = { "exposed", NULL };

      int index = luaL_checkoption(L, 1, NULL, events);

      if(index == 0) {
        exposed_event.remove_listener(L, 2);
      }

      return 0;
    }
    */

    // gfx.clip
    static int clip(lua_State * L) {
      return 0;
    }
    // gfx.unclip
    static int unclip(lua_State * L) {
      return 0;
    }

    struct LuaTilemap {
      std::string tileset;
      Vec2u size;

      ::gfx::Tilemap::Tile * tiles() {
        return (::gfx::Tilemap::Tile *)(this + 1);
      }
    };

    static int Tilemap(lua_State * L) {
      int size_x = luaL_checkint(L, 1);
      int size_y = luaL_checkint(L, 2);

      if(size_x < 0) { size_x = 0; }
      if(size_y < 0) { size_y = 0; }

      unsigned int n_tiles = size_x * size_y;

      LuaTilemap * tm = (LuaTilemap *)lua_newuserdata(L, sizeof(LuaTilemap) + sizeof(::gfx::Tilemap::Tile)*n_tiles);
        luaL_getmetatable(L, "tilemap");
          lua_setmetatable(L, -2);

      auto tiles = tm->tiles();

      new (tm) LuaTilemap;
      for(unsigned int i = 0 ; i < n_tiles ; i ++) {
        new (tm->tiles() + i) ::gfx::Tilemap::Tile;
      }

      tm->size = Vec2u(size_x, size_y);

      printf("tm->size: %u, %u\n", tm->size.x, tm->size.y);

      return 1;
    }
    static int Tilemap_set_tileset(lua_State * L) {
      LuaTilemap * tm = (LuaTilemap *)luaL_checkudata(L, 1, "tilemap");
      size_t strlen = 0;
      const char * str = luaL_checklstring(L, 2, &strlen);

      tm->tileset = std::string(str, strlen);

      return 0;
    }
    static int Tilemap_set_index(lua_State * L) {
      LuaTilemap * tm = (LuaTilemap *)luaL_checkudata(L, 1, "tilemap");
      int x = luaL_checkint(L, 2);
      int y = luaL_checkint(L, 3);
      int idx = luaL_checkint(L, 4);

      if(x >= 0 && (unsigned int)x < tm->size.x &&
         y >= 0 && (unsigned int)y < tm->size.y) {
        unsigned int i = x + y * tm->size.x;
        tm->tiles()[i].tileset_index = idx;
      }

      return 0;
    }
    static int Tilemap_set_foreground(lua_State * L) {
      LuaTilemap * tm = (LuaTilemap *)luaL_checkudata(L, 1, "tilemap");
      int x = luaL_checkint(L, 2);
      int y = luaL_checkint(L, 3);
      int r = luaL_checkint(L, 4);
      int g = luaL_checkint(L, 5);
      int b = luaL_checkint(L, 6);
      int a = luaL_optint(L, 7, 0xFF);

      if(x >= 0 && (unsigned int)x < tm->size.x &&
         y >= 0 && (unsigned int)y < tm->size.y) {
        unsigned int i = x + y * tm->size.x;
        tm->tiles()[i].foreground_color = ::gfx::Tilemap::Color(r, g, b, a);
      }

      return 0;
    }
    static int Tilemap_set_background(lua_State * L) {
      LuaTilemap * tm = (LuaTilemap *)luaL_checkudata(L, 1, "tilemap");
      int x = luaL_checkint(L, 2);
      int y = luaL_checkint(L, 3);
      int r = luaL_checkint(L, 4);
      int g = luaL_checkint(L, 5);
      int b = luaL_checkint(L, 6);
      int a = luaL_optint(L, 7, 0xFF);

      if(x >= 0 && (unsigned int)x < tm->size.x &&
         y >= 0 && (unsigned int)y < tm->size.y) {
        unsigned int i = x + y * tm->size.x;
        tm->tiles()[i].background_color = ::gfx::Tilemap::Color(r, g, b, a);
      }

      return 0;
    }
    static int Tilemap_gc(lua_State * L) {
      LuaTilemap * tm = (LuaTilemap *)luaL_checkudata(L, 1, "tilemap");
      unsigned int n_tiles = tm->size.x * tm->size.y;

      for(unsigned int i = 0 ; i < n_tiles ; i ++) {
        tm->tiles()[i].~Tile();
      }
      tm->~LuaTilemap();

      return 0;
    }

    // gfx.draw_tilemap
    static int draw_tilemap(lua_State * L) {
      LuaTilemap * tm = (LuaTilemap *)luaL_checkudata(L, 1, "tilemap");

      ::gfx::Tilemap tilemap;
      tilemap.tileset_uri = tm->tileset;
      tilemap.tiles.resize(tm->size);

      unsigned int i = 0;
      for(unsigned int y = 0 ; y < tm->size.y ; y ++) {
        for(unsigned int x = 0 ; x < tm->size.x ; x ++) {
          tilemap.tiles.set(Vec2i(x, y), tm->tiles()[i]);
          i ++;
        }
      }

      ::gfx::draw_tilemap(std::move(tilemap));

      return 0;
    }
    // gfx.draw_label
    static int draw_label(lua_State * L) {
      return 0;
    }

    static luaL_Reg functions[] = {
      { "create_window", create_window },
      {         "flush", flush         },
      {         "clear", clear         },
      {          "clip", clip          },
      {        "unclip", unclip        },
      {       "Tilemap", Tilemap       },
      {  "draw_tilemap", draw_tilemap  },
      {    "draw_label", draw_label    },
      { NULL, NULL },
    };

    int luaopen(lua_State * L) {
      lua_newtable(L);
      luaL_register(L, NULL, functions);

      if(luaL_newmetatable(L, "tilemap")) {
        lua_newtable(L);
          lua_pushcfunction(L, Tilemap_set_tileset);
            lua_setfield(L, -2, "set_tileset");
          lua_pushcfunction(L, Tilemap_set_index);
            lua_setfield(L, -2, "set_index");
          lua_pushcfunction(L, Tilemap_set_foreground);
            lua_setfield(L, -2, "set_foreground");
          lua_pushcfunction(L, Tilemap_set_background);
            lua_setfield(L, -2, "set_background");
          lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, Tilemap_gc);
          lua_setfield(L, -2, "__gc");

        lua_pop(L, 1);
      } else {
        lua_pop(L, 1);
      }

      return 1;
    }
    void clear_lua_refs() {
    }

    bool handle_sdl_event(const SDL_Event * event) {
      return false;
    }
  }
}

