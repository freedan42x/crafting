#ifndef GAME_HPP_
#define GAME_HPP_

#include "timed.hpp"
#include "grid.hpp"
#include "SDL2/SDL.h"

class RummageLeaves : public Clickable<RummageLeaves>, public AnimExpand, public Timed<RummageLeaves>
{
public:
  V2 pos;

  RummageLeaves();

  void on_timer_finish() const;
  void on_hover();
  void on_click();
  void draw();
};

class Game
{
  V2 arrow_pos;

public:
  int prev_grid_id;
  Item selected_item;
  bool grid_fill_mode;

  Grid inventory;
  CraftingGrid crafting_grid;
  ResultGrid crafting_result;

  RummageLeaves rummage_leaves;

  Game();

  void update(float dt);
  void draw();
  void on_resize();
};

extern Game game;

#endif
