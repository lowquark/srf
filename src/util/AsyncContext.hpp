#ifndef UTIL_ASYNCCONTEX_HPP
#define UTIL_ASYNCCONTEX_HPP

#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>

class AsyncContext {
  public:
  class Action {
    public:
    virtual ~Action() = default;
    virtual void operator()() {}
  };

  AsyncContext();
  ~AsyncContext();

  AsyncContext(const AsyncContext &) = delete;
  AsyncContext & operator=(const AsyncContext &) = delete;

  // must be allocated with `new`
  // will be deleted via `delete` at an unspecified point in time
  void enqueue(Action * c);

  private:
  std::thread thread;

  bool task_run = true;
  std::queue<Action *> actions;
  mutable std::mutex actions_lock;
  mutable std::condition_variable actions_cv;

  void task();
};

#endif
