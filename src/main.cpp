#include "Chip8SDL.h"
#include <iostream>


/// Main loop
int main(int argc, char *argv[])
{
  Chip8SDL chip8SDL;
  constexpr size_t sz = 32 * 64;
  uint8_t buffer[sz]{};
  unsigned toggle = 0;
  unsigned toggle_line = 0;

  for (int i=0;i<sz;i++) {
    if (i % 64 == 0) {
      toggle_line = toggle_line ? 0 : 1;
      toggle = toggle_line;
    }
    buffer[i] = toggle;
    toggle = toggle ? 0 : 1;
  }

  chip8SDL.SetSurface(buffer);

  // chip8.Load("game");

  // for (;;) {
  //   // performs one cycle
  //   chip8.EmulateCycle();
  //   // only two opcodes update the screen:
  //   //   0x00E0 - clears the screen
  //   //   0xDXYN - draws a sprite on the screen
  //   if (chip8.DrawFlag()) {
  //     // draw_graphics();
  //   }
  // }
  // // store key press state
  // chip8.SetKeys();

  char c;
  std::cin >> c;
}
