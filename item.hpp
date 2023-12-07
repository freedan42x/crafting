#ifndef ITEM_HPP_
#define ITEM_HPP_

#include "screen.hpp"
#include "clickable.hpp"
#include "anim_expand.hpp"

class Grid;

class Item : public Clickable<Item>, public AnimExpand
{
public:
  enum Type
  {
    None,
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

  void on_mouse_left(Grid& gr);
  void on_mouse_right(Grid& gr);

  void update(float dt);
  void on_hover(Grid& gr, V2g gpos);
  void on_click(Grid& gr, V2g gpos);
  void draw(Grid& gr, V2g gpos);
  void draw(Grid& gr, V2 pos);

  operator bool() const;
  friend std::ostream& operator <<(std::ostream& out, const Item& item);
};

#endif
