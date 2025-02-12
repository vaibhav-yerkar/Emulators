#include "stdint.h"
#include <SDL2/SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_video.h>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <thread>

#include "chip-8.h"

// Keypad keymap
uint8_t keymap[16] = {
    SDLK_x, SDLK_1, SDLK_2, SDLK_3, SDLK_q, SDLK_w, SDLK_e, SDLK_a,
    SDLK_s, SDLK_d, SDLK_z, SDLK_c, SDLK_4, SDLK_r, SDLK_f, SDLK_v,
};

int main(int argc, char** argv)
{
  // Command Use
  if (argc != 2)
  {
    std::cout << "Usage chip8 <ROM file>" << std::endl;
    return 1;
  }
  CHIP8 chip8 = CHIP8();

  int w = 1024; // Window width
  int h = 512;  // Window height

  SDL_Window* window = NULL;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    printf("SDL could not be initialized! SDL Error: %s\n", SDL_GetError());
    exit(1);
  }
  // Create window
  window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
                            SDL_WINDOW_SHOWN);
  if (window == NULL)
  {
    printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    exit(2);
  }
  // Create Renderer
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderSetLogicalSize(renderer, w, h);

  // Create texture that store frame buffer
  SDL_Texture* sdlTexture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
  // temp pixel buffer
  uint32_t pixels[2048];

load:
  // Attempt to load ROM
  if (!chip8.load(std::string(RESOURCE_PATH) + "/" + argv[1]))
    return 2;

  // Emulate loop
  while (true)
  {
    chip8.emulate_cycle();
    // Process SDL_Event
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
        exit(0);
      if (e.type == SDL_KEYDOWN)
      {
        if (e.key.keysym.sym == SDLK_ESCAPE)
          exit(0);
        if (e.key.keysym.sym == SDLK_F1)
          goto load;

        for (int i = 0; i < 16; ++i)
        {
          if (e.key.keysym.sym == keymap[i])
          {
            chip8.key[i] = 1;
          }
        }
      }
      if (e.type == SDL_KEYUP)
      {
        for (int i = 0; i < 16; ++i)
        {
          if (e.key.keysym.sym == keymap[i])
          {
            chip8.key[i] = 0;
          }
        }
      }
    }

    // if draw occured , render SDL screen
    if (chip8.drawFlag)
    {
      chip8.drawFlag = false;

      // Store pixels in temporary buffer
      for (int i = 0; i < 2048; ++i)
      {
        uint8_t pixel = chip8.gfx[i];
        pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
      }
      // Update SDL Texture
      SDL_UpdateTexture(sdlTexture, NULL, pixels, 64 * sizeof(Uint32));

      // Clear screen and render
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
      SDL_RenderPresent(renderer);
    }
    // Sleep to slow down emulation speed
    std::this_thread::sleep_for(std::chrono::microseconds(2000));
  }
}
