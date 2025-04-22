#pragma once
#include <array>
#include <cstdint>

#include "cpu.h"
#include "ppu.h"

class Bus
{
public:
  Bus();
  ~Bus();

public: // Devices on bus
  cpu6502 cpu;

  ppu2C02 ppu;

  std::array<uint8_t, 2048> cpuRam;

public: // Bus Read & Write
  void cpuWrite(uint16_t addr, uint8_t data);
  uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);
};
