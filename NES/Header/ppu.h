#pragma once

#include "cartridge.h"
#include "pixelEngine.h"
#include <cstdint>
#include <memory>

class ppu2C02
{
public:
  ppu2C02();
  ~ppu2C02();

  // Communication with main Bus
  uint8_t cpuRead(uint16_t addr, bool rdOnly = false);
  void cpuWrite(uint16_t addr, uint8_t data);

  // Communication with PPU Bus
  uint8_t ppuRead(uint16_t addr, bool rdOnly = false);
  void ppuWrite(uint16_t addr, uint8_t data);

  olc::Sprite& GetScreen();
  olc::Sprite& GetNameTable();
  olc::Sprite& GetPatternTable();

  bool frame_complete{false};

  void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
  void clock();

protected:
private:
  uint8_t tblName[2][1024];
  uint8_t tblPalette[2][4096];

  olc::Pixel palScreen[0x40];
  olc::Sprite* sprScreen;
  olc::Sprite* sprNameTable[2];
  olc::Sprite* sprPatternTable[2];

  int16_t scanline{0};
  int16_t cycle{0};

  std::shared_ptr<Cartridge> cart;
};
