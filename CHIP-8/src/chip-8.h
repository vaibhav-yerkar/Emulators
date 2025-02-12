#ifndef CHIP_8_H
#define CHIP_8_H

#include <cstdint>
#include <stdint.h>
#include <string>

class CHIP8
{
public:
  uint8_t gfx[64 * 32]; // Graphics Buffer
  uint8_t key[16];      // Keypad
  bool drawFlag;        // Indicate a draw has been occured

  CHIP8();
  ~CHIP8();

  void emulate_cycle();
  bool load(const std::string& file_path);

protected:
private:
  void init();

  uint16_t stack[16]; // Stack
  uint16_t sp;        // Stack Pointer

  uint8_t memory[4096]; // 4kB Memory
  uint8_t V[16];        // V-register [V0-VF]

  uint16_t pc;     // Program Counter
  uint16_t opcode; // Current op code
  uint16_t I;      // Index register

  uint8_t delay_timer; // Delay Timer
  uint8_t sound_timer; // Sound Timer
};

#endif // !CHIP_8_H
