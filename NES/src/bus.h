#pragma once

#include <array>
#include <cstdint>

#include "./6502/cpu.h"

class Bus
{
public:
  cpu6502 cpu;                        // Device on Bus
  std::array<uint8_t, 64 * 1024> ram; // temp ram

  Bus();
  ~Bus();

  void write(uint16_t addr, uint8_t data);
  uint8_t read(uint16_t addr, bool readOnly = false);

protected:
private:
};
