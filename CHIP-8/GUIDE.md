# CHIP-8 EMULATOR

## A GUIDE TO BUILD THE CHIP-8 EMULATOR

### HISTORY

CHIP-8 was created by RCA engineer Joe Weisbecker in 1977 for COSMAC VIP microcomputer. IT was intended as a simpler way to make small programs and games for the computer. Instead of using machine language fo the VIP's CDP1802 processor, you could type in hecadecimal instructions (with the VIP's hex keypad) that resembled machine code, but which were more high-level , and interpreted on the fly by a small program (the CHIP-8 emulator/interpreter).

### SPECIFICATIONS (Components of the chip-8) :

- **Memory** : CHIP-8 has direct access to up to 4 kilobytes of RAM
- **Display** : 64x32 pixels (or 128x64 for SUPER_CHIP) monochrome, i.e, black and white
- A _program counter_, often called just "**PC**" , which points at the current instruction in memory
- One 16-bit _index register_ called "**I**" which is used to point at locations in memory
- A stack for 16-bit addresses, which is used to call subroutines/functions and return from them
- An 8-bit _delay timer_ which is decremented at a rate of 60Hz (60 times per second) until it reaches 0
- An 8-bit _sound timer_ which functions like the dealy timer, but which also gives off a beeping sound as long as it's not 0
- 16 8-bit (one-byte) general-purpose variable registers numbered `0` through `F` hexadecimal, ie. 0 through 15 in decimal, called `V0` through `VF`
  - `VF` is also used as a flag register , many instructions will set it to either 1 or 0 based on some rule, for example using it as a carry flag.

### MEMORY

The Memory should be 4kB large. CHIP-8's index register and program counter can only address 12 bit, which is 4096 addresses.
All the memory is RAM and should be considered to be writable. CHIP-8 games can, and do, modify themselves.

The first CHIP-8 interpreter (on the COSMAC VIP computer) was also located in RAM, from address `000` to `1FF`. It would expect a CHIP-8 program to be loaded into memory after it, starting at address `200` (512 in decimal). Although modern interpreters are not in the same memory space, you should do the same to be able to run the old programs; you can just leave the initial space empty, except for the font.

### FONTS

The CHIP-8 emulator should have a built-in font, with sprite data representing the hexadecimal numbers from `0` through `F`. Each font character should be 4 pixels wide by 5 pixels tall. These font sprites are drawn just like regular sprites: Then set the index register **I** to the character's memory location and then draw it.
There's a special instruction for setting **I** to character's address, so you can choose where to put it. Anywhere in first 512 bytes (`000-1FF`) is fine.
For some reason, it's become popular to put it at `050-09F` , so you can follow the convention if you want.

> 0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
>
> 0x20, 0x60, 0x20, 0x20, 0x70, // 1
>
> 0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
>
> 0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
>
> 0x90, 0x90, 0xF0, 0x10, 0x10, // 4
>
> 0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
>
> 0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
>
> 0xF0, 0x10, 0x20, 0x40, 0x40, // 7
>
> 0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
>
> 0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
>
> 0xF0, 0x90, 0xF0, 0x90, 0x90, // A
>
> 0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
>
> 0xF0, 0x80, 0x80, 0x80, 0xF0, // C
>
> 0xE0, 0x90, 0x90, 0x90, 0xE0, // D
>
> 0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
>
> 0xF0, 0x80, 0xF0, 0x80, 0x80 // F
