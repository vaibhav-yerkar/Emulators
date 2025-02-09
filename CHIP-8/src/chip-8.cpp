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
