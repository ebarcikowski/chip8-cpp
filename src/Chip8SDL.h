// Chip8SDL.h -*- C++ -*-
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <cstdint>
#include "Chip8.h"


class Chip8SDL : public Chip8
{
public:
  Chip8SDL(unsigned scale=10);
  virtual ~Chip8SDL();

  void SetSurface(const uint8_t *buffer);
  void Bonk();
  void Run();

protected:
  void Init();
  void Destroy();
  void Draw();
  // Uint32
  unsigned int scale_;
  unsigned width_;
  unsigned height_;

  SDL_Texture *tex_{nullptr};
  SDL_Renderer *ren_{nullptr};
  SDL_Window *win_{nullptr};
  SDL_Surface *surf_{nullptr};
  Mix_Chunk *bonk_{nullptr};
};
