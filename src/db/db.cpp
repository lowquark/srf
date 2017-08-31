
#include "db.hpp"

namespace db {
  KVStore::~KVStore() {
    close();
  }

  bool KVStore::open(const std::string & path) {
    close();
    db_file = gdbm_open(path.c_str(), 1024, GDBM_WRCREAT, 0644, NULL);
    return db_file != NULL;
  }
  void KVStore::close() {
    if(db_file) {
      gdbm_close(db_file);
      db_file = NULL;
    }
  }
  bool KVStore::read(const std::string & key, std::string & value) {
    if(db_file) {
      datum keyd = { (char *)key.c_str(), (int)key.size() };
      datum valued = gdbm_fetch(db_file, keyd);
      if(valued.dptr) {
        value = std::string(valued.dptr, valued.dsize);
        free(valued.dptr);
        return true;
      } else {
        return false;
      }
    }
    return false;
  }
  void KVStore::write(const std::string & key, const std::string & value) {
    if(db_file) {
      datum keyd = { (char *)key.c_str(), (int)key.size() };
      datum valued = { (char *)value.c_str(), (int)value.size() };
      gdbm_store(db_file, keyd, valued, GDBM_REPLACE);
    }
  }
  void KVStore::remove(const std::string & key) {
    if(db_file) {
      datum keyd = { (char *)key.c_str(), (int)key.size() };
      gdbm_delete(db_file, keyd);
    }
  }

  void AsyncKVStore::open(const std::string & path, OpenHandler * cb) {
    actx.enqueue(new OpenAction( &db, path, cb ));
  }

  void AsyncKVStore::close(FinishHandler * cb) {
    actx.enqueue(new CloseAction( &db, cb ));
  }

  void AsyncKVStore::read(const std::string & key, ReadHandler * cb) {
    actx.enqueue(new ReadAction( &db, key, cb ));
  }

  void AsyncKVStore::write(const std::string & key,
                           const std::string & value,
                           FinishHandler * cb) {
    actx.enqueue(new WriteAction( &db, key, value, cb ));
  }

  void AsyncKVStore::remove(const std::string & key, FinishHandler * cb) {
    actx.enqueue(new RemoveAction(&db, key, cb));
  }
}

