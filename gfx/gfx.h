#ifndef GFX_H
#define GFX_H

#include <SDL2/SDL_events.h>

// public, thread-safe messaging api
void gfx_create_window(int w, int h, int fs, void (*cb)(int, void *), void * p);
void gfx_flush(void (*cb)(void *), void * p);

// only used by main task
void gfx_init();
void gfx_deinit();
int  gfx_handle_sdl_event(const SDL_Event * event);

#endif
