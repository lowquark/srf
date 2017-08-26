
#include <script/script_private.hpp>
#include <script/luaref.hpp>

#include <db/db.hpp>
#include <vector>
#include <memory>

namespace script {
  namespace db {
    static int KVStore_open(lua_State * L) {
      ::db::DB * db = (::db::DB *)luaL_checkudata(L, 1, "kvstore");
      const char * path = luaL_checkstring(L, 2);
      luaL_checktype(L, 3, LUA_TFUNCTION);

      // must be a pointer in order to copy only once
      auto callback = std::make_shared<LuaRef>(L, 3);

      db->open(path, [=](bool succ){
        defer([=](){
          lua_State * L = callback->push();
          if(L) {
            lua_pushboolean(L, succ);
            pcall(L, 1, 0);
          }
        });
      });

      return 0;
    }
    static int KVStore_close(lua_State * L) {
      ::db::DB * db = (::db::DB *)luaL_checkudata(L, 1, "kvstore");

      db->close();

      return 0;
    }
    static int KVStore_read(lua_State * L) {
      ::db::DB * db = (::db::DB *)luaL_checkudata(L, 1, "kvstore");
      const char * key = luaL_checkstring(L, 2);
      luaL_checktype(L, 3, LUA_TFUNCTION);

      // must be a pointer in order to copy only once
      auto callback = std::make_shared<LuaRef>(L, 3);

      db->read(key, [=](std::string && value){
        defer([=](){
          lua_State * L = callback->push();
          if(L) {
            lua_pushlstring(L, value.c_str(), value.size());
            pcall(L, 1, 0);
          }
        });
      });

      return 0;
    }
    static int KVStore_write(lua_State * L) {
      ::db::DB * db = (::db::DB *)luaL_checkudata(L, 1, "kvstore");
      const char * key = luaL_checkstring(L, 2);
      const char * value = luaL_checkstring(L, 3);
      luaL_checktype(L, 4, LUA_TFUNCTION);

      // must be a pointer in order to copy only once
      auto callback = std::make_shared<LuaRef>(L, 4);

      db->write(key, value, [=](){
        defer([=](){
          lua_State * L = callback->push();
          if(L) {
            pcall(L, 0, 0);
          }
        });
      });

      return 0;
    }
    static int KVStore_gc(lua_State * L) {
      ::db::DB * db = (::db::DB *)luaL_checkudata(L, 1, "kvstore");
      db->~DB();
      return 0;
    }

    static int KVStore(lua_State * L) {
      ::db::DB * d = (::db::DB *)lua_newuserdata(L, sizeof(*d));
        luaL_getmetatable(L, "kvstore");
          lua_setmetatable(L, -2);

      new (d) ::db::DB;

      return 1;
    }

    int luaopen(lua_State * L) {
      lua_newtable(L);
        lua_pushcfunction(L, KVStore);
          lua_setfield(L, -2, "KVStore");

      if(luaL_newmetatable(L, "kvstore")) {
        lua_newtable(L);
          lua_pushcfunction(L, KVStore_open);
            lua_setfield(L, -2, "open");
          lua_pushcfunction(L, KVStore_close);
            lua_setfield(L, -2, "close");
          lua_pushcfunction(L, KVStore_read);
            lua_setfield(L, -2, "read");
          lua_pushcfunction(L, KVStore_write);
            lua_setfield(L, -2, "write");
          lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, KVStore_gc);
          lua_setfield(L, -2, "__gc");
      }

      lua_pop(L, 1);

      return 1;
    }
  }
}

