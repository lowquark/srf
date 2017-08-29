#ifndef KVStore_HPP
#define KVStore_HPP

#include <gdbm.h>
#include <util/AsyncContext.hpp>
#include <memory>

namespace db {
  class KVStore {
    public:
    ~KVStore();

    bool open(const std::string & path);
    void close();
    bool read(const std::string & key, std::string & value);
    void write(const std::string & key, const std::string & value);
    void remove(const std::string & key);

    private:
    GDBM_FILE db_file = NULL;
  };

  class AsyncKVStore {
    public:
    class OpenHandler {
      public:
      virtual ~OpenHandler() = default;
      virtual void operator()(bool success) {}
    };
    class ReadHandler {
      public:
      virtual ~ReadHandler() = default;
      virtual void operator()(bool found, std::string && value) {}
    };
    class FinishHandler {
      public:
      virtual ~FinishHandler() = default;
      virtual void operator()() {}
    };

    void open(const std::string & path,
              OpenHandler * callback = nullptr);

    void close(FinishHandler * callback = nullptr);

    void read(const std::string & key,
              ReadHandler * callback = nullptr);

    void write(const std::string & key,
               const std::string & value,
               FinishHandler * callback = nullptr);

    void remove(const std::string & key,
                FinishHandler * callback = nullptr);

    private:
    KVStore db;
    AsyncContext actx;

    class OpenAction : public AsyncContext::Action {
      KVStore * db;
      std::string path;
      OpenHandler * cb;

      public:
      OpenAction(KVStore * db,
                 const std::string & path,
                 OpenHandler * cb)
        : db(db)
        , path(path)
        , cb(cb) {}

      ~OpenAction() {
        delete cb;
      }
      void operator()() override {
        bool success = db->open(path);
        if(cb) { (*cb)(success); }
      }
    };
    class CloseAction : public AsyncContext::Action {
      KVStore * db;
      FinishHandler * cb;

      public:
      CloseAction(KVStore * db,
                  FinishHandler * cb)
        : db(db)
        , cb(cb) {}

      ~CloseAction() {
        delete cb;
      }
      void operator()() override {
        db->close();
        if(cb) { (*cb)(); }
      }
    };
    class ReadAction : public AsyncContext::Action {
      KVStore * db;
      std::string key;
      ReadHandler * cb;

      public:
      ReadAction(KVStore * db,
                 const std::string & key,
                 ReadHandler * cb)
        : db(db)
        , key(key)
        , cb(cb) {}

      ~ReadAction() {
        delete cb;
      }
      void operator()() override {
        if(cb) {
          std::string value_out;
          (*cb)(db->read(key, value_out), std::move(value_out));
        }
      }
    };
    class WriteAction : public AsyncContext::Action {
      KVStore * db;
      std::string key;
      std::string value;
      FinishHandler * cb;

      public:
      WriteAction(KVStore * db,
                  const std::string & key,
                  const std::string & value,
                  FinishHandler * cb)
        : db(db)
        , key(key)
        , value(value)
        , cb(cb) {}

      ~WriteAction() {
        delete cb;
      }
      void operator()() override {
        db->write(key, value);
        if(cb) { (*cb)(); }
      }
    };
    class RemoveAction : public AsyncContext::Action {
      KVStore * db;
      std::string key;
      FinishHandler * cb;

      public:
      RemoveAction(KVStore * db, const std::string & key, FinishHandler * cb)
        : db(db)
        , key(key)
        , cb(cb) {}

      ~RemoveAction() {
        delete cb;
      }
      void operator()() override {
        db->remove(key);
        if(cb) { (*cb)(); }
      }
    };
  };
}

#endif
