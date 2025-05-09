#include "../Header/cartridge.h"
#include <memory>

Cartridge::Cartridge(const std::string& fileName)
{
  struct sHeader
  {
    char name[4];
    uint8_t prg_rom_chuncks;
    uint8_t chr_rom_chuncks;
    uint8_t mapper1;
    uint8_t mapper2;
    uint8_t prg_ram_size;
    uint8_t tv_system1;
    uint8_t tv_system2;
    char unused[5];
  } header;

  bImageValid = false;

  std::ifstream ifs;
  ifs.open(std::string(RESOURCE_PATH) + "/roms/" + fileName,
           std::ifstream::binary);
  if (ifs.is_open())
  {
    ifs.read((char*)&header, sizeof(sHeader));

    if (header.mapper1 & 0x04)
    {
      ifs.seekg(512, std::ios_base::cur);
    }

    nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
    mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

    uint8_t nFileType = 1;

    if (nFileType == 0)
    {
    }
    if (nFileType == 1)
    {
      nPRGBanks = header.prg_rom_chuncks;
      vPRGMemory.resize(nPRGBanks * 16384);
      ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());

      nCHRBanks = header.chr_rom_chuncks;
      vCHRMemory.resize(nCHRBanks * 8192);
      ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
    }
    if (nFileType == 2)
    {
    }

    switch (nMapperID)
    {
    case 0:
      pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks);
      break;
    }

    bImageValid = true;
    ifs.close();
  }
}

Cartridge::~Cartridge()
{
}

bool Cartridge::ImageVaild()
{
  return bImageValid;
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data)
{
  uint32_t mapped_addr = 0;
  if (pMapper->cpuMapRead(addr, mapped_addr))
  {
    data = vPRGMemory[mapped_addr];
    return true;
  }
  return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
  uint32_t mapped_addr = 0;
  if (pMapper->cpuMapWrite(addr, mapped_addr))
  {
    vPRGMemory[mapped_addr] = data;
    return true;
  }
  return false;
}
