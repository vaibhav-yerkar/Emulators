# CHIP-8 EMULATOR

## HISTORY

CHIP-8 was created by RCA engineer Joe Weisbecker in 1977 for COSMAC VIP microcomputer. IT was intended as a simpler way to make small programs and games for the computer. Instead of using machine language fo the VIP's CDP1802 processor, you could type in hecadecimal instructions (with the VIP's hex keypad) that resembled machine code, but which were more high-level , and interpreted on the fly by a small program (the CHIP-8 emulator/interpreter).

## SPECIFICATIONS (Components of the chip-8) :

- **Memory** : CHIP-8 has direct access to up to 4 kilobytes of RAM
- **Display** : 64x32 pixels (or 128x64 for SUPER_CHIP) monochrome, i.e, black and white
- A _program counter_, often called just "**PC**" , which points at the current instruction in memory
- One 16-bit _index register_ called "**I**" which is used to point at locations in memory
- A stack for 16-bit addresses, which is used to call subroutines/functions and return from them
- An 8-bit _delay timer_ which is decremented at a rate of 60Hz (60 times per second) until it reaches 0
- An 8-bit _sound timer_ which functions like the dealy timer, but which also gives off a beeping sound as long as it's not 0
- 16 8-bit (one-byte) general-purpose variable registers numbered `0` through `F` hexadecimal, ie. 0 through 15 in decimal, called `V0` through `VF`
  - `VF` is also used as a flag register , many instructions will set it to either 1 or 0 based on some rule, for example using it as a carry flag.
