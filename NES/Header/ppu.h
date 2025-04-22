#pragma once

#include <cstdint>
class ppu2C02
{
public:
  // Communication with main Bus
  uint8_t cpuRead(uint16_t addr, bool rdOnly = false);
  void cpuWrite(uint16_t addr, uint8_t data);

  // Communication with PPU Bus
  uint8_t ppuRead(uint16_t addr, bool rdOnly = false);
  void ppuWrite(uint16_t addr, uint8_t data);

protected:
private:
};
