#include "grid.hpp"
#include "game.hpp"
#include "recipe.hpp"

Grid::Grid(int rows, int cols, int item_size, int item_pad, SDL_Color line_color)
  : rows(rows), cols(cols),
    line_color(line_color),
    prev_click_pos{-1}, cur_hover_pos{-1},
    pos{0},
    item_size(item_size), item_pad(item_pad)
{
  static int id_counter = 0;
  id = id_counter++;
  items.resize(rows * cols);
  fill_mode_marks.resize(rows * cols);
}

int Grid::cell_size() const
{
  return item_size + item_pad * 2;
}

int Grid::total_width() const
{
  return cols * cell_size() + (cols + 1) * line_thickness;
}

int Grid::total_height() const
{
  return rows * cell_size() + (rows + 1) * line_thickness;
}

bool Grid::add_item(Item item)
{
  for (auto& i : items) {
    if (!i) {
      i = item;
      return true;
    }
  }
  return false;
}

bool Grid::add_random_item()
{
  // TODO: change to C++ way
  Item::Type tp = static_cast<Item::Type>(rand() % Item::Count);
  return add_item({tp, 1});
}

Item& Grid::item_at(V2g gp)
{
  return items[gp.x + gp.y * cols];
}

const Item& Grid::item_at(V2g gp) const
{
  return items[gp.x + gp.y * cols];
}

bool Grid::is_marked(V2g gp) const
{
  return fill_mode_marks[gp.x + gp.y * cols];
}

void Grid::mark(V2g gp)
{
  fill_mode_marks[gp.x + gp.y * cols] = true;
}

void Grid::clear_marks()
{
  std::fill(fill_mode_marks.begin(), fill_mode_marks.end(), false);
}

void Grid::set_pos(V2 p)
{
  pos = p;
}

SDL_Rect Grid::cell_to_rect(V2g gp) const
{
  return {pos.x + gp.x * cell_size() + (gp.x + 1) * line_thickness,
	  pos.y + gp.y * cell_size() + (gp.y + 1) * line_thickness,
	  cell_size(),
	  cell_size()};
}

void Grid::on_mouse_left(V2g gpos)
{
  Item& item = item_at(gpos);

  if (screen.double_click) {
    if (game.selected_item && !item) {
      for (auto& it : items) {
	if (it && it.type == game.selected_item.type) {
	  game.selected_item.count += it.count;
	  it.count = 0;
	}
      }
    } else if (!game.selected_item && item) {
      Item collected{item.type, 0};
      for (auto& it : items) {
	if (it && it.type == item.type) {
	  collected.count += it.count;
	  it.count = 0;
	}
      }
      
      std::swap(game.selected_item, collected);
    }
    return;
  }

  if (item && game.selected_item && item.type == game.selected_item.type) {
    item.count += game.selected_item.count;
    game.selected_item.count = 0;
  } else {
    std::swap(game.selected_item, item);
  }
}

void Grid::on_mouse_right(V2g gpos)
{
  Item& item = item_at(gpos);
  
  if (!item) {
    if (game.selected_item) {
      item = game.selected_item;
      item.count = 1;
      game.selected_item.count--;
    }
    return;
  }

  if (game.selected_item) {
    if (item.type == game.selected_item.type) {
      item.count++;
      game.selected_item.count--;
    } else {
      std::swap(game.selected_item, item);
    }
    return;
  }

  if (item.count == 1) {
    std::swap(game.selected_item, item);
  } else {
    game.selected_item = item;
    game.selected_item.count /= 2;
    item.count -= game.selected_item.count;
  }
}

// TODO: right-click on item is now wrong (*)
void Grid::on_hover(V2g gpos)
{
  cur_hover_pos = gpos;

  // (*) cuz removed the line here
  // if (gpos == prev_click_pos) return;
  // but otherwise with fill mode you can actually put 2 items into same slot
  // if you start from prev_click_pos (cuz in that case it is not mark()`ed)

  Item& item = item_at(gpos);

  if (game.grid_fill_mode && game.selected_item && !is_marked(gpos)) {
    if (!item) {
      item = game.selected_item;
      item.count = 1;
      game.selected_item.count--;
      mark(gpos);
      prev_click_pos = gpos;
    } else if (item.type == game.selected_item.type) {
      item.count++;
      game.selected_item.count--;
      mark(gpos);
      prev_click_pos = gpos;
    }
  }
}

