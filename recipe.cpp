#include "recipe.hpp"

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

Item check_recipe(std::vector<Item>& items)
{
  // I guess there will be 4x4 recipes in the future, so vector makes more sense
  if (items.size() != 9) panic();

  for (const auto& recipe : recipes) {
    bool found = true;

    for (auto i = 0; i < 9; i++) {
      if (items[i] && items[i].type == recipe.grid[i]);
      else if (!items[i] && recipe.grid[i] == Item::None);
      else found = false;
    }

    if (found) return recipe.result;
  }

  return Item{};
}

std::vector<Recipe> recipes = {
  Recipe{{Item::Branch, 2},
	 "   ",
	 " LL",
	 " L ",
	 {{'L', Item::Leaf}}
  }
};
