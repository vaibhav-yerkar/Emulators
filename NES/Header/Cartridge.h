#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "mapper_000.h"

class Cartridge
{
public:
  enum MIRROR
  {
    HORIZONTAL,
    VERTICAL,
    ONSCREEN_LO,
    ONSCREEN_HI,
  } mirror{HORIZONTAL};

  Cartridge(const std::string& fileName);
  ~Cartridge();

  bool ImageVaild();

  // Communication with main Bus
  bool cpuRead(uint16_t addr, uint8_t& data);
  bool cpuWrite(uint16_t addr, uint8_t data);

  // Communication with PPU Bus
  bool ppuRead(uint16_t addr, uint8_t& data);
  bool ppuWrite(uint16_t addr, uint8_t data);

protected:
private:
  bool bImageValid{false};

  uint8_t nMapperID = 0;
  uint8_t nPRGBanks = 0;
  uint8_t nCHRBanks = 0;

  std::vector<uint8_t> vPRGMemory;
  std::vector<uint8_t> vCHRMemory;

  std::shared_ptr<Mapper> pMapper;
};
