#ifndef GFX_H
#define GFX_H

#include <SDL2/SDL_events.h>

// public, thread-safe messaging api
void gfx_create_window(
    int width,
    int height,
    int fullscreen,
    void (* callback)(int success, void * userdata),
    void * userdata
);

void gfx_flush(
    void (* callback)(void * userdata),
    void * userdata
);

void gfx_load_texture_rgba(
    int size_x,
    int size_y,
    void * pixels,
    void (* callback)(unsigned int tex_id, void * userdata),
    void * userdata
);

void gfx_unload_texture(
    unsigned int tex_id
);

// only used by main task
void gfx_init();
void gfx_deinit();
int  gfx_handle_sdl_event(const SDL_Event * event);

#endif
