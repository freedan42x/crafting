#include "screen.hpp"
#include "game.hpp"
#include "util.hpp"

Game game;

int main()
{
  // TODO: grid flag to forbid putting items in them, i.e. crafting_result
  // TODO: double-click to collect items across several grids (i.e. inventory and crafting_grid)
  // TODO: if you do triple-click (i.e. want to collect all items and immediately put selected item back into slot, it doesn't put it, you have to wait since double-click)
  // TODO: shift-click for crafting_grid and crafting_result to quickly transfer them to inventory
  // TODO: make add_item() handle if inventory has that item type already, in that case add to that slot

  // *TODO: recipe book
  // *TODO: 2nd tab for lab-like tree growing
  screen.main_loop();
}
