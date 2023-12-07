#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include "util.hpp"
#include "SDL2/SDL.h"

class Texture
{
  bool loaded;

public:
  Texture();
  ~Texture();

  int width, height;
  SDL_Texture* sdl_texture;

  void load(SDL_Renderer* r, const char* path);
  void draw(SDL_Renderer* r, SDL_Rect dst) const;
  void draw(SDL_Renderer* r, V2 pos) const;
};

class TextureAnim
{
  bool running;
  Texture texture_atlas;
  int width_frames;
  int height_frames;
  int cur_frame;
  int frame_count;
  Uint32 frame_duration;
  Uint32 elapsed;

public:
  TextureAnim();

  int frame_width, frame_height;

  void load(SDL_Renderer* r, const char* path, int frame_width, int frame_height, Uint32 frame_interval=200);
  void set_frame_count(int count);
  void set_total_duration(Uint32 ms);
  void start();
  void finish();
  void update(float dt);
  void draw(SDL_Renderer* r, SDL_Rect dst) const;
  void draw(SDL_Renderer* r, V2 pos) const;
};

#endif
