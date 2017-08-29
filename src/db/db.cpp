
#include "db.hpp"

namespace db {
  DB::~DB() {
    // join thread
    close();
  }

  void DB::open(const std::string & path,
                const std::function<void(bool)> & cb) {
    actx.defer([=](){
      db_file = gdbm_open(path.c_str(), 1024, GDBM_WRCREAT, 0644, NULL);

      if(db_file) {
        cb(true);
      } else {
        cb(false);
      }
    });
  }

  void DB::close() {
    actx.defer([=](){
      if(db_file) {
        gdbm_close(db_file);
        db_file = NULL;
      }
    });
  }

  void DB::read(const std::string & key,
                const std::function<void(std::shared_ptr<std::string> &&)> & cb) {
    actx.defer([=](){
      if(db_file) {
        datum keyd = { (char *)key.c_str(), (int)key.size() };
        datum valued = gdbm_fetch(db_file, keyd);
        if(valued.dptr) {
          auto value = std::make_shared<std::string>(valued.dptr, valued.dsize);
          free(valued.dptr);
          cb(std::move(value));
        } else {
          cb(nullptr);
        }
      } else {
        cb(nullptr);
      }
    });
  }

  void DB::write(const std::string & key,
                 const std::string & value) {
    actx.defer([=](){
      if(db_file) {
        datum keyd = { (char *)key.c_str(), (int)key.size() };
        datum valued = { (char *)value.c_str(), (int)value.size() };
        gdbm_store(db_file, keyd, valued, GDBM_REPLACE);
      }
    });
  }
  void DB::write(const std::string & key,
                 const std::string & value,
                 const std::function<void()> & cb) {
    actx.defer([=](){
      if(db_file) {
        datum keyd = { (char *)key.c_str(), (int)key.size() };
        datum valued = { (char *)value.c_str(), (int)value.size() };
        gdbm_store(db_file, keyd, valued, GDBM_REPLACE);
      }
      cb();
    });
  }

  void DB::remove(const std::string & key) {
    actx.defer([=](){
      if(db_file) {
        datum keyd = { (char *)key.c_str(), (int)key.size() };
        gdbm_delete(db_file, keyd);
      }
    });
  }
  void DB::remove(const std::string & key,
                  const std::function<void()> & cb) {
    actx.defer([=](){
      if(db_file) {
        datum keyd = { (char *)key.c_str(), (int)key.size() };
        gdbm_delete(db_file, keyd);
      }
      cb();
    });
  }
}

