#ifndef SCRIPT_H
#define SCRIPT_H

#include <SDL2/SDL_events.h>

void script_init();
void script_deinit();

int script_handle_sdl_event(const SDL_Event * event);

#endif
