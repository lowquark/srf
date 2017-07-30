
#include <cstdio>

#include <gfx/gl/Texture.hpp>

namespace gfx {
  namespace gl {
    Texture::Texture() {
      glGenTextures(1, &_id);
    }
    Texture::Texture(const Image & image) {
      glGenTextures(1, &_id);
      load(image);
    }
    Texture::~Texture() {
      glDeleteTextures(1, &_id);
      _width = 0;
      _height = 0;
      _id = 0;
    }

    void Texture::load(const Image & image) {
      glDeleteTextures(1, &_id);
      glGenTextures(1, &_id);

      _width = image.width();
      _height = image.height();

      if(image.pixels()) {
        glBindTexture(GL_TEXTURE_2D, _id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels());
        // TODO: Rebind old texture?
        //glBindTexture(GL_TEXTURE_2D, old_id)
      }
    }
  }
}

