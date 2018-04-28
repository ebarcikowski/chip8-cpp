#include "Chip8.h"


/// Main loop
int main(int argc, char *argv[])
{
  Chip8 chip8;
  chip8.Load("game");

  for (;;) {
    // performs one cycle
    chip8.EmulateCycle();
    // only two opcodes update the screen:
    //   0x00E0 - clears the screen
    //   0xDXYN - draws a sprite on the screen
    if (chip8.DrawFlag()) {
      // draw_graphics();
    }
  }
  // store key press state
  chip8.SetKeys();
}
