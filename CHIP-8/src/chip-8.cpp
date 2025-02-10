#include "time.h"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>

#include "chip-8.h"

unsigned char chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

CHIP8::CHIP8()
{
}
CHIP8::~CHIP8()
{
}

// Initlialization
void CHIP8::init()
{
  pc = 0x200; // Set program counter to 0x200
  opcode = 0; // Reset op code
  I = 0;      // Reset I
  sp = 0;     // Reset stack pointer

  for (int i = 0; i < 2048; i++) // Clear display
  {
    gfx[i] = 0;
  }

  for (int i = 0; i < 16; i++) // Clear stack, keypad, and V register
  {
    stack[i] = 0;
    key[i] = 0;
    V[i] = 0;
  }

  for (int i = 0; i < 4096; i++) // Clear memory
  {
    memory[i] = 0;
  }

  for (int i = 0; i < 80; i++) // Load font set into memory
  {
    memory[i] = chip8_fontset[i];
  }

  // Reset timers
  delay_timer = 0;
  sound_timer = 0;

  srand(time(NULL)); // Seed rng
}

// Initlialize and Load ROM into memory
bool CHIP8::load(const char* file_path)
{
  init();
  printf("Loading ROM: %s\n", file_path);

  // Open ROM file
  FILE* rom = fopen(file_path, "rb");
  if (rom == NULL)
  {
    std::cerr << "Failed to open ROM " << std::endl;
    return false;
  }

  fseek(rom, 0, SEEK_END); // Get file size
  long rom_size = ftell(rom);
  rewind(rom);

  // Allocate memory to store rom
  char* rom_buffer = (char*)malloc(sizeof(char) * rom_size);
  if (rom_buffer == NULL)
  {
    std::cerr << "Failed to allocate memory for ROM" << std::endl;
    return false;
  }

  // Copy ROM into buffer
  size_t result = fread(rom_buffer, sizeof(char), (size_t)rom_size, rom);
  if (result != rom_size)
  {
    std::cerr << "Failed to read ROM" << std::endl;
    return false;
  }

  // Copy buffer to memory
  if ((4096 - 512) > rom_size)
  {
    for (int i = 0; i < rom_size; i++)
    {
      memory[i + 512] = (uint8_t)rom_buffer[i]; // Load into memory starting at 0x200 (=512)
    }
  }
  else
  {
    std::cerr << "ROM too larget to fit into the memory" << std::endl;
    return false;
  }

  // Clean up
  fclose(rom);
  free(rom_buffer);
  return true;
}

