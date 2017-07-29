
#include "gfx.hpp"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cassert>

#include <queue>
#include <Image.hpp>

namespace gfx {
  static SDL_Window * window = NULL;
  static SDL_GLContext gl_ctx = NULL;

  static Uint32 gfx_event = (Uint32)-1;

  void init() {
    if(gfx_event == (Uint32)-1) {
      gfx_event = SDL_RegisterEvents(1);
      assert(gfx_event != (Uint32)-1);
    }
  }
  void deinit() {
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

  class Action {
    public:
    virtual ~Action() = default;
    virtual void operator()() {};
  };
  class CreateWindowAction : public Action {
    public:
    CreateWindowAction(int width, int height, bool fullscreen)
      : width(width)
      , height(height)
      , fullscreen(fullscreen)
      , callback(nullptr) {}
    CreateWindowAction(int width, int height, bool fullscreen, std::unique_ptr<CreateWindowCallback> && callback)
      : width(width)
      , height(height)
      , fullscreen(fullscreen)
      , callback(std::forward<decltype(callback)>(callback)) {}

    void operator()() override {
      bool success = create_window("srf", width, height);

      if(callback) { (*callback)(success); }
    }

    private:
    int width;
    int height;
    bool fullscreen;

    std::unique_ptr<CreateWindowCallback> callback;
  };
  class FlushAction : public Action {
    public:
    FlushAction() = default;
    FlushAction(std::unique_ptr<FlushCallback> && callback)
      : callback(std::forward<decltype(callback)>(callback)) {}

    void operator()() override {
      if(window) {
        SDL_GL_SwapWindow(window);
      }

      if(callback) { (*callback)(); }
    }

    private:
    std::unique_ptr<FlushCallback> callback;
  };

  class LoadTextureAction : public Action {
    public:

    LoadTextureAction(const Image & image, LoadTextureCallback * callback)
      : image(image)
      , callback(callback) {}

    void operator()() override {
      if(gl_ctx) {
        GLuint tex_id = 0;
        glGenTextures(1, &tex_id);
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     image.width(),
                     image.height(),
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     image.pixels());

        (*callback)(tex_id);
      }
    }

    private:
    Image image;
    LoadTextureCallback * callback;
  };
  class UnloadTextureAction : public Action {
    public:
    UnloadTextureAction(unsigned int tex_id)
      : tex_id(tex_id) {}

    void operator()() override {
      if(gl_ctx) {
        glDeleteTextures(1, &tex_id);
        printf("gfx: unloaded texture %u\n", tex_id);
      }
    }

    private:
    unsigned int tex_id;
  };

  static void emit_sdl_event(Action * data1) {
    SDL_Event event;
    SDL_zero(event);
    event.type = gfx_event;
    event.user.code = 0;
    event.user.data1 = (void *)data1;
    event.user.data2 = NULL;
    SDL_PushEvent(&event);
  }

  void create_window(int w, int h, bool fs, std::unique_ptr<CreateWindowCallback> && callback) {
    emit_sdl_event(new CreateWindowAction(w, h, fs, std::forward<decltype(callback)>(callback)));
  }
  void flush(std::unique_ptr<FlushCallback> && callback) {
    emit_sdl_event(new FlushAction(std::forward<decltype(callback)>(callback)));
  }

  bool handle_sdl_event(const SDL_Event * event) {
    if(event->type == gfx_event) {
      Action * action = (Action *)event->user.data1;

      (*action)();

      delete action;

      return true;
    } else {
      return false;
    }
  }
}

