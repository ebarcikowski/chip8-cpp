// Chip8SDL.h -*- C++ -*-
#pragma once
#include <SDL2/SDL.h>
#include "Chip8.h"
#include <cstdint>


class Chip8SDL
{
public:
  Chip8SDL(unsigned scale=10);
  virtual ~Chip8SDL();

  void SetSurface(const uint8_t *buffer);

protected:
  void Init();
  void Destroy();
  // Uint32
  unsigned int scale_;
  unsigned width_;
  unsigned height_;

  SDL_Texture *tex_{nullptr};
  SDL_Renderer *ren_{nullptr};
  SDL_Window *win_{nullptr};
  SDL_Surface *surf_{nullptr};
};
