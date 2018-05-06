#include "Chip8SDL.h"
#include <iostream>

int main(int argc, char *argv[])
{
  Chip8SDL chip8;
  assert(argc > 1);
  chip8.Load(argv[1]);
  chip8.Run();
}