// Emulate one cycle
void CHIP8::emulate_cycle()
{
  // Fetch op code
  opcode = memory[pc] << 8 | memory[pc + 1]; // opcode is two bytes
  switch (opcode & 0xF000)
  {
  // 00E_
  case 0x0000:
    switch (opcode & 0x000F)
    {
    // 00E0 - Clear Screen
    case 0x0000:
      for (int i = 0; i < 2048; i++)
      {
        gfx[i] = 0;
      }
      drawFlag = true;
      pc += 2;
      break;

    // 00EE - Return from subroutine
    case 0x000E:
      --sp;
      pc = stack[sp];
      pc += 2;
      break;

    default:
      printf("\nUnknown op code %.4X\n", opcode);
      break;
    }
    break;

  // 1NNN - Jumps to address NNN
  case 0x1000:
    pc = opcode & 0x0FFF;
    break;

  // 2NNN - Calls subroutine at NNN
  case 0x2000:
    stack[sp] = pc;
    ++sp;
    pc = opcode & 0x0FFF;
    break;
  // 3XNN - Skip next instruction if VX equals to NN.
  case 0x3000:
    if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
      pc += 4;
    else
      pc += 2;
    break;
  // 4XNN - Skip next instruction if VX is not equal to NN.
  case 0x4000:
    if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
      pc += 4;
    else
      pc += 2;
    break;
  // 5XYO - Skip the next instruction if VX equals to VY
  case 0x5000:
    if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
      pc += 4;
    else
      pc += 2;
    break;
  // 6XNN - Set VX to NN
  case 0x6000:
    V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
    pc += 2;
    break;
  // 7XNN - Add NN to VX
  case 0x7000:
    V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
    pc += 2;
    break;
  // 8XY_
  case 0x8000:
    switch (opcode & 0x000F)
    {
    // 8XY0 - Set VX to value of VY
    case 0x0000:
      V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    // 8XY1 - Set VX to (VX OR VY)
    case 0x0001:
      V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    // 8XY2 - Set VX to (VX AND VY)
    case 0x0002:
      V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    // 8XY3 - Set VX to (VX XOR VY)
    case 0x0003:
      V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    // 8XY4 - Add VY to VX. VF is set to 1 when there's a carry and to 0 when there isn't
    case 0x0004:
      V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
      if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
        V[0xF] = 1;
      else
        V[0xF] = 0;
      pc += 2;
      break;
    // 8XY5 - VY is subtracted from VX. VF is set to 0 when there's a borrow and to 1 elsewise
    case 0x0005:
      if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
        V[0xF] = 0;
      else
        V[0xF] = 1;
      V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    // 8XY6 - Shifts VX right by one. VF is set to value of the least significant bit of VX before
    // the shift.
    case 0x0006:
      V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
      V[(opcode & 0x0F00) >> 8] >>= 1;
      pc += 2;
      break;
    // 8XY7 - Set VX to VY-VX. VF is set to 0 when there's a borrow and to 1 elsewise
    case 0x0007:
      if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
        V[0xF] = 0;
      else
        V[0xF] = 1;
      V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
      pc += 2;
      break;
    // 8XYE - Shift VX by left by one. VF is set to value of most significant bit of VX before the
    // shift.
    case 0x000E:
      V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
      V[(opcode & 0x0F00) >> 8] <<= 1;
      pc += 2;
      break;
    default:
      printf("\nUnknown op code %.4X\n", opcode);
      exit(3);
    }
    break;
  // 9XY0 - Skip next instruction if VX doesn't equal to VY
  case 0x9000:
    if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
      pc += 4;
    else
      pc += 2;
    break;
  // ANNN - Set I to address of NNN
  case 0xA000:
    I = opcode & 0x0FFF;
    pc += 2;
    break;
  // BNNN - Jumps to address NNN plus V0
  case 0xB000:
    pc = (opcode & 0x0FFF) + V[0];
    break;
  // CXNN - Set VX to random number, masked by NN
  case 0xC000:
    V[(opcode & 0x0F00) >> 8] = (rand() % (0xFF + 1)) & (opcode & 0x0FFF);
    pc += 2;
    break;
  // DXYN - Draw a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N
  // pixels Each row of 8 pixels is read as bit-coded starting from memory location I; I value
  // doesn't change after execution of this instruction. VF is set to 1 if screen pixel are flipped
  // from set to unset when sprite is drawn. and to 0 elsewise
  case 0xD000:
  {
    unsigned short x = V[(opcode & 0x0F00) >> 8];
    unsigned short y = V[(opcode & 0x00F0) >> 4];
    unsigned short height = opcode & 0x000F;
    unsigned short pixel;

    V[0xF] = 0;
    for (int yline = 0; yline < height; yline++)
    {
      pixel = memory[I + yline];
      for (int xline = 0; xline < 8; xline++)
      {
        if ((pixel & (0x80 >> xline)) != 0)
        {
          if (gfx[(x + xline + ((y + yline) * 64))] == 1)
          {
            V[0xF] = 1;
          }
          gfx[x + xline + ((y + yline) * 64)] ^= 1;
        }
      }
    }
    drawFlag = true;
    pc += 2;
  }
  break;
  // EX__
  case 0xE000:
    switch (opcode & 0x00FF)
    {
    // EX9E - Skip next instruction if key with the value of VX is pressed
    case 0x009E:
      if (key[V[(opcode & 0x0F00) >> 8]] != 0)
        pc += 4;
      else
        pc += 2;
      break;
    // EXA1 - Skip next instruction if key with the value of VX is not pressed
    case 0x00A1:
      if (key[V[(opcode & 0x0F00) >> 8]] == 0)
        pc += 4;
      else
        pc += 2;
      break;
    default:
      printf("\nUnknown op code %.4X\n", opcode);
      exit(3);
    }
    break;
  // FX__
  case 0xF000:
    switch (opcode & 0x00FF)
    {
    // FX07 - Set VX to value of delay timer
    case 0x0007:
      V[(opcode & 0x0F00) >> 8] = delay_timer;
      pc += 2;
      break;
    // FX0A - A key press is awaited, and then stored in VX
    case 0x000A:
    {
      bool key_pressed = false;
      for (int i = 0; i < 16; i++)
      {
        if (key[i] != 0)
        {
          V[(opcode & 0x0F00) >> 8] = i;
          key_pressed = true;
        }
      }
      if (!key_pressed)
        return;
      pc += 2;
    }
    break;
    // FX15 - Set delay timer to VX
    case 0x0015:
      delay_timer = V[(opcode & 0x0F00) >> 8];
      pc += 2;
      break;
    // FX18 - Set sound timer to VX
    case 0x0018:
      sound_timer = V[(opcode & 0x0F00) >> 8];
      pc += 2;
      break;
    // FX1E - Add VX to I
    case 0x001E:
      // VF is set to 1 when range overflow (I+VX>0xFFF), and 0 when there isn't.
      if (I + V[(opcode & 0x0F00) >> 8] > 0xFFF)
        V[0xF] = 1;
      else
        V[0xF] = 0;
      I += V[(opcode & 0x0F00) >> 8];
      pc += 2;
      break;
    // FX29 - Set I to the location of the sprite for the character in VX. Characters 0-F (in
    // hexadecimal) are represented by a 4x5 font.
    case 0x0029:
      I = V[(opcode & 0x0F00) >> 8] * 0x5;
      pc += 2;
      break;
    // FX33 - Store the binary-coded decimal representation of VX, with the most significant of
    // three digits at the address in I, the middle digit at I plus 1, and the least significant
    // digit at I plus 2.
    case 0x0033:
      memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
      memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
      memory[I + 2] = V[(opcode & 0x0F00) >> 8] % 10;
      pc += 2;
      break;
    // FX55 - Store V0 to VX in memory starting at address I
    case 0x0055:
      for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
      {
        memory[I + i] = V[i];
      }
      I += ((opcode & 0x0F00) >> 8) + 1;
      pc += 2;
      break;
    // FX65 - Fill V0 to VX with values from memory starting at address I
    case 0x0065:
      for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
      {
        V[i] = memory[I + i];
      }
      I += ((opcode & 0x0F00) >> 8) + 1;
      pc += 2;
      break;
    default:
      printf("\nUnknown op code %.4X\n", opcode);
      exit(3);
    }
    break;
  default:
    printf("\nUnknown op code %.4X\n", opcode);
    exit(3);
  }

  // Update timers
  if (delay_timer > 0)
    --delay_timer;
  if (sound_timer > 0)
  {
    if (sound_timer == 1)
      printf("BEEP!\n");
    --sound_timer;
  }
}
