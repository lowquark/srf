#ifndef SCRIPT_PRIVATE_HPP
#define SCRIPT_PRIVATE_HPP

#include <utility>

namespace script {
  struct Action {
    public:
    virtual ~Action() = default;
    virtual void operator()() {};
  };

  template <typename T, typename ... Args>
  void emit(Args ... args) {
    extern void emit(Action * event);
    emit(new T(std::forward<Args>(args)...));
  }
}

#endif
