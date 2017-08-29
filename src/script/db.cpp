
#include <script/script_private.hpp>
#include <script/luaref.hpp>
#include <script/Call.hpp>

#include <db/db.hpp>
#include <vector>
#include <memory>

namespace script {
  namespace db {
    class OpenHandler : public ::db::AsyncKVStore::OpenHandler {
      LuaRef fn;
      public:
      OpenHandler(LuaRef && fn)
        : fn((LuaRef &&)fn) {}
      void operator()(bool succ) {
        actx->enqueue(
            new Call<bool>(std::move(fn), succ));
      }
    };
    class ReadHandler : public ::db::AsyncKVStore::ReadHandler {
      LuaRef fn;
      public:
      ReadHandler(LuaRef && fn)
        : fn((LuaRef &&)fn) {}
      void operator()(bool found, std::string && value) {
        if(found) {
          actx->enqueue(
              new Call<std::string &&>(std::move(fn), (std::string &&)value));
        } else {
          actx->enqueue(
              new Call<nullptr_t>(std::move(fn), nullptr));
        }
      }
    };
    class FinishHandler : public ::db::AsyncKVStore::FinishHandler {
      LuaRef fn;
      public:
      FinishHandler(LuaRef && fn)
        : fn((LuaRef &&)fn) {}
      void operator()() {
        actx->enqueue(new Call<>(std::move(fn)));
      }
    };

    static int KVStore_open(lua_State * L) {
      ::db::AsyncKVStore * db = (::db::AsyncKVStore *)luaL_checkudata(L, 1, "kvstore");
      const char * path = luaL_checkstring(L, 2);
      luaL_checktype(L, 3, LUA_TFUNCTION);

      db->open(path, new OpenHandler(LuaRef(L, 3)));

      return 0;
    }
    static int KVStore_close(lua_State * L) {
      ::db::AsyncKVStore * db = (::db::AsyncKVStore *)luaL_checkudata(L, 1, "kvstore");

      db->close();

      return 0;
    }
    static int KVStore_read(lua_State * L) {
      size_t key_len;
      ::db::AsyncKVStore * db = (::db::AsyncKVStore *)luaL_checkudata(L, 1, "kvstore");
      const char * key = luaL_checklstring(L, 2, &key_len);
      luaL_checktype(L, 3, LUA_TFUNCTION);

      db->read(std::string(key, key_len), new ReadHandler(LuaRef(L, 3)));

      return 0;
    }
    static int KVStore_write(lua_State * L) {
      size_t key_len, value_len;
      ::db::AsyncKVStore * db = (::db::AsyncKVStore *)luaL_checkudata(L, 1, "kvstore");
      const char * key = luaL_checklstring(L, 2, &key_len);
      const char * value = luaL_checklstring(L, 3, &value_len);

      if(lua_type(L, 4) == LUA_TFUNCTION) {
        db->write(std::string(key, key_len),
                  std::string(value, value_len),
                  new FinishHandler(LuaRef(L, 4)));
      } else {
        db->write(std::string(key, key_len),
                  std::string(value, value_len));
      }

      return 0;
    }
    static int KVStore_gc(lua_State * L) {
      ::db::AsyncKVStore * db = (::db::AsyncKVStore *)luaL_checkudata(L, 1, "kvstore");
      db->~AsyncKVStore();
      return 0;
    }

    static int KVStore(lua_State * L) {
      ::db::AsyncKVStore * d = (::db::AsyncKVStore *)lua_newuserdata(L, sizeof(*d));
        luaL_getmetatable(L, "kvstore");
          lua_setmetatable(L, -2);

      new (d) ::db::AsyncKVStore;

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

