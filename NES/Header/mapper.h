#pragma once

#include <cstdint>

class Mapper
{
public:
  Mapper(uint8_t prgBanks, uint8_t chrBanks);
  ~Mapper();

  // Transform CPU bus address to PRG ROM offset
  virtual bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
  virtual bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;

  // Transform PPU bus address to CHR ROM offset
  virtual bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
  virtual bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;

protected:
  uint8_t nPRGBanks{0};
  uint8_t nCHRBanks{0};

private:
};
