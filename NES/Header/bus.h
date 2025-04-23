#pragma once
#include <array>
#include <cstdint>

#include "cartridge.h"
#include "cpu.h"
#include "ppu.h"

class Bus
{
public:
  cpu6502 cpu;
  ppu2C02 ppu;

  std::shared_ptr<Cartridge> cart;
  std::array<uint8_t, 2048> cpuRam;

  Bus();
  ~Bus();

  void cpuWrite(uint16_t addr, uint8_t data);
  uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

  void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
  void reset();
  void clock();

protected:
private:
  uint32_t nSystemClockCounter = 0;
};
