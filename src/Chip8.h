// Chip8.h -*- C++ -*-
#pragma once
#include <cstdint>

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
  /// initialize registers and memory once
  void Init();

  /// Fetch Opcode
  /// Decode Opcode
  /// Execute Opcode
  ///
  /// Update timers
  void EmulateCycle();

  void Load(const char *filename);

  void SetKeys() {};
  bool DrawFlag() {};

protected:
  /// current opcode
  uint16_t opcode_;
  /// 4k memory
  uint8_t memory_[4096];
  /// 15 8-bit registers named V0, V1,.., VE.  The 16th register is used for
  /// the 'carry flag'.
  uint8_t v_[16];
  /// index register
  uint16_t i_;
  /// program counter. May be 0x000 to 0xfff.
  ///
  /// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
  /// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
  /// 0x200-0xFFF - Program ROM and work RAM
  /// pc_ points to location in memory_
  uint16_t pc_;
  /// graphics system: The chip-8 has one instruction that draws a sprite
  /// to the screen.  Drawing is done in XOR mode and if a pixel is turn off
  /// as a result of the drawing the VF register is set.
  ///
  /// The window size is 64 x 32 (2048 pixels)
  uint8_t gfx_[64 * 32];
  uint8_t delay_timer_;
  uint8_t sound_timer_;
  /// stack depth is 16 deep
  uint16_t stack_[16];
  /// stack pointer
  uint16_t sp_;
  /// hex based keypad to store the current state of a key
  uint8_t key_[16];
};
