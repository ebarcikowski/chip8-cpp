#include "Chip8SDL.h"
#include <iostream>

static constexpr size_t SZ = 32 * 64;

// quadrants.  order is clock-wise
void quads(unsigned char *buffer, int quad)
{
  for (int i=0;i<64;i++) {
    for (int j=0;j<32;j++) {
      unsigned index = j * 64 + i;

      unsigned row = i / 32;
      unsigned col = j / 16;
      if (row == 0 && col == 0)
        // quad 1
        buffer[index] = quad == 0;
      else if (row == 1 && col == 0)
        // quad 2
        buffer[index] = quad == 1;
      else if (row == 0 && col == 1)
        // quad 3
        buffer[index] = quad == 2;
      else
        // quad 4
        buffer[index] = quad == 3;
    }
  }
}


void checker(unsigned char *buffer)
{
  unsigned toggle = 0;
  unsigned toggle_line = 0;
  constexpr size_t sz = 32 * 64;

  for (int i=0;i<sz;i++) {
    if (i % 64 == 0) {
      toggle_line = toggle_line ? 0 : 1;
      toggle = toggle_line;
    }
    buffer[i] = toggle;
    toggle = toggle ? 0 : 1;
  }
}

void draw_examples()
{
  Chip8SDL chip8SDL;
  constexpr size_t sz = 32 * 64;
  uint8_t buffer[sz]{};

  char c;
  checker(buffer);
  chip8SDL.SetSurface(buffer);

  std::cout << "enter for next\n";
  std::cin >> c;

  quads(buffer, 0);
  chip8SDL.SetSurface(buffer);
  std::cin >> c;

  quads(buffer, 1);
  chip8SDL.SetSurface(buffer);
  std::cin >> c;

  quads(buffer, 2);
  chip8SDL.SetSurface(buffer);
  std::cin >> c;

  quads(buffer, 3);
  chip8SDL.SetSurface(buffer);
  std::cin >> c;
}
/// Main loop
int main(int argc, char *argv[])
{
  Chip8SDL chip8;
  assert(argc > 1);
  chip8.Load(argv[1]);
  chip8.Run();

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

}
