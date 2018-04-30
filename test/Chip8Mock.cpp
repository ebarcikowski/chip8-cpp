#include "Chip8Mock.h"


// Do the right Op functions get called
TEST(Chip8Mock, OpCodes)
{
  Chip8Mock chip8;
  chip8.pc_ = 0x200;
  chip8.SetMemory(0xa000);
  EXPECT_CALL(chip8, OpSetAddress(0xa000));
  chip8.EmulateCycle();

  chip8.SetMemory(0x1000);
  EXPECT_CALL(chip8, OpJump(0x1000));
  chip8.EmulateCycle();

  chip8.SetMemory(0x2000);
  EXPECT_CALL(chip8, OpSubRoutine(0x2000));
  chip8.EmulateCycle();

  chip8.SetMemory(0x3000);
  EXPECT_CALL(chip8, OpSkipInstr(0x3000));
  chip8.EmulateCycle();

  chip8.SetMemory(0x4000);
  EXPECT_CALL(chip8, OpSkipInstrNot(0x4000));
  chip8.EmulateCycle();

  chip8.SetMemory(0x5000);
  EXPECT_CALL(chip8, OpSkipInstrEquals(0x5000));
  chip8.EmulateCycle();
}
