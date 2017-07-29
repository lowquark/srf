
#include <lua5.1/lua.hpp>

namespace script {
  namespace font {
    static int from_file(lua_State * L) {
      lua_pushnil(L);
      return 1;
    }

    static luaL_Reg functions[] = {
      { "from_file", from_file  },
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

