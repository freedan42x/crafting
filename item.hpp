#ifndef ITEM_HPP_
#define ITEM_HPP_

#include "screen.hpp"
#include "clickable.hpp"
#include "anim_expand.hpp"

class Grid;

class Item
{
public:
  enum Type
  {
    None = -1,
    Leaf,
    Branch,
    Sapling,
    Log,
   
    Count
  };

  Type type;
  int count;

  Item();
  Item(Type type, int count);

  void draw(Grid& gr, V2g gpos);
  void draw(Grid& gr, V2 pos);
  void draw(Grid& gr, V2g gpos, SDL_Color bg);

  operator bool() const;
  static const char* type_stringify(Item::Type type);
  friend std::ostream& operator <<(std::ostream& out, const Item& item);
};

#endif
