#include "cpu.h"
#include "../bus.h"
#include <cstdint>

cpu6502::cpu6502()
{
  using cpu = cpu6502;
  lookup = {
      {"BRK", &cpu::BRK, &cpu::IMM, 7}, {"ORA", &cpu::ORA, &cpu::IZX, 6},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"???", &cpu::NOP, &cpu::IMP, 3}, {"ORA", &cpu::ORA, &cpu::ZP0, 3},
      {"ASL", &cpu::ASL, &cpu::ZP0, 5}, {"???", &cpu::XXX, &cpu::IMP, 5},
      {"PHP", &cpu::PHP, &cpu::IMP, 3}, {"ORA", &cpu::ORA, &cpu::IMM, 2},
      {"ASL", &cpu::ASL, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 2},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"ORA", &cpu::ORA, &cpu::ABS, 4},
      {"ASL", &cpu::ASL, &cpu::ABS, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},

      {"BPL", &cpu::BPL, &cpu::REL, 2}, {"ORA", &cpu::ORA, &cpu::IZY, 5},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"ORA", &cpu::ORA, &cpu::ZPX, 4},
      {"ASL", &cpu::ASL, &cpu::ZPX, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},
      {"CLC", &cpu::CLC, &cpu::IMP, 2}, {"ORA", &cpu::ORA, &cpu::ABY, 4},
      {"???", &cpu::NOP, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 7},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"ORA", &cpu::ORA, &cpu::ABX, 4},
      {"ASL", &cpu::ASL, &cpu::ABX, 7}, {"???", &cpu::XXX, &cpu::IMP, 7},

      {"JSR", &cpu::JSR, &cpu::ABS, 6}, {"AND", &cpu::AND, &cpu::IZX, 6},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"BIT", &cpu::BIT, &cpu::ZP0, 3}, {"AND", &cpu::AND, &cpu::ZP0, 3},
      {"ROL", &cpu::ROL, &cpu::ZP0, 5}, {"???", &cpu::XXX, &cpu::IMP, 5},
      {"PLP", &cpu::PLP, &cpu::IMP, 4}, {"AND", &cpu::AND, &cpu::IMM, 2},
      {"ROL", &cpu::ROL, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 2},
      {"BIT", &cpu::BIT, &cpu::ABS, 4}, {"AND", &cpu::AND, &cpu::ABS, 4},
      {"ROL", &cpu::ROL, &cpu::ABS, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},

      {"BMI", &cpu::BMI, &cpu::REL, 2}, {"AND", &cpu::AND, &cpu::IZY, 5},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"AND", &cpu::AND, &cpu::ZPX, 4},
      {"ROL", &cpu::ROL, &cpu::ZPX, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},
      {"SEC", &cpu::SEC, &cpu::IMP, 2}, {"AND", &cpu::AND, &cpu::ABY, 4},
      {"???", &cpu::NOP, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 7},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"AND", &cpu::AND, &cpu::ABX, 4},
      {"ROL", &cpu::ROL, &cpu::ABX, 7}, {"???", &cpu::XXX, &cpu::IMP, 7},

      {"RTI", &cpu::RTI, &cpu::IMP, 6}, {"EOR", &cpu::EOR, &cpu::IZX, 6},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"???", &cpu::NOP, &cpu::IMP, 3}, {"EOR", &cpu::EOR, &cpu::ZP0, 3},
      {"LSR", &cpu::LSR, &cpu::ZP0, 5}, {"???", &cpu::XXX, &cpu::IMP, 5},
      {"PHA", &cpu::PHA, &cpu::IMP, 3}, {"EOR", &cpu::EOR, &cpu::IMM, 2},
      {"LSR", &cpu::LSR, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 2},
      {"JMP", &cpu::JMP, &cpu::ABS, 3}, {"EOR", &cpu::EOR, &cpu::ABS, 4},
      {"LSR", &cpu::LSR, &cpu::ABS, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},

      {"BVC", &cpu::BVC, &cpu::REL, 2}, {"EOR", &cpu::EOR, &cpu::IZY, 5},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"EOR", &cpu::EOR, &cpu::ZPX, 4},
      {"LSR", &cpu::LSR, &cpu::ZPX, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},
      {"CLI", &cpu::CLI, &cpu::IMP, 2}, {"EOR", &cpu::EOR, &cpu::ABY, 4},
      {"???", &cpu::NOP, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 7},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"EOR", &cpu::EOR, &cpu::ABX, 4},
      {"LSR", &cpu::LSR, &cpu::ABX, 7}, {"???", &cpu::XXX, &cpu::IMP, 7},

      {"RTS", &cpu::RTS, &cpu::IMP, 6}, {"ADC", &cpu::ADC, &cpu::IZX, 6},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"???", &cpu::NOP, &cpu::IMP, 3}, {"ADC", &cpu::ADC, &cpu::ZP0, 3},
      {"ROR", &cpu::ROR, &cpu::ZP0, 5}, {"???", &cpu::XXX, &cpu::IMP, 5},
      {"PLA", &cpu::PLA, &cpu::IMP, 4}, {"ADC", &cpu::ADC, &cpu::IMM, 2},
      {"ROR", &cpu::ROR, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 2},
      {"JMP", &cpu::JMP, &cpu::IND, 5}, {"ADC", &cpu::ADC, &cpu::ABS, 4},
      {"ROR", &cpu::ROR, &cpu::ABS, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},

      {"BVS", &cpu::BVS, &cpu::REL, 2}, {"ADC", &cpu::ADC, &cpu::IZY, 5},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"ADC", &cpu::ADC, &cpu::ZPX, 4},
      {"ROR", &cpu::ROR, &cpu::ZPX, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},
      {"SEI", &cpu::SEI, &cpu::IMP, 2}, {"ADC", &cpu::ADC, &cpu::ABY, 4},
      {"???", &cpu::NOP, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 7},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"ADC", &cpu::ADC, &cpu::ABX, 4},
      {"ROR", &cpu::ROR, &cpu::ABX, 7}, {"???", &cpu::XXX, &cpu::IMP, 7},

      {"???", &cpu::NOP, &cpu::IMP, 2}, {"STA", &cpu::STA, &cpu::IZX, 6},
      {"???", &cpu::NOP, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 6},
      {"STY", &cpu::STY, &cpu::ZP0, 3}, {"STA", &cpu::STA, &cpu::ZP0, 3},
      {"STX", &cpu::STX, &cpu::ZP0, 3}, {"???", &cpu::XXX, &cpu::IMP, 3},
      {"DEY", &cpu::DEY, &cpu::IMP, 2}, {"???", &cpu::NOP, &cpu::IMP, 2},
      {"TXA", &cpu::TXA, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 2},
      {"STY", &cpu::STY, &cpu::ABS, 4}, {"STA", &cpu::STA, &cpu::ABS, 4},
      {"STX", &cpu::STX, &cpu::ABS, 4}, {"???", &cpu::XXX, &cpu::IMP, 4},

      {"BCC", &cpu::BCC, &cpu::REL, 2}, {"STA", &cpu::STA, &cpu::IZY, 6},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 6},
      {"STY", &cpu::STY, &cpu::ZPX, 4}, {"STA", &cpu::STA, &cpu::ZPX, 4},
      {"STX", &cpu::STX, &cpu::ZPY, 4}, {"???", &cpu::XXX, &cpu::IMP, 4},
      {"TYA", &cpu::TYA, &cpu::IMP, 2}, {"STA", &cpu::STA, &cpu::ABY, 5},
      {"TXS", &cpu::TXS, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 5},
      {"???", &cpu::NOP, &cpu::IMP, 5}, {"STA", &cpu::STA, &cpu::ABX, 5},
      {"???", &cpu::XXX, &cpu::IMP, 5}, {"???", &cpu::XXX, &cpu::IMP, 5},

      {"LDY", &cpu::LDY, &cpu::IMM, 2}, {"LDA", &cpu::LDA, &cpu::IZX, 6},
      {"LDX", &cpu::LDX, &cpu::IMM, 2}, {"???", &cpu::XXX, &cpu::IMP, 6},
      {"LDY", &cpu::LDY, &cpu::ZP0, 3}, {"LDA", &cpu::LDA, &cpu::ZP0, 3},
      {"LDX", &cpu::LDX, &cpu::ZP0, 3}, {"???", &cpu::XXX, &cpu::IMP, 3},
      {"TAY", &cpu::TAY, &cpu::IMP, 2}, {"LDA", &cpu::LDA, &cpu::IMM, 2},
      {"TAX", &cpu::TAX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 2},
      {"LDY", &cpu::LDY, &cpu::ABS, 4}, {"LDA", &cpu::LDA, &cpu::ABS, 4},
      {"LDX", &cpu::LDX, &cpu::ABS, 4}, {"???", &cpu::XXX, &cpu::IMP, 4},

      {"BCS", &cpu::BCS, &cpu::REL, 2}, {"LDA", &cpu::LDA, &cpu::IZY, 5},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 5},
      {"LDY", &cpu::LDY, &cpu::ZPX, 4}, {"LDA", &cpu::LDA, &cpu::ZPX, 4},
      {"LDX", &cpu::LDX, &cpu::ZPY, 4}, {"???", &cpu::XXX, &cpu::IMP, 4},
      {"CLV", &cpu::CLV, &cpu::IMP, 2}, {"LDA", &cpu::LDA, &cpu::ABY, 4},
      {"TSX", &cpu::TSX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 4},
      {"LDY", &cpu::LDY, &cpu::ABX, 4}, {"LDA", &cpu::LDA, &cpu::ABX, 4},
      {"LDX", &cpu::LDX, &cpu::ABY, 4}, {"???", &cpu::XXX, &cpu::IMP, 4},

      {"CPY", &cpu::CPY, &cpu::IMM, 2}, {"CMP", &cpu::CMP, &cpu::IZX, 6},
      {"???", &cpu::NOP, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"CPY", &cpu::CPY, &cpu::ZP0, 3}, {"CMP", &cpu::CMP, &cpu::ZP0, 3},
      {"DEC", &cpu::DEC, &cpu::ZP0, 5}, {"???", &cpu::XXX, &cpu::IMP, 5},
      {"INY", &cpu::INY, &cpu::IMP, 2}, {"CMP", &cpu::CMP, &cpu::IMM, 2},
      {"DEX", &cpu::DEX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 2},
      {"CPY", &cpu::CPY, &cpu::ABS, 4}, {"CMP", &cpu::CMP, &cpu::ABS, 4},
      {"DEC", &cpu::DEC, &cpu::ABS, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},

      {"BNE", &cpu::BNE, &cpu::REL, 2}, {"CMP", &cpu::CMP, &cpu::IZY, 5},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"CMP", &cpu::CMP, &cpu::ZPX, 4},
      {"DEC", &cpu::DEC, &cpu::ZPX, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},
      {"CLD", &cpu::CLD, &cpu::IMP, 2}, {"CMP", &cpu::CMP, &cpu::ABY, 4},
      {"NOP", &cpu::NOP, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 7},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"CMP", &cpu::CMP, &cpu::ABX, 4},
      {"DEC", &cpu::DEC, &cpu::ABX, 7}, {"???", &cpu::XXX, &cpu::IMP, 7},

      {"CPX", &cpu::CPX, &cpu::IMM, 2}, {"SBC", &cpu::SBC, &cpu::IZX, 6},
      {"???", &cpu::NOP, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"CPX", &cpu::CPX, &cpu::ZP0, 3}, {"SBC", &cpu::SBC, &cpu::ZP0, 3},
      {"INC", &cpu::INC, &cpu::ZP0, 5}, {"???", &cpu::XXX, &cpu::IMP, 5},
      {"INX", &cpu::INX, &cpu::IMP, 2}, {"SBC", &cpu::SBC, &cpu::IMM, 2},
      {"NOP", &cpu::NOP, &cpu::IMP, 2}, {"???", &cpu::SBC, &cpu::IMP, 2},
      {"CPX", &cpu::CPX, &cpu::ABS, 4}, {"SBC", &cpu::SBC, &cpu::ABS, 4},
      {"INC", &cpu::INC, &cpu::ABS, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},

      {"BEQ", &cpu::BEQ, &cpu::REL, 2}, {"SBC", &cpu::SBC, &cpu::IZY, 5},
      {"???", &cpu::XXX, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 8},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"SBC", &cpu::SBC, &cpu::ZPX, 4},
      {"INC", &cpu::INC, &cpu::ZPX, 6}, {"???", &cpu::XXX, &cpu::IMP, 6},
      {"SED", &cpu::SED, &cpu::IMP, 2}, {"SBC", &cpu::SBC, &cpu::ABY, 4},
      {"NOP", &cpu::NOP, &cpu::IMP, 2}, {"???", &cpu::XXX, &cpu::IMP, 7},
      {"???", &cpu::NOP, &cpu::IMP, 4}, {"SBC", &cpu::SBC, &cpu::ABX, 4},
      {"INC", &cpu::INC, &cpu::ABX, 7}, {"???", &cpu::XXX, &cpu::IMP, 7},
  };
}

