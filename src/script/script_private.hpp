#ifndef SCRIPT_PRIVATE_HPP
#define SCRIPT_PRIVATE_HPP

#include <utility>

namespace script {
  struct Event {
    public:
    virtual ~Event() = default;
    virtual void emit() {};
  };

  template <typename T, typename ... Args>
  void emit(Args ... args) {
    extern void emit(Event * event);
    emit(new T(std::forward<Args>(args)...));
  }
}

#endif
