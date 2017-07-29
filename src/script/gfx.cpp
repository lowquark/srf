
#include <script/script_private.hpp>
#include <script/luaref.hpp>
#include <gfx/gfx.hpp>
#include <memory>

namespace script {
  namespace gfx {
    struct CreateWindowEvent : public script::Event {
      public:
      CreateWindowEvent(bool success, LuaRef && function)
        : success(success)
        , lua_function(function) {}

      void emit() override {
        lua_State * L = lua_function.push();
        if(L) {
          lua_pushboolean(L, success);
          lua_call(L, 1, 0);
          lua_function.clear();
        }
      }

      private:
      bool success = false;
      LuaRef lua_function;
    };
    struct CreateWindowCallback : public ::gfx::CreateWindowCallback {
      CreateWindowCallback(LuaRef && function)
        : lua_function(function) {}

      // gfx module callback
      virtual void operator()(bool success) {
        script::emit<CreateWindowEvent>(success, std::move(lua_function));
      }

      private:
      LuaRef lua_function;
    };

    struct FlushEvent : public script::Event {
      FlushEvent(LuaRef && function)
        : lua_function(function) {}

      void emit() override {
        lua_State * L = lua_function.push();
        if(L) {
          lua_call(L, 0, 0);
          lua_function.clear();
        }
      }

      private:
      LuaRef lua_function;
    };
    struct FlushCallback : public ::gfx::FlushCallback {
      FlushCallback(LuaRef && function)
        : lua_function(function) {}

      // gfx module callback
      void operator()() override {
        script::emit<FlushEvent>(std::move(lua_function));
      }

      private:
      LuaRef lua_function;
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
            std::unique_ptr<CreateWindowCallback>(
              new CreateWindowCallback(LuaRef(L, 4))
            ));
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
        ::gfx::flush(std::unique_ptr<FlushCallback>(new FlushCallback(LuaRef(L, 1))));
      } else {
        ::gfx::flush();
      }

      return 0;
    }

    // gfx.clear
    static int clear(lua_State * L) {
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
      {           "clip", clip           },
      {         "unclip", unclip         },
      {  "draw_tile_map", draw_tile_map  },
      {     "draw_label", draw_label     },
      { NULL, NULL },
    };

    int luaopen(lua_State * L) {
      lua_newtable(L);
      luaL_register(L, NULL, functions);
      return 1;
    }
    void clear_lua_refs() {
    }
  }
}

