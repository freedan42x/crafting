#include "util.hpp"

std::ostream& operator <<(std::ostream& out, const SDL_Rect& r)
{
  return out << "SDL_Rect{" << r.x << ", " << r.y << ", " << r.w << ", " << r.h << "}";
}

V2::V2(int x, int y)
  : x(x), y(y) { }

V2::V2(int s)
  : x(s), y(s) { }

bool V2::in(SDL_Rect r) const
{
  SDL_Point p{x, y};
  return SDL_PointInRect(&p, &r);
}

V2 V2::operator +(const V2& other) const
{
  return {x + other.x, y + other.y};
}

V2g::V2g(int s)
  : x(s), y(s) { }

V2g::V2g(int x, int y)
  : x(x), y(y) { }

bool V2g::operator ==(const V2g& other) const
{
  return x == other.x && y == other.y;
}

bool V2g::operator !=(const V2g& other) const
{
  return x != other.x || y != other.y;
}
