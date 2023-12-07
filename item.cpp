#include "item.hpp"
#include "atlas.hpp"
#include "game.hpp"

Item::Item() : type(None), count(0)
{ }

Item::Item(Type type, int count)
  : Item()
{
  this->type = type;
  this->count = count;
}

void Item::draw(Grid& gr, V2g gpos)
{
  if (count <= 0) return;

  SDL_Rect r = gr.cell_to_rect(gpos);
  SDL_Rect dst = {r.x + gr.item_pad, r.y + gr.item_pad, gr.item_size, gr.item_size};
  atlas.items.at(type).draw(screen.renderer, dst);

  // TODO: proper font rendering interface
  std::string str = std::to_string(count);
  SDL_Surface* text_surface = TTF_RenderText_Blended(screen.font, str.c_str(), {191, 173, 172, 255});
  SDL_Texture* text_texture = SDL_CreateTextureFromSurface(screen.renderer, text_surface);
  SDL_FreeSurface(text_surface);

  int w, h;
  TTF_SizeText(screen.font, str.c_str(), &w, &h);
  
  SDL_Rect text_dst = {r.x + gr.cell_size() - gr.item_pad - w, r.y + gr.cell_size() - gr.item_pad - h, w, h};
  SDL_RenderCopy(screen.renderer, text_texture, NULL, &text_dst);

  SDL_DestroyTexture(text_texture);
}

void Item::draw(Grid& gr, V2 pos)
{
  atlas.items.at(type).draw(screen.renderer, pos);

  // TODO: proper font rendering interface
  std::string str = std::to_string(count);
  SDL_Surface* text_surface = TTF_RenderText_Blended(screen.font, str.c_str(), {191, 173, 172, 255});
  SDL_Texture* text_texture = SDL_CreateTextureFromSurface(screen.renderer, text_surface);
  SDL_FreeSurface(text_surface);

  int w, h;
  TTF_SizeText(screen.font, str.c_str(), &w, &h);

  SDL_Rect text_dst = {pos.x + gr.cell_size() - gr.item_pad - w, pos.y + gr.cell_size() - gr.item_pad - h, w, h};
  SDL_RenderCopy(screen.renderer, text_texture, NULL, &text_dst);

  SDL_DestroyTexture(text_texture);
}

void Item::draw(Grid& gr, V2g gpos, SDL_Color bg)
{
  SDL_SetRenderDrawColor(screen.renderer, SDL_COLOR_EXPAND(bg));
  SDL_Rect r = gr.cell_to_rect(gpos);
  SDL_RenderFillRect(screen.renderer, &r);
  draw(gr, gpos);
}

Item::operator bool() const
{
  return count > 0;
}

const char* Item::type_stringify(Item::Type type)
{
  using enum Item::Type;

  switch (type) {
  case None: return "None";
  case Leaf: return "Leaf";
  case Branch: return "Branch";
  case Sapling: return "Sapling";
  case Log: return "Log";   
  case Count: return "Count";
  default: panic("Unknown Item::Type of `", static_cast<int>(type), "`");
  }
}

std::ostream& operator <<(std::ostream& out, const Item& item)
{
  return out << "Item{" << Item::type_stringify(item.type) << ", " << item.count << "}";
}