void Grid::on_click(V2g gpos)
{
  bool down = screen.mouse_event.type == SDL_MOUSEBUTTONDOWN;
  bool up = screen.mouse_event.type == SDL_MOUSEBUTTONUP;
  bool left = screen.mouse_event.button == SDL_BUTTON_LEFT;
  bool right = screen.mouse_event.button == SDL_BUTTON_RIGHT;

  if (down) {
    if (left) on_mouse_left(gpos);
    else if (right && !is_marked(gpos)) on_mouse_right(gpos);
    else return;

    prev_click_pos = gpos;
    game.prev_grid_id = id;

  } else if (up) {
    if (gpos == prev_click_pos && game.prev_grid_id == id) return;

    if (left) on_mouse_left(gpos);
  }
}

void Grid::draw()
{
  SDL_SetRenderDrawColor(screen.renderer, SDL_COLOR_EXPAND(line_color));

  for (auto gx = 0; gx <= cols; gx++) {
    SDL_Rect r = {pos.x + gx * (cell_size() + line_thickness),
		  pos.y,
		  line_thickness,
		  total_height()};
    SDL_RenderDrawRect(screen.renderer, &r);
  }

  for (auto gy = 0; gy <= rows; gy++) {
    SDL_Rect r = {pos.x,
		  pos.y + gy * (cell_size() + line_thickness),
		  total_width(),
		  line_thickness};
    SDL_RenderDrawRect(screen.renderer, &r);
  }

  cur_hover_pos = {-1};
  
  for (auto x = 0; x < cols; x++) {
    for (auto y = 0; y < rows; y++) {
      V2g gpos{x, y};

      SDL_Rect r = cell_to_rect(gpos);
      Clickable::handle_region(r, gpos);

      if (cur_hover_pos == gpos) {
	item_at(gpos).draw(*this, gpos, {85, 66, 94, 255});
      } else {
	item_at(gpos).draw(*this, gpos);
      }
    }
  }

  if (game.selected_item) {
    game.selected_item.draw(*this, screen.mouse);
  }
}

CraftingGrid::CraftingGrid(int rows, int cols, int item_size, int item_pad, SDL_Color line_color)
  : Grid{rows, cols, item_size, item_pad, line_color}
{ }

void CraftingGrid::on_hover(V2g gpos)
{
  V2g cur_click_pos = prev_click_pos;
  Grid::on_hover(gpos);

  if (prev_click_pos != cur_click_pos) {
    game.crafting_result.refresh();
  }
}

void CraftingGrid::on_click(V2g gpos)
{
  Grid::on_click(gpos);

  game.crafting_result.refresh();
}

ResultGrid::ResultGrid(int rows, int cols, int item_size, int item_pad, SDL_Color line_color)
  : Grid{rows, cols, item_size, item_pad, line_color}
{ }

void ResultGrid::refresh()
{
  items[0] = check_recipe(game.crafting_grid.items);
}

void ResultGrid::on_hover(V2g gpos)
{
  cur_hover_pos = gpos;
}

void ResultGrid::on_click(V2g gpos)
{
  bool down = screen.mouse_event.type == SDL_MOUSEBUTTONDOWN;
  bool left = screen.mouse_event.button == SDL_BUTTON_LEFT;

  if (down && left) {
    game.prev_grid_id = id;
    
    Item& item = item_at(gpos);
    // TODO: if selected_item and of same type, add to it from result
    if (item && !game.selected_item) {
      std::swap(game.selected_item, item);
      for (auto& it : game.crafting_grid.items) {
	if (it) it.count--;
      }
      refresh();
    }
  }
}
