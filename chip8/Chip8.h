// Chip8.h -*- C++ -*-
/// \file
/// This is toy chip8 emulatation that I wrote just to learn how chip8 and
/// emulators work.
#pragma once
#include <cstdint>
#include <array>
#include <functional>
#include <unordered_map>
#include <cassert>

/// \brief Chip8 implimentation class
///
/// This class emulates all the chip8 functionality, but knows nothing
/// about graphics, keys, or sound.  This should be inherited to provide
/// that functionality.  As of this this writting there is only one which
/// handles graphics with SDL.
class Chip8
{
public:
  Chip8();

  /// \brief Executes one cycle of the chip8.
  void EmulateCycle();

  /// \brief Load a rom by file name.
  void Load(const char *filename);

  /// Simple inline worker function as I kept doing this operation.
  inline static uint8_t GetNibble(uint16_t val, unsigned index)
  {
    assert(index < 4);
    auto shift = index * 4;
    return (val >> shift) & 0xf;
  }

  /// \brief Chip8 has 8k of memory
  static constexpr unsigned kMemSize{4096};
  /// \brief Starting program counter.
  static constexpr unsigned kPCIndex{0x200};
  /// \brief Max possible size of a ROM.
  static constexpr unsigned kMaxRomSize{kMemSize - kPCIndex};
  /// \brief Width of a Chip8 screen.
  static constexpr unsigned kWidth{64};
  /// \brief Height of a Chip8 screen.
  static constexpr unsigned kHeight{32};

protected:
  /// \brief Initialize registers and memory.
  void Init();

  /// \brief Map opcodes to functions
  void InitOpFunc();

  /// \brief Map actual keys to their index (0-0xf).
  virtual void InitKeyMap();

  /// \brief Set all pressed keys back to zero.
  void ResetKeys();

  /// The next opcode from memory based on current pc.
  uint16_t OpCode() const;

  //
  // Opcode functions
  // The following are dispatch functions mapped to the most significant
  // nibble.

  /// \brief Return from a subroutine and clears screen.
  ///
  /// Default is a bad name for this, but stuck because it's two jobs are
  /// so different.
  /// (prefix 0x0000)
  virtual void OpDefault(uint16_t opc);

  /// \brief Jumps the pc.
  ///
  /// (prefix 0x1000)
  virtual void OpJump(uint16_t opc);

  /// \brief Sets the pc to a subroutine location.
  ///
  /// (prefix 0x2000)
  virtual void OpSubRoutine(uint16_t opc);

  /// \brief Does a skip if the desired register equals the included value.
  ///
  /// (prefix 0x3000)
  virtual void OpSkipInstr(uint16_t opc);

  /// \brief Does a skip if the desired register doesn't equals the
  ///   included value.
  ///
  /// (prefix 0x4000)
  virtual void OpSkipInstrNot(uint16_t opc);

  /// \brief Does a skip if two registers are equal.
  ///
  /// (prefix 0x5000)
  virtual void OpSkipInstrEquals(uint16_t opc);

  /// \brief Sets a register.
  ///
  /// (prefix 0x6000)
  virtual void OpRegSet(uint16_t opc);

  /// \brief \brief Adds a value to a register.
  ///
  /// (prefix 0x7000)
  virtual void OpRegAdd(uint16_t opc);

  /// \brief Math operations between two registers.
  ///
  /// (prefix 0x8000)
  virtual void OpRegInterOps(uint16_t opc);

  /// brief Skips an instruction if two registers are unequal.
  ///
  /// (prefix 0x9000)
  virtual void OpRegSkipNe(uint16_t opc);

  /// \brief Set's the memory index to an address.
  ///
  /// (prefix 0xa000)
  virtual void OpSetAddress(uint16_t opc);

  /// \brief Jump the pc to a register plus offset.
  ///
  /// (prefix 0xb000)
  virtual void OpJumpToAddr(uint16_t opc);

  /// \brief Sets a register to a random number.
  ///
  /// (prefix 0xc000)
  virtual void OpRegSetRand(uint16_t opc);

  /// \brief Handles drawing to scree.
  ///
  /// (prefix 0xd000)
  virtual void OpDraw(uint16_t opc);

  /// \brief Skips instuctions based on key presses.
  ///
  /// (prefix 0xe000);
  virtual void OpKeySkipInstr(uint16_t opc);

  /// \brief Many different operations, involving memory and timers.
  /// (prefix 0xf000);
  virtual void OpTimers(uint16_t opc);

  /// \brief Blocking key press
  ///
  /// This function is used mostly for debugging as it will require an
  /// and enter.  This function should be overloaded with a library that
  /// will do the actually graphics (ncurses, SDL, or some other).
  virtual char KeyPress();

  /// 4k main memory
  std::array<uint8_t, kMemSize> memory_{};

  /// 15 8-bit registers named V0, V1,.., VE.
  ///
  ///  The 16th register (VF) has two dedicated purposes:
  ///     - Collision detection in the graphics system.
  ///     - Acts as a carry flag for bit operations.
  std::array<uint8_t, 16> v_{};

  /// Index to locations in memory used for writing or reading data
  uint16_t I_{0};

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

  /// Ready to draw.
  bool draw_flag_{0};

  /// Play a sound.
  bool sound_flag_{0};
  /// Chip8 delay timer.
  uint8_t delay_timer_{0};
  /// Chip8 sound timer.
  uint8_t sound_timer_{0};
  /// Instruction stack, depth is 16.
  std::array<uint16_t, 16> stack_{};
  /// stack pointer, obviously works with stack_.
  uint16_t sp_{0};
  /// hex based keypad to store the current state of a key
  /// these are one or zero.
  std::array<uint8_t, 16> key_{};
  /// Dispatch table for opcode instructions
  std::unordered_map<unsigned, std::function<void(uint16_t)>> func_map_;
  /// Provides mapping from keys to key index.
  std::unordered_map<char, uint8_t> key_map_;
};
