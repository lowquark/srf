#ifndef GFX_H
#define GFX_H

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
  class LoadTextureCallback {
    public:
    virtual ~LoadTextureCallback() = default;
    virtual void operator()(unsigned int tex_id) {};
  };

  void init();
  void deinit();

  // public, thread-safe messaging
  void create_window  (int w, int h, bool fs, std::unique_ptr<CreateWindowCallback> && callback = nullptr);
  void flush          (std::unique_ptr<FlushCallback> && callback = nullptr);

  // only used by main task
  bool handle_sdl_event(const SDL_Event * event);
}

#endif
