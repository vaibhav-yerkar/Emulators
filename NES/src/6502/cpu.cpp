#include "cpu.h"
#include "../bus.h"

cpu6502::cpu6502()
{
  using cpu = cpu6502;
  lookup = {
      {"BRK", &cpu::BRK, &cpu::IMM, 7}, {"ORA", &cpu::ORA, &cpu::IZX, 6},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"???", &cpu::NOP, &cpu::IMP, 3}, {"ORA", &cpu::ORA, &cpu::ZPO, 3},
      {"ASL", &cpu::ASL, &cpu::ZPO, 5}, {"???", &cpu::XXX, &cpu::IMP, 5},
      {"PHP", &cpu::PHP, &cpu::IMP, 3}, {"ORA", &cpu::ORA, &cpu::IMM, 2},
      {"ASL", &cpu::ASL, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 2},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"ORA", &cpu::ORA, &cpu::ABS, 4},
      {"ASL", &cpu::ASL, &cpu::ABS, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},

      {"BRK", &cpu::BRK, &cpu::IMM, 7}, {"ORA", &cpu::ORA, &cpu::IZX, 6},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"???", &cpu::NOP, &cpu::IMP, 3}, {"ORA", &cpu::ORA, &cpu::ZPO, 3},
      {"ASL", &cpu::ASL, &cpu::ZPO, 5}, {"???", &cpu::XXX, &cpu::IMP, 5},
      {"PHP", &cpu::PHP, &cpu::IMP, 3}, {"ORA", &cpu::ORA, &cpu::IMM, 2},
      {"ASL", &cpu::ASL, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 2},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"ORA", &cpu::ORA, &cpu::ABS, 4},
      {"ASL", &cpu::ASL, &cpu::ABS, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},

      {"BRK", &cpu::BRK, &cpu::IMM, 7}, {"ORA", &cpu::ORA, &cpu::IZX, 6},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"???", &cpu::NOP, &cpu::IMP, 3}, {"ORA", &cpu::ORA, &cpu::ZPO, 3},
      {"ASL", &cpu::ASL, &cpu::ZPO, 5}, {"???", &cpu::XXX, &cpu::IMP, 5},
      {"PHP", &cpu::PHP, &cpu::IMP, 3}, {"ORA", &cpu::ORA, &cpu::IMM, 2},
      {"ASL", &cpu::ASL, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 2},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"ORA", &cpu::ORA, &cpu::ABS, 4},
      {"ASL", &cpu::ASL, &cpu::ABS, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},

      {"BRK", &cpu::BRK, &cpu::IMM, 7}, {"ORA", &cpu::ORA, &cpu::IZX, 6},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"???", &cpu::NOP, &cpu::IMP, 3}, {"ORA", &cpu::ORA, &cpu::ZPO, 3},
      {"ASL", &cpu::ASL, &cpu::ZPO, 5}, {"???", &cpu::XXX, &cpu::IMP, 5},
      {"PHP", &cpu::PHP, &cpu::IMP, 3}, {"ORA", &cpu::ORA, &cpu::IMM, 2},
      {"ASL", &cpu::ASL, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 2},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"ORA", &cpu::ORA, &cpu::ABS, 4},
      {"ASL", &cpu::ASL, &cpu::ABS, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},
  };
}