cpu6502::~cpu6502()
{
}

uint8_t cpu6502::read(uint16_t addr)
{
  return bus->read(addr, false); // read 8-bit frm bus
}

void cpu6502::write(uint16_t addr, uint8_t data)
{
  bus->write(addr, data); // write a byte to bus
}

void cpu6502::reset()
{
  // Get address to set program counter to
  abs_addr = 0xFFFC;

  uint16_t low = read(abs_addr + 0);
  uint16_t high = read(abs_addr + 1);

  pc = (high << 8) | low; // setting program counter

  // reset registers
  a = 0;
  x = 0;
  y = 0;
  sp = 0xFD;
  status = 0x00 | U;

  // Clear helper variable
  abs_addr = 0x0000;
  rel_addr = 0x0000;
  fetched = 0x00;

  // Reset takes time
  cycles = 8;
}

void cpu6502::ireq()
{
  if (getFlag(I) == 0)
  {
    // push program counter to stack
    write(0x0100 + sp, (pc >> 8) & 0x00FF);
    sp--;
    write(0x0100 + sp, pc & 0x00FF);
    sp--;

    // push status register to stack
    setFlag(B, 0);
    setFlag(U, 1);
    setFlag(I, 1);
    write(0x0100 + sp, status);
    sp--;

    // Read new pc location from fixed adderss
    abs_addr = 0xFFFE;
    uint16_t low = read(abs_addr + 0);
    uint16_t high = read(abs_addr + 1);
    pc = (high << 8) | low;

    cycles = 7;
  }
}

