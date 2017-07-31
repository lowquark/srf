
#include "gfx.hpp"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cassert>

#include <queue>
#include <util/Image.hpp>
#include <util/load_png.hpp>
#include <gfx/draw.hpp>

namespace gfx {
  static SDL_Window * window = NULL;
  static SDL_GLContext gl_ctx = NULL;

  static Uint32 gfx_action_event = (Uint32)-1;

  uptr<TilemapShader> tilemap_shader;
  sptr<gl::Texture> tileset;


  sptr<gl::Texture> get_texture(const std::string & uri) {
    return tileset;
  }
  Vec2u get_tileset_size(const std::string & uri) {
    return Vec2u(16, 16);
  }
  Vec2u get_tileset_tile_size(const std::string & uri) {
    return Vec2u(16, 16);
  }


  static void init_gl(Vec2u window_size) {
    glewInit();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);

    draw::set_window_size(window_size);

    tilemap_shader.reset(
        new TilemapShader(
          "#version 130\n\n"
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
          "uniform sampler2D tileset;\n"
          "uniform sampler2D fg_color;\n"
          "uniform sampler2D bg_color;\n"
          "uniform sampler2D index_data;\n"
          "uniform ivec2 tilemap_size;\n"
          "uniform ivec2 tileset_size;\n"
          "\n"
          "varying vec2 texcoord;\n"
          "void main() { \n"
          "vec4 fg = texture(fg_color, texcoord);\n"
          "vec4 bg = texture(bg_color, texcoord);\n"
          "vec2 tileset_coord = texture(index_data, texcoord).xy * 255/256;\n"
          "\n"
          "vec2 tile_local_texcoord = texcoord*tilemap_size - floor(texcoord*tilemap_size);\n"
          "vec2 tileset_texcoord = tileset_coord + tile_local_texcoord/tileset_size;\n"
          "vec4 tile_color = texture(tileset, tileset_texcoord);\n"
          "\n"
          "if(abs(tile_color.r - tile_color.g) < 0.001 && \n"
          "   abs(tile_color.g - tile_color.b) < 0.001) {\n"
          "gl_FragColor = bg + tile_color.r*(fg - bg);\n"
          "} else {\n"
          "gl_FragColor = tile_color;\n"
          "}\n"
          "}\n")); // lol

    Image tileset_image;
    load_png(tileset_image, "tiles.png");
    tileset.reset(new gl::Texture(tileset_image));
  }

  static void deinit_gl() {
    tilemap_shader.reset();
    tileset.reset();
  }

  void init() {
    if(gfx_action_event == (Uint32)-1) {
      gfx_action_event = SDL_RegisterEvents(1);
      assert(gfx_action_event != (Uint32)-1);
    }
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
    CreateWindowAction(int width, int height, bool fullscreen, uptr<CreateWindowCallback> && callback)
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

    uptr<CreateWindowCallback> callback;
  };
  class FlushAction : public Action {
    public:
    FlushAction() = default;
    FlushAction(uptr<FlushCallback> && callback)
      : callback(std::forward<decltype(callback)>(callback)) {}

    void operator()() override {
      if(window) {
        SDL_GL_SwapWindow(window);
      }

      if(callback) { (*callback)(); }
    }

    private:
    uptr<FlushCallback> callback;
  };
  class DrawTilemapAction : public Action {
    public:
    DrawTilemapAction(const Tilemap & tilemap) : tilemap(tilemap) {}
    DrawTilemapAction(Tilemap && tilemap) : tilemap(std::forward<decltype(tilemap)>(tilemap)) {}

    void operator()() override {
      tilemap_shader->draw(tilemap, Vec2i(0, 0));
    }

    private:
    Tilemap tilemap;
  };

  template <typename T, typename ... Args>
  static typename std::enable_if<std::is_base_of<Action, T>::value, void>::type
  enqueue_action(Args ... args) {
    SDL_Event event;
    SDL_zero(event);
    event.type = gfx_action_event;
    event.user.code = 0;
    event.user.data1 = (void *)new T(std::forward<Args>(args)...);
    event.user.data2 = NULL;
    SDL_PushEvent(&event); // alledgedly thread-safe
  }

  void create_window(int w, int h, bool fs, uptr<CreateWindowCallback> && callback) {
    enqueue_action<CreateWindowAction>(w, h, fs, std::forward<decltype(callback)>(callback));
  }
  void flush(uptr<FlushCallback> && callback) {
    enqueue_action<FlushAction>(std::forward<decltype(callback)>(callback));
  }

  void draw_tilemap(const Tilemap & tilemap) {
    enqueue_action<DrawTilemapAction>(tilemap);
  }
  void draw_tilemap(Tilemap && tilemap) {
    enqueue_action<DrawTilemapAction>(std::forward<decltype(tilemap)>(tilemap));
  }

  bool handle_sdl_event(const SDL_Event * event) {
    if(event->type == gfx_action_event) {
      Action * action = (Action *)event->user.data1;

      (*action)();

      delete action;

      return true;
    }

    return false;
  }
}

