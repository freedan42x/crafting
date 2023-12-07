#ifndef ATLAS_HPP_
#define ATLAS_HPP_

#include "item.hpp"
#include "texture.hpp"
#include <array>

class Atlas
{
  static std::string item_path(Item::Type type);

public:
  Atlas();
  void load(SDL_Renderer* r);

  TextureAnim arrow;
  
  std::array<Texture, Item::Count> items;
  TextureAnim rummage_leaves;
};

extern Atlas atlas;

#endif
