#ifndef UTIL_ASYNCCONTEX_HPP
#define UTIL_ASYNCCONTEX_HPP

#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <sstream>
#include <queue>

class AsyncContext {
  public:
  typedef std::function<void()> Action;

  AsyncContext()
    : thread(&AsyncContext::task, this) {}
  ~AsyncContext() {
    std::stringstream ss;
    ss << thread.get_id();
    uint64_t id = std::stoull(ss.str());

    defer([&](){ task_run = false; });
    thread.join();

    printf("thread %lu joined\n", id);
  }

  void defer(Action && c) {
    std::unique_lock<std::mutex> lk(actions_lock);
    actions.push(new Action(c));
    lk.unlock();
    actions_cv.notify_one();
  }

  private:
  std::thread thread;

  bool task_run = true;
  std::queue<Action *> actions;
  mutable std::mutex actions_lock;
  mutable std::condition_variable actions_cv;

  void task() {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    uint64_t id = std::stoull(ss.str());

    printf("thread %lu starting\n", id);

    task_run = true;
    while(task_run) {
      std::unique_lock<std::mutex> lk(actions_lock);
      while(actions.empty()) {
        actions_cv.wait(lk);
      }

      Action * c = actions.front();
      actions.pop();

      lk.unlock();

      (*c)();
      delete c;
      c = nullptr;
    }

    printf("thread %lu stopping\n", id);
  }
};

#endif
