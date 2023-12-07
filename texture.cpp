#include "texture.hpp"
#include "SDL2/SDL_image.h"

Texture::Texture()
  : loaded(false), sdl_texture(nullptr) { }

Texture::~Texture()
{
  if (loaded) SDL_DestroyTexture(sdl_texture);
}

void Texture::load(SDL_Renderer* r, const char* path)
{
  sdl_texture = IMG_LoadTexture(r, path);
  if (!sdl_texture) panic(IMG_GetError);
  if (SDL_QueryTexture(sdl_texture, NULL, NULL, &width, &height) != 0) {
    panic(SDL_GetError());
  }
}

void Texture::draw(SDL_Renderer* r, SDL_Rect dst) const
{
  SDL_RenderCopy(r, sdl_texture, NULL, &dst);
}

void Texture::draw(SDL_Renderer* r, V2 pos) const
{
  draw(r, {pos.x, pos.y, width, height});
}

TextureAnim::TextureAnim() : running(false), cur_frame(0), elapsed(0) { }

void TextureAnim::load(SDL_Renderer* r, const char* path, int frame_width, int frame_height, Uint32 frame_duration)
{
  texture_atlas.load(r, path);

  this->frame_width = frame_width;
  this->frame_height = frame_height;
  
  if (texture_atlas.width % frame_width != 0) {
    panic(texture_atlas.width, " % ", frame_width, " != 0");
  }

  if (texture_atlas.height % frame_height != 0) {
    panic(texture_atlas.height, " % ", frame_height, " != 0");
  }

  this->width_frames = texture_atlas.width / frame_width;
  this->height_frames = texture_atlas.height / frame_height;
  this->frame_count = this->width_frames * this->height_frames;
  this->frame_duration = frame_duration;
}

void TextureAnim::set_frame_count(int count)
{
  frame_count = count;
}

void TextureAnim::set_total_duration(Uint32 ms)
{
  frame_duration = ms / frame_count;
}

void TextureAnim::start()
{
  running = true;
}

void TextureAnim::finish()
{
  running = false;
  cur_frame = 0;
  elapsed = 0;
}

void TextureAnim::update(float dt)
{
  if (!running) return;
  
  elapsed += dt * 1000;
  if (elapsed >= frame_duration) {
    cur_frame = (cur_frame + 1) % frame_count;
    elapsed -= frame_duration;
  }
}

void TextureAnim::draw(SDL_Renderer* r, SDL_Rect dst) const
{
  SDL_Rect src = {cur_frame % width_frames * frame_width, cur_frame / width_frames * frame_height, frame_width, frame_height};
  SDL_RenderCopy(r, texture_atlas.sdl_texture, &src, &dst);
}

void TextureAnim::draw(SDL_Renderer* r, V2 pos) const
{
  draw(r, {pos.x, pos.y, frame_width, frame_height});
}
