#ifndef SCREEN_HPP_
#define SCREEN_HPP_

#include "util.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <array>

class Screen
{
  Uint32 last_click_timestamp;
  void draw() const;

public:
  int width, height;

  SDL_Window* window;
  SDL_Renderer* renderer;
  TTF_Font* font;

  int fps;

  V2 mouse;
  bool clicked;
  SDL_MouseButtonEvent mouse_event;

  Screen();
  ~Screen();

  bool double_click;

  void main_loop();
};

extern Screen screen;

#endif
