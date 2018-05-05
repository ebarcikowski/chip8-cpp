// Chip8Mock.h -*- C++ -*-
#pragma once
#include "Chip8.h"
#include <gmock/gmock.h>


/// Iterface class to get access to chip8 internals for testing
class Chip8IFace : public Chip8
{
public:
  // change visibility of some members for seting.
  using Chip8::Init;
  using Chip8::memory_;
  using Chip8::pc_;

  void SetMemory(uint16_t opcode)
  {
    memory_[pc_] = opcode >> 8;
    memory_[pc_+1] = opcode & 0xff;
  }

  void AssertMemoryEquals(size_t index, size_t value)
  {
    ASSERT_EQ(memory_[index], value);
  }
};


class Chip8Mock : public Chip8IFace
{
public:
  MOCK_METHOD1(OpSetAddress, void(uint16_t));
  MOCK_METHOD1(OpJump, void(uint16_t));
  MOCK_METHOD1(OpSubRoutine, void(uint16_t));
  MOCK_METHOD1(OpSkipInstr, void(uint16_t));
  MOCK_METHOD1(OpSkipInstrNot, void(uint16_t));
  MOCK_METHOD1(OpSkipInstrEquals, void(uint16_t));
};


