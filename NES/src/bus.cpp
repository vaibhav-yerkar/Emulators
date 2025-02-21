#include "bus.h"

Bus::Bus()
{
  cpu.connectBus(this);
  for (auto& i : ram)
    i = 0x00;
}

Bus::~Bus()
{
}

void Bus::write(uint16_t addr, uint8_t data)
{
  if (addr >= 0x0000 and addr <= 0xFFFF)
    ram[addr] = data;
}

uint8_t Bus::read(uint16_t addr, bool readOnly)
{
  if (addr >= 0x0000 and addr <= 0xFFFF)
    return ram[addr];

  return 0x00;
}
