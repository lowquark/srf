
#include "db.hpp"

#include <sqlite3.h>

namespace db {
  DB::~DB() {
    // join thread
    close();
  }

  void DB::open(const std::string & path,
                const std::function<void(bool)> & cb) {
    actx.defer([=](){
      printf("DB::open()!\n");
      cb(false);
    });
  }

  void DB::close() {
    actx.defer([=](){
      printf("DB::close()!\n");
    });
  }

  void DB::read(const std::string & key,
                const std::function<void(std::string &&)> & cb) {
    actx.defer([=](){
      printf("DB::read(`%s`)!\n", key.c_str());
      cb("");
    });
  }

  void DB::write(const std::string & key,
                 const std::string & value,
                 const std::function<void()> & cb) {
    actx.defer([=](){
      printf("DB::write(`%s`, `%s`)!\n", key.c_str(), value.c_str());
      cb();
    });
  }
}

