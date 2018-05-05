#include "gtest/gtest.h"
#include "Chip8.h"
#include "Chip8Mock.h"
#include <fstream>
#include <string_view>


class Chip8Test : public ::testing::Test
{
protected:
  /// dummy file to write some data to
  static constexpr char test_file[]{"test_file"};
  /// dummy data to compare to after load
  std::array<uint8_t, Chip8::kRomSize> rom_{};

  /// Setup dummy file and data
  virtual void SetUp()
  {
    std::ofstream f(test_file, std::ios::out | std::ios::binary);
    for (unsigned int i=0;i<Chip8::kRomSize;i++) {
      uint8_t to_write = static_cast<uint8_t>(i % UINT8_MAX);
      rom_[i] = to_write;
      f << to_write;
    }
    f.close();
  }
  /// remove created file
  virtual void TearDown()
  {
    std::remove(test_file);
  }
};


/// load dummy data and check that it has the right values
TEST_F(Chip8Test, Load)
{
  Chip8IFace chip8;
  chip8.Load(test_file);
  for (unsigned i=0x200;i<Chip8::kMemSize;i++)
    chip8.AssertMemoryEquals(i, rom_[i-0x200]);
}

/// Check basic initialization
TEST(Chip8, Init)
{
  Chip8IFace chip8;
  // for (int i=0;i<Chip8::kMemSize;i++)
  //   chip8.AssertMemoryEquals(i, 0);

  // chip8.Init();
  ASSERT_EQ(chip8.pc_, Chip8::kPCIndex);
}


TEST(Chip8, GetNibble)
{
  uint16_t value = 0x1234;
  ASSERT_EQ(Chip8::GetNibble(value, 0), 4);
  ASSERT_EQ(Chip8::GetNibble(value, 1), 3);
  ASSERT_EQ(Chip8::GetNibble(value, 2), 2);
  ASSERT_EQ(Chip8::GetNibble(value, 3), 1);

}
