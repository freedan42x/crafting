#include "atlas.hpp"
#include "game.hpp"

std::string Atlas::item_path(Item::Type type)
{
  using enum Item::Type;
  if (type == None || type == Count) panic();

  std::string s = Item::type_stringify(type);
  s[0] = std::tolower(s[0]);

  return "assets/" + s + ".png";
};

Atlas::Atlas() { }

void Atlas::load(SDL_Renderer* r)
{
  arrow.load(r, "assets/arrow.png", 32, 32, 100);

  for (auto i = 0; i < Item::Count; i++) {
    items[i].load(r, item_path(static_cast<Item::Type>(i)).c_str());
  }

  rummage_leaves.load(r, "assets/rummage_leaves.png", 150, 50);
  rummage_leaves.set_frame_count(12);
  rummage_leaves.set_total_duration(game.rummage_leaves.duration);
}
