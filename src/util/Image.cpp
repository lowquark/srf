
#include "Image.hpp"

#include <algorithm>
#include <string.h>

Image::~Image() {
  delete [] _pixels;
}
Image::Image(unsigned int w, unsigned int h)
  : _width(w)
  , _height(h)
  , _pixels(nullptr) {
  _pixels = new uint8_t[w*h*4];
}

Image::Image(const Image & other) {
  unsigned int w = other._width;
  unsigned int h = other._height;

  _width = w;
  _height = h;
  _pixels = new uint8_t[w*h*4];

  std::copy(other._pixels, other._pixels + w*h*4, _pixels);
}
Image & Image::operator=(const Image & other) {
  if(this != &other) {
    unsigned int w = other._width;
    unsigned int h = other._height;

    delete [] _pixels;
    _width = w;
    _height = h;
    _pixels = new uint8_t[w*h*4];

    std::copy(other._pixels, other._pixels + w*h*4, _pixels);
  }

  return *this;
}
Image::Image(Image && other) {
  _width = other._width;
  _height = other._height;
  _pixels = other._pixels;
  other._width = 0;
  other._height = 0;
  other._pixels = nullptr;
}
Image & Image::operator=(Image && other) {
  if(this != &other) {
    delete [] _pixels;
    _width = other._width;
    _height = other._height;
    _pixels = other._pixels;
    other._width = 0;
    other._height = 0;
    other._pixels = nullptr;
  }
  return *this;
}

void Image::set_size(unsigned int w, unsigned int h) {
  delete [] _pixels;
  _width = w;
  _height = h;
  _pixels = new uint8_t[_width*_height*4];
  memset(_pixels, 0, _width*_height*4);
}

