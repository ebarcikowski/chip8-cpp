#include "Chip8.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <array>
#include <cassert>
#include <cstdlib>


static std::array<uint8_t, 80> chip8_fontset{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8::InitOpFunc()
{
  func_map_[0x0000] = [this](uint16_t opc){OpDefault(opc);};
  func_map_[0x1000] = [this](uint16_t opc){OpJump(opc);};
  func_map_[0x2000] = [this](uint16_t opc){OpSubRoutine(opc);};
  func_map_[0x3000] = [this](uint16_t opc){OpSkipInstr(opc);};
  func_map_[0x4000] = [this](uint16_t opc){OpSkipInstrNot(opc);};
  func_map_[0x5000] = [this](uint16_t opc){OpSkipInstrEquals(opc);};
  func_map_[0x6000] = [this](uint16_t opc){OpRegSet(opc);};
  func_map_[0x7000] = [this](uint16_t opc){OpRegAdd(opc);};
  func_map_[0x8000] = [this](uint16_t opc){OpRegInterOps(opc);};
  func_map_[0x9000] = [this](uint16_t opc){OpRegSkipNe(opc);};
  func_map_[0xa000] = [this](uint16_t opc){OpSetAddress(opc);};
  func_map_[0xb000] = [this](uint16_t opc){OpJumpToAddr(opc);};
  func_map_[0xc000] = [this](uint16_t opc){OpRegSetRand(opc);};
  func_map_[0xd000] = [this](uint16_t opc){OpDraw(opc);};
  func_map_[0xe000] = [this](uint16_t opc){OpKeySkipInstr(opc);};
  func_map_[0xf000] = [this](uint16_t opc){OpTimers(opc);};
}

Chip8::Chip8()
{
  Init();
  InitOpFunc();
}

void Chip8::Init()
{
  pc_ = kPCIndex;
  opcode_ = 0;
  I_ = 0;
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
  I_ = opcode & 0x0fff;
  pc_ += 2;
}


void Chip8::OpSubRoutine(uint16_t opc)
{
  // jump to address in lower 12 bits 0x0fff
  // put the current program counter on the stack
  stack_[sp_] = pc_;
  ++sp_;
  // set pc to to jump address
  pc_ = opc & 0x0fff;
}

void Chip8::OpSkipInstr(uint16_t opc)
{
  uint8_t val = opc & 0xff;
  uint8_t reg = GetNibble(opc, 2);

  assert(reg < 16);
  if (v_[reg] == val) {
    pc_ += 2;
  }
}

void Chip8::OpSkipInstrNot(uint16_t opc)
{
  uint8_t val = opc & 0xff;
  uint8_t reg = GetNibble(opc, 2);
  assert(reg < 16);
  if (v_[reg] != val) {
    pc_ += 2;
  }
}

void Chip8::OpSkipInstrEquals(uint16_t opc)
{
  uint8_t regx = GetNibble(opc, 1);
  uint8_t regy = GetNibble(opc, 2);
  if (v_[regx] == v_[regy]) {
    pc_ += 2;
  }
}

void Chip8::OpJump(uint16_t opc)
{
  pc_ = opc & 0x0fff;
}

void Chip8::OpDefault(uint16_t opc)
{
  switch (opc) {
  case 0x00e0:
    std::fill(gfx_.begin(), gfx_.end(), 0);
    break;
  case 0x0ee:
    // return from subroutine
    assert(sp_ > 0);
    pc_ = stack_[--sp_];
    break;
  }
}

void Chip8::OpRegSet(uint16_t opc)
{
  auto reg_index = GetNibble(opc, 2);
  v_[reg_index] = opc & 0xff;
}

void Chip8::OpRegAdd(uint16_t opc)
{
  auto reg_index = GetNibble(opc, 2);
  v_[reg_index] += opc & 0xff;
}

void Chip8::OpRegSkipNe(uint16_t opc)
{
  auto regx = GetNibble(opc, 2);
  auto regy = GetNibble(opc, 1);
  if (v_[regx] != v_[regy])
    pc_ += 2;
}

void Chip8::OpRegInterOps(uint16_t opc)
{
  unsigned regx = GetNibble(opc, 2);
  unsigned regy = GetNibble(opc, 1);

  switch (GetNibble(opc, 0)) {
  case 0x0:
    v_[regx] = v_[regy];
    break;
  case 0x1:
    v_[regx] |= v_[regy];
    break;
  case 0x2:
    v_[regx] &= v_[regy];
    break;
  case 0x3:
    v_[regx] ^= v_[regy];
    break;
  case 0x4:
    v_[regx] += v_[regy];
    break;
  case 0x5:
    v_[regx] -= v_[regy];
    break;
  case 0x6:
    v_[regx] = v_[regy] = v_[regy] >> 1;
    break;
  case 0x7:
    v_[regx] = v_[regy] - v_[regx];
    break;
  case 0xe:
    v_[regx] = v_[regy] = v_[regy] << 1;
    break;
  default:
    std::cerr << "Should not get here\n";
  };
}

void Chip8::OpJumpToAddr(uint16_t opc)
{
  pc_ = v_[0] + (opc & 0xfff);
}

void Chip8::OpRegSetRand(uint16_t opc)
{
  uint8_t reg = GetNibble(opc, 2);
  uint8_t mask = opc & 0xff;
  v_[reg] = rand() & (opc & mask);
}
