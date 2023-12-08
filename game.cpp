#include "game.hpp"
#include "screen.hpp"
#include "atlas.hpp"

RummageLeaves::RummageLeaves()
  : AnimExpand{1.05f, 100}, Timed{1000}, pos{0}
{ }

void RummageLeaves::on_timer_finish() const
{
  game.inventory.add_random_item();
  atlas.rummage_leaves.finish();
}

void RummageLeaves::on_hover()
{
  AnimExpand::on_hover();
}

void RummageLeaves::on_click()
{
  if (screen.mouse_event.type != SDL_MOUSEBUTTONDOWN) return;
  Timed::start();
  atlas.rummage_leaves.start();
}

void RummageLeaves::draw()
{
  SDL_Rect r = {pos.x, pos.y, atlas.rummage_leaves.frame_width, atlas.rummage_leaves.frame_height};
  Clickable::handle_region(r);
  atlas.rummage_leaves.draw(screen.renderer, AnimExpand::transform(r));
}

Game::Game()
  : prev_grid_id(-1),
    grid_fill_mode(false),
    inventory{4, 7, 32, 8, {235, 122, 47, 255}},
    crafting_grid{3, 3, 32, 8},
    crafting_result{1, 1, 32, 8, {245, 49, 150, 255}},
    arrow_pos{0}
{
  on_resize();
}

void Game::update(float dt)
{
  // TODO: maybe a better way to manage updates?
  // like store in a vector "Updatable" instances or something
  
  atlas.arrow.update(dt);

  rummage_leaves.AnimExpand::update(dt);
  rummage_leaves.Timed::update(dt);
  atlas.rummage_leaves.update(dt);

  int grid_end_x = crafting_grid.pos.x + crafting_grid.total_width();
  int w = crafting_result.pos.x - grid_end_x;

  arrow_pos.x = grid_end_x + (w - atlas.arrow.frame_width) / 2;
  arrow_pos.y = crafting_result.pos.y + (crafting_result.total_height() - atlas.arrow.frame_height) / 2;
}

void Game::draw()
{
  inventory.draw();
  crafting_grid.draw();
  atlas.arrow.draw(screen.renderer, arrow_pos);
  crafting_result.draw();
  rummage_leaves.draw();
}

void Game::on_resize()
{
  inventory.set_pos({(screen.width - inventory.total_width()) / 2,
		     (screen.height - inventory.total_height()) / 3});

  crafting_grid.set_pos({inventory.pos.x + inventory.cell_size() + Grid::line_thickness, inventory.pos.y + inventory.total_height() + 40});
  crafting_result.set_pos({crafting_grid.pos.x + crafting_grid.total_width() + crafting_grid.cell_size(), crafting_grid.pos.y + crafting_grid.cell_size() + Grid::line_thickness});

  rummage_leaves.pos = {inventory.pos.x - atlas.rummage_leaves.frame_width - 20, inventory.pos.y};
}
