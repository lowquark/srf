#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <SDL2/SDL_events.h>

namespace script {
  void init();
  void deinit();

  bool handle_sdl_event(const SDL_Event * event);
}

#endif
