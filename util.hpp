#ifndef UTIL_HPP_
#define UTIL_HPP_

#include "SDL2/SDL.h"
#include <iostream>

template <class... Args>
void panic_(const char* file, int line, const Args&... args)
{
  std::cerr << "PANIC at " << file << ":" << line << ": ";
  (std::cerr << ... << args) << std::endl;
  std::abort();
}

#define panic(...) panic_(__FILE__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

#define SDL_COLOR_EXPAND(color) (color).r, (color).g, (color).b, (color).a

std::ostream& operator <<(std::ostream& out, const SDL_Rect& r);

struct V2
{
  int x, y;
  V2(int s);
  V2(int x, int y);

  bool in(SDL_Rect r) const;
  
  V2 operator +(const V2& other) const;
};

struct V2g
{
  int x, y;
  V2g(int s);
  V2g(int x, int y);

  bool operator ==(const V2g& other) const;
  bool operator !=(const V2g& other) const;
};

#endif
