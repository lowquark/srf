
#include <SDL2/SDL.h>

#include <script/script.hpp>
#include <gfx/gfx.hpp>

int main(int argc, char ** argv) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

  gfx::init();
  script::init();

  int run = 1;
  while(run) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        run = 0;
      } else if(event.type == SDL_KEYDOWN) {
        if(event.key.keysym.sym == SDLK_0) {
          SDL_Event quit_event;
          SDL_zero(quit_event);
          quit_event.type = SDL_QUIT;
          SDL_PushEvent(&quit_event);
          continue;
        }
      }

      if(script::handle_sdl_event(&event)) {}
      else if(gfx::handle_sdl_event(&event)) {}
    }
  }

  script::deinit();
  gfx::deinit();

  SDL_Quit();
}

