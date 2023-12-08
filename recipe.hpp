#ifndef RECIPE_HPP_
#define RECIPE_HPP_

#include "item.hpp"
#include <unordered_map>
#include <vector>

// TODO shapeless recipes
struct Recipe
{
  std::array<Item::Type, 9> grid;
  Item result;

  Recipe(Item result, const char* row1, const char* row2, const char* row3, std::unordered_map<char, Item::Type> binds);
};

Item check_recipe(std::vector<Item>& items);

extern std::vector<Recipe> recipes;

#endif