void cpu6502::nmi()
{
  write(0x0100 + sp, (pc >> 8) & 0x00FF);
  sp--;
  write(0x0100 + sp, pc & 0x00FF);
  sp--;

  setFlag(B, 0);
  setFlag(U, 1);
  setFlag(I, 1);

  write(0x0100 + sp, status);
  sp--;

  abs_addr = 0xFFFA;
  uint16_t low = read(abs_addr + 0);
  uint16_t high = read(abs_addr + 1);
  pc = (high << 8) | low;

  cycles = 8;
}

void cpu6502::clock()
{
  if (cycles == 0)
  {
    opcode = read(pc); // Read next instruction

#ifdef LOGMODE
    uint16_t log_pc = pc;
#endif // LOGMODE

    // Set the unset flag bit to 1
    setFlag(U, true);

    // Increment pc
    pc++;

    cycles = lookup[opcode].cycles;

    // Perform fetch of intermediate data
    uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();
    // perform operation
    uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

    cycles += (additional_cycle1 & additional_cycle2);

    setFlag(U, true);

#ifdef LOGMODE
    if (logfile == nullptr)
      logfile = fopen("cpu6502.txt", "wt");
    if (logfile != nullptr)
    {
      fprintf(logfile, "%10d:%02d PC:%04X %s A:%02X Y:%02X %s%s%s%s%s%s%s SP:%02X\n", clock_count,
              0, log_pc, "XXX", a, x, y, getFlag(N) ? "N" : ".", getFlag(V) ? "V" : ".",
              getFlag(U) ? "U" : ".", getFlag(B) ? "B" : ".", getFlag(D) ? "D" : ".",
              getFlag(I) ? "I" : ".", getFlag(Z) ?: "Z" : ".", getFlag(C) ? "C" : ".", sp);
    }
#endif // LOGMODE
  }
  clock_count++;
  cycles--;
}

