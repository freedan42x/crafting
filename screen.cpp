#include "screen.hpp"
#include "atlas.hpp"
#include "game.hpp"
#include "SDL2/SDL_image.h"

Atlas atlas;

Screen::Screen()
  : last_click_timestamp(0),
    fps(60),
    mouse{0}, clicked(false),
    double_click(false)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) panic(SDL_GetError());

  window = SDL_CreateWindow("crafting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
  if (!window) panic(SDL_GetError());
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) panic(SDL_GetError());

  SDL_GetWindowSize(window, &width, &height);

  if(TTF_Init() != 0) panic(TTF_GetError());
  font = TTF_OpenFont("font/terminus.ttf", 12);
  if (!font) panic(TTF_GetError());

  if(IMG_Init(IMG_INIT_PNG) == 0) panic(IMG_GetError());

  atlas.load(renderer);
}

Screen::~Screen()
{
  TTF_CloseFont(font);
  TTF_Quit();
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Screen::draw() const
{
  SDL_SetRenderDrawColor(renderer, 104, 81, 115, 255);
  SDL_RenderClear(renderer);

  game.draw();

  SDL_RenderPresent(renderer);
}

void Screen::main_loop()
{ 
  float dt = 1.0f / fps;
  bool quit = false;

  while (!quit) {
    Uint64 start = SDL_GetPerformanceCounter();

    clicked = false;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT: {
	quit = true;
      } break;

      case SDL_MOUSEBUTTONDOWN: {
	clicked = true;
	mouse_event = e.button;
	
	if (e.button.button == SDL_BUTTON_LEFT) {
	  Uint32 delta = e.button.timestamp - last_click_timestamp;
	  last_click_timestamp = e.button.timestamp;
	  double_click = delta < 200;
	} else if (e.button.button == SDL_BUTTON_RIGHT) {
	  game.grid_fill_mode = true;
	}

      } break;

      case SDL_MOUSEBUTTONUP: {
	clicked = true;
	mouse_event = e.button;

	if (e.button.button == SDL_BUTTON_RIGHT) {
	  // TODO: better way to manage this
	  game.grid_fill_mode = false;
	  game.inventory.clear_marks();
	  game.crafting_grid.clear_marks();
	  game.crafting_result.clear_marks();
	}
      } break;

      case SDL_MOUSEMOTION: {
	mouse = {e.motion.x, e.motion.y};
      } break;

      case SDL_KEYDOWN: {
	switch (e.key.keysym.sym) {
	case SDLK_SPACE: {
	  game.inventory.add_random_item();
	} break;
	}
      } break;

      case SDL_WINDOWEVENT: {
	switch (e.window.event) {
	case SDL_WINDOWEVENT_SIZE_CHANGED: {
	  width = e.window.data1;
	  height = e.window.data2;
	  game.on_resize();
	} break;
	}
      }
      }
    }

    game.update(dt);
    draw();

    Uint64 end = SDL_GetPerformanceCounter();
    float elapsed = static_cast<float>(end - start) / SDL_GetPerformanceFrequency();
    dt = std::max(1.0f / fps - elapsed, 0.0f);
    SDL_Delay(dt * 1000);
  }
}

Screen screen;
