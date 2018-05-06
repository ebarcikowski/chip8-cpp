#include "Chip8SDL.h"
#include <iostream>
#include <SDL2/SDL_mixer.h>

Chip8SDL::Chip8SDL(unsigned scale) : scale_{scale},
                                     width_{Chip8::kWidth * scale},
                                     height_{Chip8::kHeight * scale}
{
  Init();
  InitSDLKeys();
}

void Chip8SDL::InitSDLKeys()
{
  sdl_key_map_[SDLK_1] = 0x1;
  sdl_key_map_[SDLK_2] = 0x2;
  sdl_key_map_[SDLK_2] = 0x3;
  sdl_key_map_[SDLK_4] = 0xc;
  sdl_key_map_[SDLK_q] = 0x4;
  sdl_key_map_[SDLK_w] = 0x5;
  sdl_key_map_[SDLK_e] = 0x6;
  sdl_key_map_[SDLK_r] = 0xd;
  sdl_key_map_[SDLK_a] = 0x7;
  sdl_key_map_[SDLK_s] = 0x8;
  sdl_key_map_[SDLK_d] = 0x9;
  sdl_key_map_[SDLK_f] = 0xe;
  sdl_key_map_[SDLK_z] = 0xa;
  sdl_key_map_[SDLK_x] = 0x0;
  sdl_key_map_[SDLK_c] = 0xb;
  sdl_key_map_[SDLK_v] = 0xf;
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

void Chip8SDL::Run()
{
  int counter = 0;
  bool quit = false;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT)
        quit = true;
      else if (e.type == SDL_KEYDOWN) {
        auto key_iter = sdl_key_map_.find(e.key.keysym.sym);
        if (key_iter == sdl_key_map_.end())
          continue;
        key_[key_iter->second] = 1;
      }
    }

    EmulateCycle();

    counter++;
    if ((counter % 75) == 0)
      ResetKeys();

    if (draw_flag_) {
      Draw();
      draw_flag_ = false;
    }
    SDL_Delay(delay_);
  }
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
}

void Chip8SDL::Draw()
{
  SetSurface(gfx_.data());
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
  SDL_UpdateTexture(tex_, nullptr, surf_->pixels, surf_->pitch);
  SDL_RenderClear(ren_);
  SDL_RenderCopy(ren_, tex_, nullptr, nullptr);
  SDL_RenderPresent(ren_);
}

void Chip8SDL::Bonk()
{
  Mix_PlayChannel(-1, bonk_, 0);
}