uint8_t cpu6502::getFlag(Flag6502 flag)
{
  return ((status & flag) > 0) ? 1 : 0;
}

void cpu6502::setFlag(Flag6502 flag, bool value)
{
  if (value)
    status |= flag;
  else
    status &= ~flag;
}

// ADDRESSING MODES ===================
uint8_t cpu6502::IMP() // Implied
{
  fetched = a;
  return 0;
}
uint8_t cpu6502::IMM() // Immediate
{
  abs_addr = pc++;
  return 0;
}
uint8_t cpu6502::ZP0() // Zero Page
{
  abs_addr = read(pc);
  pc++;
  abs_addr &= 0x00FF;
  return 0;
}
uint8_t cpu6502::ZPX() // Zero Page with X offset
{
  abs_addr = read(pc + x);
  pc++;
  abs_addr &= 0x00FF;
  return 0;
}
uint8_t cpu6502::ZPY() // Zero Page with Y offset
{
  abs_addr = read(pc + y);
  pc++;
  abs_addr &= 0x00FF;
  return 0;
}
uint8_t cpu6502::REL() // Relative
{
  rel_addr = read(pc);
  pc++;
  if (rel_addr & 0x80)
    rel_addr |= 0xFF00;
  return 0;
}
uint8_t cpu6502::ABS() // Absolute
{
  uint16_t low = read(pc);
  pc++;
  uint16_t high = read(pc);
  pc++;
  abs_addr = (high << 8) | low;
  return 0;
}
uint8_t cpu6502::ABX() // Absolute with X offset
{
  uint16_t low = read(pc);
  pc++;
  uint16_t high = read(pc);
  pc++;
  abs_addr = (high << 8) | low;
  abs_addr += x;
  if ((abs_addr & 0xFF00) != (high << 8))
    return 1;
  else
    return 0;
}
uint8_t cpu6502::ABY() // Absolute with Y offset
{
  uint16_t low = read(pc);
  pc++;
  uint16_t high = read(pc);
  pc++;
  abs_addr = (high << 8) | low;
  abs_addr += y;
  if ((abs_addr & 0xFF00) != (high << 8))
    return 1;
  else
    return 0;
}
uint8_t cpu6502::IND() // Indirect
{
  uint16_t ptr_low = read(pc);
  pc++;
  uint16_t ptr_high = read(pc);
  pc++;
  uint16_t ptr = (ptr_high << 8) | ptr_low;
  if (ptr_low == 0x00FF)
  {
    abs_addr = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
  }
  else
  {
    abs_addr = (read(ptr + 1) << 8) | read(ptr + 0);
  }
  return 0;
}
uint8_t cpu6502::IZX() // Indirect X
{
  uint16_t t = read(pc);
  pc++;
  uint16_t low = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
  uint16_t high = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

  abs_addr = (high << 8) | low;
  return 0;
}
uint8_t cpu6502::IZY() // Indirect Y
{
  uint16_t t = read(pc);
  pc++;
  uint16_t low = read(t & 0x00FF);
  uint16_t high = read((t + 1) & 0x00FF);
  abs_addr = (high << 8) | low;
  abs_addr += y;
  if ((abs_addr & 0xFF00) != (high << 8))
    return 1;
  else
    return 0;
}

