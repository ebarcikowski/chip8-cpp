// Chip8SDL.h -*- C++ -*-
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <cstdint>
#include "Chip8.h"
#include <unordered_map>

#ifndef BONK_FNAME
#  define BONK_FNAME ""
#endif

/// \brief SDL elements for Chip8.
///
/// The Chip8 does all the really lifting but this class will draw
/// graphics, play sound and get user inputs.
class Chip8SDL : public Chip8
{
public:
  /// \brief Chip8SDL ctor.
  Chip8SDL(unsigned scale=10);
  virtual ~Chip8SDL();
  /// Run SDL loop.
  void Run();
  /// Draw arbitary surface.
  void SetSurface(const uint8_t *buffer);
  /// \brief worker function just to see if .wav file is around.
  static bool FileExists(const char *fname);
protected:
  /// Play SDL sound.
  void Bonk();
  /// Init SDL data.
  void Init();
  /// Fill in keys for sdl_key_map_ member.
  void InitSDLKeys();

  /// Teardown all the SDL structs.
  void Destroy();
  /// Perform draw with gfx_ member.
  void Draw();

  /// Scale from chip pixels to SDL pixels
  unsigned scale_;
  /// Width of SDL window.
  unsigned width_;
  /// Height of SDL window.
  unsigned height_;
  /// Delay for SDL loop.
  unsigned delay_{1};

  /// Mapping from SDL key values to indexes in key_ member.
  std::unordered_map<unsigned, unsigned> sdl_key_map_;
  SDL_Texture *tex_{nullptr};
  SDL_Renderer *ren_{nullptr};
  SDL_Window *win_{nullptr};
  SDL_Surface *surf_{nullptr};
  Mix_Chunk *bonk_{nullptr};
};
