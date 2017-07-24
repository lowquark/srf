
#include <SDL2/SDL.h>

#include <queue.h>

#include <script/script.h>
#include <gfx/gfx.h>

#include <assert.h>

int main(int argc, char ** argv) {
  squeue_test();
  queue_test();

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

  gfx_init();
  script_init();

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

      if(script_handle_sdl_event(&event)) {}
      else if(gfx_handle_sdl_event(&event)) {}
    }
  }

  script_deinit();
  gfx_deinit();

  SDL_Quit();
}

