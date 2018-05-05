/// \file test based around testing the opcode functions
#include <gtest/gtest.h>
#include "Chip8.h"

/// \brief wrapper class so data can be see by test functions.
class OpCodes : public Chip8, public ::testing::Test
{
protected:
  uint16_t pc_orig_;
public:
  void SetUp()
  {
    Init();
    pc_orig_ = 0x200;
  }
};


TEST_F(OpCodes, SetAddress)
{
  uint16_t opcode = 0xa123;
  OpSetAddress(opcode);
  ASSERT_EQ(I_, 0x123);
  ASSERT_EQ(pc_, pc_orig_ + 2);
}

TEST_F(OpCodes, SubRoutine)
{
  // call a subroutine
  uint16_t opc = 0x2123;
  OpSubRoutine(opc);
  ASSERT_EQ(sp_, 1);
  stack_[0] = pc_orig_;
  ASSERT_EQ(pc_, 0x123);
  // return from subroutine
  opc = 0x00ee;
  OpDefault(opc);
  ASSERT_EQ(pc_, pc_orig_);
  ASSERT_EQ(sp_, 0);
}

TEST_F(OpCodes, Jump)
{
  uint16_t opc = 0x1123;
  OpJump(opc);
  ASSERT_EQ(pc_, 0x123);
}

TEST_F(OpCodes, SkipInstr)
{
  uint16_t opc = 0x3123;
  // register is not set, we shouldn't skip
  OpSkipInstr(opc);
  ASSERT_EQ(pc_, pc_orig_+2);
  // set nibble 2 to the first byte 0x23
  v_[0x1] = 0x23;
  // Now we should get a skip.
  OpSkipInstr(opc);
  ASSERT_EQ(pc_, pc_orig_ + 6);
}

TEST_F(OpCodes, SkipInstrNot)
{
  uint16_t opc = 0x4123;
  // register is not set so this this will skip.
  OpSkipInstrNot(opc);
  ASSERT_EQ(pc_, pc_orig_ + 4);
  // Now register IS set, so we will NOT skip.
  v_[1] = 0x23;
  OpSkipInstrNot(opc);
  ASSERT_EQ(pc_, pc_orig_ + 6);
}

TEST_F(OpCodes, SkipInstrEquals)
{
  uint16_t opc = 0x5123;
  // All registers are zero, so this is equal, so we should skip.
  OpSkipInstrEquals(opc);
  ASSERT_EQ(pc_, pc_orig_+4);

  // Set both registers to 1, should skip
  v_[1] = 1;
  v_[2] = 1;
  OpSkipInstrEquals(opc);
  ASSERT_EQ(pc_, pc_orig_+8);

  // Now registers are unequal, should not skip.
  v_[2] = 0;
  OpSkipInstrEquals(opc);
  ASSERT_EQ(pc_, pc_orig_+10);
}

TEST_F(OpCodes, RegSet)
{
  uint16_t opc = 0x6123;
  OpRegSet(opc);
  ASSERT_EQ(v_[1], 0x23);
  ASSERT_EQ(pc_, pc_orig_ + 2);
}

TEST_F(OpCodes, RegAdd)
{
  uint16_t opc = 0x7123;
  OpRegAdd(opc);
  ASSERT_EQ(v_[1], 0x23);
  ASSERT_EQ(pc_, pc_orig_ + 2);
  // Do it again.
  OpRegAdd(opc);
  ASSERT_EQ(v_[1], 2 * 0x23);
  ASSERT_EQ(pc_, pc_orig_ + 4);
}

TEST_F(OpCodes, RegSkipNe)
{
  uint16_t opc = 0x9123;
  OpRegSkipNe(opc);
  ASSERT_EQ(pc_, pc_orig_+2);
  v_[2] = 1;
  OpRegSkipNe(opc);
  ASSERT_EQ(pc_, pc_orig_+6);
}

TEST_F(OpCodes, JumpToAddr)
{
  uint16_t opc = 0xb123;
  OpJumpToAddr(opc);
  ASSERT_EQ(pc_, 0x123);
  v_[0] = 1;
  OpJumpToAddr(opc);
  ASSERT_EQ(pc_, 0x124);
}

