
#include "gfx.hpp"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cassert>

#include <queue>
#include <util/Image.hpp>
#include <gfx/draw.hpp>
#include <gfx/TileMap.hpp>

namespace gfx {
  static SDL_Window * window = NULL;
  static SDL_GLContext gl_ctx = NULL;

  static Uint32 gfx_event = (Uint32)-1;

  TileMapShader tile_map_shader;

  void init() {
    if(gfx_event == (Uint32)-1) {
      gfx_event = SDL_RegisterEvents(1);
      assert(gfx_event != (Uint32)-1);
    }
  }

  static void deinit_gl() {
    tile_map_shader.unload();
  }

  void deinit() {
    if(window) {
      if(gl_ctx) {
        deinit_gl();
        SDL_GL_DeleteContext(gl_ctx);
        gl_ctx = NULL;
      }

      SDL_DestroyWindow(window);
      window = NULL;
    }
  }

  static void init_gl(Vec2u window_size) {
    glewInit();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);

    draw::set_window_size(window_size);

    tile_map_shader.load("#version 130\n\n"
                         "in vec2 vertex_pos;\n"
                         "in vec2 vertex_texcoord;\n"
                         "varying vec2 texcoord;\n"
                         "void main() {\n"
                         "gl_Position.xy = vertex_pos;\n"
                         "gl_Position.z = 0.0;\n"
                         "gl_Position.w = 1.0;\n"
                         "texcoord = vertex_texcoord;\n"
                         "}",
                         "#version 130\n\n"
                         "uniform sampler2D tile_set;\n"
                         "uniform sampler2D fg_color;\n"
                         "uniform sampler2D bg_color;\n"
                         "uniform sampler2D index_data;\n"
                         "uniform ivec2 tile_map_size;\n"
                         "uniform ivec2 tile_set_size;\n"
                         "\n"
                         "varying vec2 texcoord;\n"
                         "void main() { \n"
                         "vec4 fg = texture(fg_color, texcoord);\n"
                         "vec4 bg = texture(bg_color, texcoord);\n"
                         "vec2 tile_set_coord = texture(index_data, texcoord).xy * 255/256;\n"
                         "\n"
                         "vec2 tile_local_texcoord = texcoord*tile_map_size - floor(texcoord*tile_map_size);\n"
                         "vec2 tile_set_texcoord = tile_set_coord + tile_local_texcoord/tile_set_size;\n"
                         "vec4 tile_color = texture(tile_set, tile_set_texcoord);\n"
                         "\n"
                         "if(abs(tile_color.r - tile_color.g) < 0.001 && \n"
                         "   abs(tile_color.g - tile_color.b) < 0.001) {\n"
                         "gl_FragColor = bg + tile_color.r*(fg - bg);\n"
                         "} else {\n"
                         "gl_FragColor = tile_color;\n"
                         "}\n"
                         "}\n"); // lol
  }

  static int create_window(const char * name, int w, int h) {
    if(window) {
      draw::set_window_size(Vec2u(0, 0));
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
        init_gl(Vec2u(w, h));
        return 1;
      }

      draw::set_window_size(Vec2u(0, 0));

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

