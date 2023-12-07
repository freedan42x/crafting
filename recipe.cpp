#include "recipe.hpp"
#include <cstring>

Recipe::Recipe(Item result, const char* row1, const char* row2, const char* row3, std::unordered_map<char, Item::Type> binds)
  : result(result)
{
  std::string rows = std::string{row1, 3} + std::string{row2, 3} + std::string{row3, 3};

  for (auto i = 0; i < 9; i++) {
    char c = rows[i];
    if (c == ' ') {
      grid[i] = Item::None;
    } else if (auto p = binds.find(c); p != binds.end()) {
      grid[i] = p->second;
    } else {
      panic("Unknown binding `", c, "` for ", result);
    }
  }
}

std::vector<Recipe> recipes = {
  Recipe{{Item::Branch, 2},
	 "   ",
	 " LL",
	 " L ",
	 {{'L', Item::Leaf}}
  }
};