uint8_t cpu6502::fetch()
{
  if (!(lookup[opcode].addrmode == &cpu6502::IMP))
    fetched = read(abs_addr);
  return fetched;
}

// OPCODES IMPELEMENTATION ============

uint8_t cpu6502::ADC() // Add with Carry In
{
  fetch();
  temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)getFlag(C);
  setFlag(C, temp > 255);
  setFlag(Z, (temp & 0x00FF) == 0);
  setFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);
  setFlag(N, temp & 0x00);
  a = temp & 0x00FF;
  return 1;
}
uint8_t cpu6502::AND() // Bitwise logic AND
{
  fetch();
  a = a & fetched;
  setFlag(Z, a == 0x00);
  setFlag(N, a & 0x80);
  return 1;
}
uint8_t cpu6502::ASL() // Arithmetic Shift Left
{
  fetch();
}
uint8_t cpu6502::SBC() // Subtract wuth Borrow In
{
  fetch();
  uint16_t value = ((uint16_t)fetched) ^ 0x00FF;
  temp = (uint16_t)a + value + (uint16_t)getFlag(C);
  setFlag(C, temp & 0xFF00);
  setFlag(Z, ((temp & 0xFF00) == 0));
  setFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0000);
  setFlag(N, temp & 0x0080);
  a = temp & 0x00FF;
  return 1;
}
