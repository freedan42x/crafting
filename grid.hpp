#ifndef GRID_HPP_
#define GRID_HPP_

#include "item.hpp"
#include "SDL2/SDL.h"
#include <vector>
#include <optional>

class Grid : public Clickable<Grid>
{
  int rows, cols;
  SDL_Color line_color;
  V2g cur_hover_pos;
  std::vector<bool> fill_mode_marks;

  void on_mouse_left(V2g gpos);
  void on_mouse_right(V2g gpos);
  
public:
  static constexpr auto line_thickness = 2;
  
  Grid(int rows, int cols, int item_size, int item_pad, SDL_Color line_color={209, 201, 178, 255});

  std::vector<Item> items;
  // TODO class ResultGrid : Grid with crafting support
  bool is_result_grid; // meaning you can't put items in it, only take

  int id;
  V2 pos;
  V2g prev_click_pos;

  int item_size;
  int item_pad;

  int cell_size() const;
  int total_width() const;
  int total_height() const;

  bool add_item(Item item);
  bool add_random_item();

  Item& item_at(V2g gp);
  const Item& item_at(V2g gp) const;

  bool is_marked(V2g gp) const;
  void mark(V2g gp);
  void clear_marks();
  
  void set_pos(V2 p);
  SDL_Rect cell_to_rect(V2g gp) const;

  void on_hover(V2g gpos);
  void on_click(V2g gpos);
  void draw();
};

#endif