TEST_F(OpCodes, RegSetRand)
{
  uint16_t opc = 0xc123;
  OpRegSetRand(opc);
  ASSERT_EQ(pc_, pc_orig_ + 2);
  // Tested this with a quick c program and this should be
  // the value after srand(0)
  srand(0);
  constexpr unsigned rand_valu = 1804289383;
  ASSERT_EQ(v_[1], rand_valu & 0x23);

}


/// \brief wrapper class so data can be see by test functions.
class OpCodesMath : public Chip8, public ::testing::Test
{
protected:
  uint8_t vx_orig_;
  uint8_t vy_orig_;

  uint8_t vx_index_;
  uint8_t vy_index_;
public:
  void SetUp()
  {
    Init();
    vx_orig_ = 3;
    vy_orig_ = 4;

    vx_index_ = 1;
    vy_index_ = 2;
    v_[vx_index_] = vx_orig_;
    v_[vy_index_] = vy_orig_;
  }
};

TEST_F(OpCodesMath, Assign)
{
  uint16_t opc = 0xe120;
  OpRegInterOps(opc);
  ASSERT_EQ(v_[1], v_[2]);
}

TEST_F(OpCodesMath, Or)
{
  uint16_t opc = 0xe121;
  OpRegInterOps(opc);

  ASSERT_EQ(v_[1], vx_orig_ | vy_orig_);
  ASSERT_EQ(v_[2], vy_orig_);
}


TEST_F(OpCodesMath, And)
{
  uint16_t opc = 0xe122;
  OpRegInterOps(opc);

  ASSERT_EQ(v_[1], vx_orig_ & vy_orig_);
  ASSERT_EQ(v_[2], vy_orig_);
}

TEST_F(OpCodesMath, Not)
{
  uint16_t opc = 0xe123;
  OpRegInterOps(opc);

  ASSERT_EQ(v_[1], vx_orig_ ^ vy_orig_);
  ASSERT_EQ(v_[2], vy_orig_);
}


TEST_F(OpCodesMath, Plus)
{
  uint16_t opc = 0xe124;
  OpRegInterOps(opc);

  ASSERT_EQ(v_[1], vx_orig_ + vy_orig_);
  ASSERT_EQ(v_[2], vy_orig_);
}


TEST_F(OpCodesMath, Minus)
{
  uint16_t opc = 0xe125;
  OpRegInterOps(opc);

  ASSERT_EQ(v_[1], (uint8_t)(vx_orig_ - vy_orig_));
  ASSERT_EQ(v_[2], vy_orig_);
}


TEST_F(OpCodesMath, ShiftRight)
{
  uint16_t opc = 0xe126;
  OpRegInterOps(opc);

  ASSERT_EQ(v_[1], vy_orig_ >> 1);
  ASSERT_EQ(v_[2], vy_orig_ >> 1);
}

TEST_F(OpCodesMath, Diff)
{
  uint16_t opc = 0xe127;
  OpRegInterOps(opc);

  ASSERT_EQ(v_[1], (uint8_t)(vy_orig_ - vx_orig_));
  ASSERT_EQ(v_[2], vy_orig_);

}


TEST_F(OpCodesMath, ShiftLeft)
{
  uint16_t opc = 0xe12e;
  OpRegInterOps(opc);

  ASSERT_EQ(v_[1], vy_orig_ << 1);
  ASSERT_EQ(v_[2], vy_orig_ << 1);
}


TEST_F(OpCodes, Draw)
{
  uint16_t opc = 0xD003;
  I_ = 0x200;
  memory_[I_] = 0x3c;
  memory_[I_+1] = 0xc3;
  memory_[I_+2] = 0xff;
  OpDraw(opc);
  ASSERT_EQ(v_[0xf], 0);
  std::vector<bool> line0{0, 0, 1, 1, 1, 1, 0, 0};
  for (unsigned i=0;i<8;i++) {
    ASSERT_EQ(gfx_[i], line0[i]);
  }
  std::vector<bool> line1{1, 1, 0, 0, 0, 0, 1, 1};
  for (unsigned i=0;i<8;i++) {
    ASSERT_EQ(gfx_[64 + i], line1[i]);
  }
  for (unsigned i=0;i<8;i++) {
    ASSERT_EQ(gfx_[128 + i], 1);
  }
}
