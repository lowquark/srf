#ifndef GFX_H
#define GFX_H

#include <gfx/Tilemap.hpp>
#include <SDL2/SDL_events.h>
#include <memory>
#include <mutex>

template <typename ... EventArgs>
class Event {
  public:
  struct Listener {
    virtual ~Listener() = default;
    virtual void operator()(EventArgs ... args) {};
  };

  template <typename T, typename ... Args>
  typename std::enable_if<std::is_base_of<Listener, T>::value, T *>::type
  add_listener(Args ... args) {
    std::lock_guard<std::mutex> g(lock);
    listeners.push_back(new T(std::forward<Args>(args)...));
    return listeners.back();
  }

  Event() = default;
  ~Event() {
    std::lock_guard<std::mutex> g(lock);
    for(auto & l : listeners) {
      delete l;
    }
    listeners.clear();
  }

  Event(const Event & other) = delete;
  Event & operator=(const Event & other) = delete;

  void emit(EventArgs ... args) const {
    std::lock_guard<std::mutex> g(lock);
    for(auto & l : listeners) {
      (*l)(std::forward<EventArgs>(args)...);
    }
  }

  private:
  std::vector<Listener *> listeners;
  mutable std::mutex lock;
};

namespace gfx {
  class CreateWindowCallback {
    public:
    virtual ~CreateWindowCallback() = default;
    virtual void operator()(bool success) {};
  };
  class FlushCallback {
    public:
    virtual ~FlushCallback() = default;
    virtual void operator()() {};
  };

  // public, thread-safe messaging
  void create_window (int w, int h, bool fs, CreateWindowCallback * cb = nullptr);
  void flush         (FlushCallback * cb = nullptr);

  void draw_tilemap  (const Tilemap & tilemap);
  void draw_tilemap  (Tilemap && tilemap);

  // only used by main task
  void init();
  void deinit();

  bool handle_sdl_event(const SDL_Event * event);
}

#endif
