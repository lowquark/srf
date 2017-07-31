#ifndef GFX_H
#define GFX_H

#include <gfx/Tilemap.hpp>
#include <SDL2/SDL_events.h>
#include <memory>

namespace gfx {
  // callbacks are called from a possibly separate thread
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

  template <typename T>
  using uptr = std::unique_ptr<T>;
  template <typename T>
  using sptr = std::shared_ptr<T>;
  template <typename T>
  using wptr = std::shared_ptr<T>;

  // public, thread-safe messaging
  void create_window  (int w, int h, bool fs, uptr<CreateWindowCallback> && callback = nullptr);
  void flush          (uptr<FlushCallback> && callback = nullptr);

  void draw_tilemap   (const Tilemap & tilemap);
  void draw_tilemap   (Tilemap && tilemap);

  // only used by main task
  void init();
  void deinit();

  bool handle_sdl_event(const SDL_Event * event);
}

#endif
