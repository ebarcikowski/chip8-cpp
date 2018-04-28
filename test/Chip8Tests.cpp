#include "gtest/gtest.h"
#include "Chip8.h"
#include <fstream>
#include <string_view>

/// Iterface class to get access to chip8 internals for testing
class Chip8IFace : public Chip8
{
public:
  uint16_t PC() {return pc_;}

  void AssertMemoryEquals(size_t index, size_t value)
  {
    ASSERT_EQ(memory_[index], value);
  }
};

class Chip8Test : public ::testing::Test
{
protected:
  /// dummy file to write some data to
  static constexpr char test_file[]{"test_file"};
  /// dummy data to compare to after load
  std::array<uint8_t, Chip8::kRomSize> rom_{0};

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
  for (int i=0;i<Chip8::kMemSize;i++)
    chip8.AssertMemoryEquals(i, 0);

  chip8.Init();
  ASSERT_EQ(chip8.PC(), Chip8::kPCIndex);
}
