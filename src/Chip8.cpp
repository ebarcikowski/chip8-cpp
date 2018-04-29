#include "Chip8.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <array>


static std::array<uint8_t, 80> chip8_fontset{};

void Chip8::InitOpFunc()
{
  func_map_[0xa000] = [this](uint16_t opc){OpSetAddress(opc);};
  func_map_[0x2000] = [this](uint16_t opc){OpJumpAddress(opc);};
}

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

uint16_t Chip8::OpCode() const
{
  return (memory_[pc_] << 8) | memory_[pc_ + 1];
}
void Chip8::EmulateCycle()
{
  // get opcode
  uint16_t opcode = OpCode();

  try {
    func_map_[opcode & 0xf000](opcode);
  }
  catch (...) {
    std::cerr << "Unknown opcode: 0x" << std::hex << opcode << "\n";
  }

  if (delay_timer_ > 0)
    --delay_timer_;
  if (sound_timer_ > 0) {
    if (sound_timer_ == 1)
      std::cout << "BEEP\n";
    --sound_timer_;
  }
}


void Chip8::OpSetAddress(uint16_t opcode)
{
  i_ = opcode & 0x0fff;
  pc_ += 2;
}


void Chip8::OpJumpAddress(uint16_t opc)
{
  // jump to address in lower 12 bits 0x0fff
  // put the current program counter on the stack
  stack_[sp_] = pc_;
  ++sp_;
  // set pc to to jump address
  pc_ = opcode_ & 0x0fff;
}
