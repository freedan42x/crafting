#include "item.hpp"
#include "atlas.hpp"
#include "game.hpp"

Item::Item() : AnimExpand{1.1f, 100}, count(0)
{ }

Item::Item(Type type, int count)
  : Item()
{
  this->type = type;
  this->count = count;
}

void Item::on_mouse_left(Grid& gr)
{
  if (screen.double_click) {
    if (game.selected_item && !*this) {
      for (auto& item : gr.items) {
	if (item.type == game.selected_item.type) {
	  game.selected_item.count += item.count;
	  item.count = 0;
	}
      }
    } else if (!game.selected_item && *this) {
      int total_count = 0;
      for (auto& item : gr.items) {
	if (item.type == type) {
	  total_count += item.count;
	  item.count = 0;
	}
      }
      count = total_count;
      std::swap(game.selected_item, *this);
    }
    return;
  }

  if (gr.is_result_grid) {
    if (*this && !game.selected_item) {
      std::swap(game.selected_item, *this);
    }
    return;
  }

  if (*this && game.selected_item && type == game.selected_item.type) {
    count += game.selected_item.count;
    game.selected_item.count = 0;
  } else {
    std::swap(game.selected_item, *this);
  }
}

void Item::on_mouse_right(Grid& gr)
{
  if (gr.is_result_grid) return;
  
  if (count <= 0) {
    if (game.selected_item) {
      *this = game.selected_item;
      count = 1;
      game.selected_item.count--;
    }
    return;
  }

  if (game.selected_item) {
    if (type == game.selected_item.type) {
      count++;
      game.selected_item.count--;
    } else {
      std::swap(game.selected_item, *this);
    }
    return;
  }

  if (count == 1) {
    std::swap(game.selected_item, *this);
  } else {
    game.selected_item = *this;
    game.selected_item.count /= 2;
    count -= game.selected_item.count;
  }
}

void Item::update(float dt)
{
  if (count <= 0) return;

  AnimExpand::update(dt);
}

void Item::on_hover(Grid& gr, V2g gpos)
{
  AnimExpand::on_hover();

  if (gr.prev_cursor_pos == gpos) return;

  if (game.grid_fill_mode && game.selected_item && !gr.is_marked(gpos)) {
    if (!*this) {
      *this = game.selected_item;
      count = 1;
      game.selected_item.count--;
      gr.mark(gpos);
      gr.prev_cursor_pos = gpos;
    } else if (type == game.selected_item.type) {
      count++;
      game.selected_item.count--;
      gr.mark(gpos);
      gr.prev_cursor_pos = gpos;
    }
  }
}

void Item::on_click(Grid& gr, V2g gpos)
{
  bool down = screen.mouse_event.type == SDL_MOUSEBUTTONDOWN;
  bool up = screen.mouse_event.type == SDL_MOUSEBUTTONUP;
  bool left = screen.mouse_event.button == SDL_BUTTON_LEFT;
  bool right = screen.mouse_event.button == SDL_BUTTON_RIGHT;

  if (down) {
    if (left) on_mouse_left(gr);
    else if (right && !gr.is_marked(gpos)) on_mouse_right(gr);
    else return;

    gr.prev_cursor_pos = gpos;
    game.prev_grid_id = gr.id;

  } else if (up) {
    if (gpos == gr.prev_cursor_pos && game.prev_grid_id == gr.id) return;

    if (left) on_mouse_left(gr);
  }
}

void Item::draw(Grid& gr, V2g gpos)
{
  SDL_Rect r = gr.grid_to_rect(gpos);
  Clickable::handle_region(r, gr, gpos);

  if (count <= 0) return;

  SDL_Rect dst = {r.x + gr.item_pad, r.y + gr.item_pad, r.w - 2 * gr.item_pad, r.h - 2 * gr.item_pad};
  atlas.items.at(type).draw(screen.renderer, AnimExpand::transform(dst));

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

Item::operator bool() const
{
  return count > 0;
}

static const char* item_type_str(Item::Type type)
{
  using enum Item::Type;

  switch (type) {
  case None: return "None";
  case Leaf: return "Leaf";
  case Branch: return "Branch";
  case Sapling: return "Sapling";
  case Log: return "Log";   
  case Count: return "Count";
  default: panic("Unknown Item::Type of `", static_cast<std::size_t>(type), "`");
  }
}

std::ostream& operator <<(std::ostream& out, const Item& item)
{
  return out << "Item{" << item_type_str(item.type) << ", " << item.count << "}";
}
