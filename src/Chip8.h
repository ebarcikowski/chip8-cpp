// Chip8.h -*- C++ -*-
#pragma once
#include <cstdint>
#include <array>
#include <functional>
#include <unordered_map>

/// This is toy chip8 emulatation that I wrote just to learn how chip8 and
/// emulators work.
///
/// I'm writing this as I read the website:
/// http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
/// so this will probably end up structured similar to his code.

/// \brief Chip8 implimentation class
class Chip8
{
public:


  /// Fetch Opcode
  /// Decode Opcode
  /// Execute Opcode
  ///
  /// Update timers
  void EmulateCycle();

  void Load(const char *filename);

  void SetKeys() {};
  bool DrawFlag() {};

  static constexpr unsigned kMemSize{4096};
  static constexpr unsigned kPCIndex{0x200};
  static constexpr unsigned kRomSize{kMemSize - kPCIndex};
  static constexpr unsigned kWidth{64};
  static constexpr unsigned kHeight{32};

protected:
  /// \brief Initialize registers and memory once
  void Init();

  /// \brief Map opcodes to functions
  void InitOpFunc();

  uint16_t OpCode() const;

  //
  // Opcode functions

  void OpSetAddress(uint16_t opc);
  void OpJump(uint16_t opc);
  void OpSubRoutine(uint16_t opc);
  void OpDefault(uint16_t opc);

  /// current opcode
  uint16_t opcode_{0};
  /// 4k memory
  std::array<uint8_t, kMemSize> memory_{};
  /// 15 8-bit registers named V0, V1,.., VE.  The 16th register is used for
  /// the 'carry flag'.
  std::array<uint8_t, 16> v_;

  /// Index to locations in memory used for writing or reading data
  uint16_t i_{0};

  /// program counter. May be 0x000 to 0xfff.
  ///
  /// pc_ points to location in memory_
  ///
  /// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
  /// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
  /// 0x200-0xFFF - Program ROM and work RAM
  uint16_t pc_{0};
  /// graphics system: The chip-8 has one instruction that draws a sprite
  /// to the screen.  Drawing is done in XOR mode and if a pixel is turn off
  /// as a result of the drawing the VF register is set.
  ///
  /// The window size is 64 x 32 (2048 pixels)
  std::array<uint8_t, kWidth * kHeight> gfx_{};
  uint8_t delay_timer_{0};
  uint8_t sound_timer_{0};
  /// stack depth is 16 deep
  std::array<uint16_t, 16> stack_{};
  /// stack pointer
  uint16_t sp_{0};
  /// hex based keypad to store the current state of a key
  std::array<uint8_t, 16> key_{};

  std::unordered_map<unsigned, std::function<void(uint16_t)>> func_map_;
};
