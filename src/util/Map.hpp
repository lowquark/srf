#ifndef MAP_HPP
#define MAP_HPP

#include "Vec2.hpp"
#include <vector>
#include <cstdio>

template <typename T>
class Map {
  std::vector<T> _data;
  Vec2u _size;

  public:
  Map() = default;
  Map(Vec2u size, const T & fill = T()) {
    resize(size, fill);
  }

  void clear() {
    _size = Vec2u(0, 0);
    _data.clear();
  }
  void resize(Vec2u size, const T & fill = T()) {
    _size = size;
    _data.resize(_size.x * _size.y, fill);
  }

  bool valid(Vec2i pos) const {
    if(pos.x < 0) { return false; }
    if((unsigned int)pos.x >= _size.x) { return false; }
    if(pos.y < 0) { return false; }
    if((unsigned int)pos.y >= _size.y) { return false; }

    return true;
  }
  unsigned int index(Vec2i pos) const {
    return pos.x + pos.y*_size.x;
  }
  Vec2i coord(unsigned int index) {
    return Vec2i(index % _size.x, index / _size.x);
  }

  const T & get(Vec2i pos, const T & _default = T()) const {
    if(valid(pos)) {
      return _data[index(pos)];
    }
    return _default;
  }
  void set(Vec2i pos, const T & t) {
    if(valid(pos)) {
      _data[index(pos)] = t;
    }
  }

  Vec2u size() const { return _size; }
  const std::vector<T> & data() const { return _data; }
};

#endif
