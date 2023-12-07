#include "grid.hpp"
#include "game.hpp"

Grid::Grid(int rows, int cols, int item_size, int item_pad, SDL_Color line_color)
  : rows(rows), cols(cols),
    line_color(line_color),
    is_result_grid(false),
    prev_cursor_pos{-1, -1},
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
    if (i.count <= 0) {
      i = item;
      return true;
    }
  }
  return false;
}

bool Grid::add_random_item()
{
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

SDL_Rect Grid::grid_to_rect(V2g gp) const
{
  return {pos.x + gp.x * cell_size() + (gp.x + 1) * line_thickness,
	  pos.y + gp.y * cell_size() + (gp.y + 1) * line_thickness,
	  cell_size(),
	  cell_size()};
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

  for (auto x = 0; x < cols; x++) {
    for (auto y = 0; y < rows; y++) {
      V2g gpos{x, y};

      item_at(gpos).draw(*this, gpos);
    }
  }

  if (game.selected_item) {
    game.selected_item.draw(*this, screen.mouse);
  }
}
