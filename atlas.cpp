#include "atlas.hpp"
#include "game.hpp"

const char* Atlas::item_paths[Item::Count] = {
  [Item::Leaf] = "assets/leaf.png",
  [Item::Branch] = "assets/branch.png",
  [Item::Sapling] = "assets/sapling.png",
  [Item::Log] = "assets/log.png"
};

Atlas::Atlas() { }

void Atlas::load(SDL_Renderer* r)
{
  arrow.load(r, "assets/arrow.png", 32, 32, 100);

  for (auto i = 0; i < Item::Count; i++) {
    items[i].load(r, item_paths[i]);
  }

  rummage_leaves.load(r, "assets/rummage_leaves.png", 150, 50);
  rummage_leaves.set_frame_count(12);
  rummage_leaves.set_total_duration(game.rummage_leaves.duration);
}
