#include "Chip8.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <array>


static std::array<uint8_t, 80> chip8_fontset{0};

void Chip8::Init()
{
  pc_ = kPCIndex;
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
  std::ifstream is(filename, std::ios::binary);
  if (is.fail()) {
    std::cerr << "Could not open " << filename << "\n";
    exit(1);
  }

  is.seekg(0, is.end);
  size_t length = is.tellg();
  if (length < kRomSize) {
    std::cerr << "Invalid input file: " << filename << "\n";
    exit(1);
  }
  // TODO: assert here
  is.seekg(0, is.beg);
  // fill in read.  Just copy file data to memory_
  std::copy(std::istreambuf_iterator<char>(is),
            std::istreambuf_iterator<char>(),
            memory_.begin() + kPCIndex);
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
