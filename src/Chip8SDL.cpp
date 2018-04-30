#include "Chip8SDL.h"
#include <iostream>
#include <SDL2/SDL_mixer.h>

Chip8SDL::Chip8SDL(unsigned scale) : scale_{scale},
                                     width_{Chip8::kWidth * scale},
                                     height_{Chip8::kHeight * scale}
{
  Init();
}

Chip8SDL::~Chip8SDL()
{
  Destroy();
}


void Chip8SDL::Destroy()
{
  if (bonk_)
    Mix_FreeChunk(bonk_);
  if (surf_)
    SDL_FreeSurface(surf_);
  if (win_)
    SDL_DestroyWindow(win_);

  bonk_ = nullptr;
  surf_ = nullptr;
  win_ = nullptr;

  Mix_Quit();
  SDL_Quit();
}

void Chip8SDL::Init()
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return;
  }

  // SDL_CreateWindowAndRenderer(width_, height_, 0, &win_, &ren_);
  win_ = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED, width_, height_,
                          SDL_WINDOW_SHOWN);

  ren_ = SDL_CreateRenderer(win_, -1, 0);
	//Make sure creating our window went ok
	if (win_ == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return;
	}
  tex_ = SDL_CreateTexture(ren_,
                           SDL_PIXELFORMAT_ARGB8888,
                           SDL_TEXTUREACCESS_STREAMING,
                           width_, height_);

  surf_ = SDL_CreateRGBSurface(0, width_, height_, 32, 0, 0, 0, 0);
	//Make sure creating our window went ok
	if (surf_ == nullptr){
		std::cout << "SDL_CreateRGBSurface Error: " << SDL_GetError() << std::endl;
		return;
	}
  //Initialize SDL_mixer
  if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
    std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " <<
      Mix_GetError()
              << "\n";
    return;
  }
  bonk_ = Mix_LoadWAV("/home/elliottb/development/chip8-cpp/res/bonk.wav");

  // TODO: This can only be two colors I suppose?
  // setup palette
  // SDL_Color colors[256];
  // // 0 => pixel is off
  // colors[0].r = 0;
  // colors[0].g = 0;
  // colors[0].b = 0;

  // colors[1].r = 255;
  // colors[1].g = 255;
  // colors[1].b = 255;
  // // // everything about 0, pixel is the same color
  // for (int i=1;i<256;i++) {
  //   colors[i].r = 255;
  //    colors[i].g = 255;
  //    colors[i].b = 255;
  //  }
  // SDL_SetPaletteColors(surf_->format->palette, colors, 0, 256);
}

void Chip8SDL::SetSurface(const uint8_t *gfx)
{
  SDL_LockSurface(surf_);
  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.w = scale_;
  rect.h = scale_;

  for (int i=0;i<Chip8::kWidth;i++) {
    for (int j=0;j<Chip8::kHeight;j++) {
      auto index = j * Chip8::kWidth + i;
      rect.x = i * scale_;
      rect.y = j * scale_;
      if (gfx[index]) {
        SDL_FillRect(surf_, &rect, 255);
      }
      else {
        SDL_FillRect(surf_, &rect, 0);
      }
    }
  }
  SDL_UnlockSurface(surf_);
  // std::cout << "pitch: " << surf_->pitch << "\n";
  SDL_UpdateTexture(tex_, nullptr, surf_->pixels, surf_->pitch);
  SDL_RenderClear(ren_);
  SDL_RenderCopy(ren_, tex_, nullptr, nullptr);
  // SDL_Surface *win_surf = SDL_GetWindowSurface(win_);
  SDL_RenderPresent(ren_);
  // SDL_BlitSurface(win_surf, nullptr, surf_, nullptr);
  // SDL_UpdateWindowSurface(win_);
  // SDL_Delay(1);
}

void Chip8SDL::Bonk()
{
  Mix_PlayChannel(-1, bonk_, 0);
}
