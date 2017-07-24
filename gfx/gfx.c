
#include "gfx.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <assert.h>

#define GFX_CREATE_WINDOW 0
#define GFX_DRAW          1

typedef struct gfx_create_window_event {
  unsigned int type;
  int size_x;
  int size_y;
  int fullscreen;
  void (*on_finish)(int succ, void *);
  void * data;
} gfx_create_window_event_t;
typedef struct gfx_draw_event {
  unsigned int type;
  void (*on_finish)(void *);
  void * data;
} gfx_draw_event_t;

typedef union gfx_event {
  unsigned int type;
  gfx_create_window_event_t create_window;
  gfx_draw_event_t draw;
} gfx_event_t;

static Uint32 gfx_event = (Uint32)-1;
static void emit_sdl_event(Uint32 code, void * data1, void * data2) {
  SDL_Event event;
  SDL_zero(event);
  event.type = gfx_event;
  event.user.code = code;
  event.user.data1 = data1;
  event.user.data2 = data2;
  SDL_PushEvent(&event);
}

void gfx_create_window(int w, int h, int fs, void (*cb)(int, void *), void * p) {
  gfx_event_t * newevent = calloc(1, sizeof(*newevent));
  newevent->type = GFX_CREATE_WINDOW;
  newevent->create_window.size_x = w;
  newevent->create_window.size_y = h;
  newevent->create_window.fullscreen = fs;
  newevent->create_window.on_finish = cb;
  newevent->create_window.data = p;

  emit_sdl_event(0, newevent, NULL);
}
void gfx_flush(void (*cb)(void *), void * p) {
  gfx_event_t * newevent = calloc(1, sizeof(*newevent));
  newevent->type = GFX_DRAW;
  newevent->draw.on_finish = cb;
  newevent->draw.data = p;

  emit_sdl_event(0, newevent, NULL);
}


SDL_Window * window = NULL;
SDL_GLContext gl_ctx = NULL;

void gfx_init() {
  if(gfx_event == (Uint32)-1) {
    gfx_event = SDL_RegisterEvents(1);
    assert(gfx_event != (Uint32)-1);
  }
}
void gfx_deinit() {
  if(window) {
    if(gl_ctx) {
      SDL_GL_DeleteContext(gl_ctx);
      gl_ctx = NULL;
    }

    SDL_DestroyWindow(window);
    window = NULL;
  }
}


static int create_window(const char * name, int w, int h) {
  if(window) {
    SDL_DestroyWindow(window);
    window = NULL;
  }

  window = SDL_CreateWindow(
    "srf",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    w,
    h,
    SDL_WINDOW_OPENGL
  );

  if(window) {
    gl_ctx = SDL_GL_CreateContext(window);

    if(gl_ctx) {
      glewInit();
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      SDL_GL_SwapWindow(window);

      return 1;
    }

    SDL_DestroyWindow(window);
    window = NULL;
  }

  return 0;
}

int gfx_handle_sdl_event(const SDL_Event * event) {
  if(event->type == gfx_event) {
    gfx_event_t * gev = (gfx_event_t *)event->user.data1;

    if(gev->type == GFX_CREATE_WINDOW) {
      int succ = create_window(
        "srf", 
        gev->create_window.size_x,
        gev->create_window.size_y
      );

      if(gev->create_window.on_finish) {
        gev->create_window.on_finish(succ, gev->create_window.data);
      }
    } else if(gev->type == GFX_DRAW) {
      if(gev->draw.on_finish) {
        gev->draw.on_finish(gev->draw.data);
      }
    }

    free(gev);
    gev = NULL;

    return 1;
  } else {
    return 0;
  }
}

