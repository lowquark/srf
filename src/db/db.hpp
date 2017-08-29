#ifndef DB_HPP
#define DB_HPP

#include <gdbm.h>
#include <util/AsyncContext.hpp>
#include <memory>

namespace db {
  class DB {
    AsyncContext actx;

    public:
    ~DB();

    void open(const std::string & path,
              const std::function<void(bool)> & cb);

    void close();

    void read(const std::string & key,
              const std::function<void(std::shared_ptr<std::string> &&)> & cb);

    void write(const std::string & key,
               const std::string & value);
    void write(const std::string & key,
               const std::string & value,
               const std::function<void()> & cb);

    void remove(const std::string & key);
    void remove(const std::string & key,
                const std::function<void()> & cb);

    void flush();

    private:
    GDBM_FILE db_file = NULL;
  };
}

#endif
