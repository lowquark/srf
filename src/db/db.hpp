#ifndef DB_HPP
#define DB_HPP

#include <sqlite3.h>
#include <util/AsyncContext.hpp>

namespace db {
  class DB {
    AsyncContext actx;

    public:
    ~DB();

    void open(const std::string & path,
              const std::function<void(bool)> & cb);

    void close();

    void read(const std::string & key,
              const std::function<void(std::string &&)> & cb);

    void write(const std::string & key,
               const std::string & value,
               const std::function<void()> & cb);

    private:
    sqlite3 * db = NULL;
  };
}

#endif
