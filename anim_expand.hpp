#ifndef ANIM_EXPAND_HPP
#define ANIM_EXPAND_HPP

#include "SDL2/SDL.h"

class AnimExpand
{
  float scale;
  float cur_scale;
  float step;
  int direction;

public:
  AnimExpand(float scale, Uint32 duration);

  void update(float dt);
  void on_hover();
  void on_hover_end();
  SDL_Rect transform(SDL_Rect dst);
};

#endif
