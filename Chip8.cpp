#include "Chip8.h"
#include <iostream>

static uint8_t chip8_fontset[80] {0};

void Chip8::Init()
{
  pc_ = 0x200;
  opcode_ = 0;
  i_ = 0;
  sp_ = 0;

  // clear display
  // clear stack
  // clear registers v0-vf

  // load font set
  for (int i=0;i<80;++i)
    memory_[i] = chip8_fontset[i];
}

void Chip8::Load(const char* filename)
{
  FILE *f = fopen(filename, "rb");
  if (f == nullptr) {
    std::cerr << "failed to open file: " << filename << "\n";
    return;
  }
  size_t read_size = 0;
  uint8_t buffer[1];
  // fill in read.  Just copy file data to memory_
  for (int i=0;i<read_size;i++) {
    memory_[i+0x200] = buffer[i];
  }
}

void Chip8::EmulateCycle()
{
  // get opcode
  opcode_ = (memory_[pc_] << 8) | memory_[pc_ + 1];
  switch (opcode_ & 0xF000) {
    // some opcodes //
  case 0xA000:
    // ANNN: sets index to the address at the lower 12 bits
    i_ = opcode_ & 0x0fff;
    pc_ += 2;
    break;
  case 0x2000:
    // jump to address in lower 12 bits 0x0fff
    // put the current program counter on the stack
    stack_[sp_] = pc_;
    ++sp_;
    // set pc to to jump address
    pc_ = opcode_ & 0x0fff;
    // more opcodes //

  default:
    std::cerr << "Unknown opcode: 0x" << std::hex << opcode_ << "\n";
  }

  if (delay_timer_ > 0)
    --delay_timer_;
  if (sound_timer_ > 0) {
    if (sound_timer_ == 1)
      std::cout << "BEEP\n";
    --sound_timer_;
  }
}
