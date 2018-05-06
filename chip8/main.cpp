#include "Chip8SDL.h"
#include <iostream>

int main(int argc, char *argv[])
{
  Chip8SDL chip8;
  if (argc < 2) {
    std::cout << "usage: " << argv[0] << " <rom>\n";
    exit(1);
  }
  assert(argc > 1);
  chip8.Load(argv[1]);
  chip8.Run();
}
