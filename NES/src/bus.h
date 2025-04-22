#pragma once
#include <array>
#include <cstdint>

#include "./6502/cpu.h"

class Bus
{
public:
  Bus();
  ~Bus();

public: // Devices on bus
  cpu6502 cpu;

  std::array<uint8_t, 64 * 1024> ram;

public: // Bus Read & Write
  void write(uint16_t addr, uint8_t data);
  uint8_t read(uint16_t addr, bool bReadOnly = false);
};
