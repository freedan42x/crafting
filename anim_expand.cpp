#include "anim_expand.hpp"

AnimExpand::AnimExpand(float scale, Uint32 duration)
  : scale(scale), cur_scale(1.0f),
    step{(scale - 1) * 1000 / duration}
{
}

void AnimExpand::update(float dt)
{
  if (!direction) return;

  cur_scale += direction * step * dt;
  if (cur_scale < 1.0f) {
    cur_scale = 1.0f;
    direction = 0;
  } else if (cur_scale > scale) {
    cur_scale = scale;
    direction = 0;
  }
}

void AnimExpand::on_hover()
{
  direction = 1;
}

void AnimExpand::on_hover_end()
{
  direction = -1;
}

SDL_Rect AnimExpand::transform(SDL_Rect dst)
{
  if (cur_scale == 1.0f) return dst;

  int dw = static_cast<float>(dst.w) * (cur_scale - 1.0f);
  int dh = static_cast<float>(dst.h) * (cur_scale - 1.0f);

  return {dst.x - dw / 2, dst.y - dh / 2, dst.w + dw, dst.h + dh};
}
