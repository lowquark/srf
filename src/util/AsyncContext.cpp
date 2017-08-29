
#include "AsyncContext.hpp"

#include <sstream>

AsyncContext::AsyncContext()
  : thread(&AsyncContext::task, this) {}
AsyncContext::~AsyncContext() {
  std::stringstream ss;
  ss << thread.get_id();
  uint64_t id = std::stoull(ss.str());

  struct StopAction : public Action {
    AsyncContext * ctx;
    StopAction(AsyncContext * ctx) : ctx(ctx) {}
    void operator()() override {
      ctx->task_run = false;
    }
  };

  enqueue(new StopAction(this));
  thread.join();

  printf("(AsyncContext) thread %lu joined\n", id);
}

void AsyncContext::enqueue(Action * c) {
  std::unique_lock<std::mutex> lk(actions_lock);
  actions.push(c);
  lk.unlock();
  actions_cv.notify_one();
}

void AsyncContext::task() {
  std::stringstream ss;
  ss << std::this_thread::get_id();
  uint64_t id = std::stoull(ss.str());

  printf("(AsyncContext) thread %lu starting\n", id);

  task_run = true;
  while(task_run) {
    std::unique_lock<std::mutex> lk(actions_lock);
    while(actions.empty()) {
      actions_cv.wait(lk);
    }

    auto c = actions.front();
    actions.pop();

    lk.unlock();

    (*c)();
    delete c;
    c = nullptr;
  }

  printf("(AsyncContext) thread %lu stopping\n", id);
}

