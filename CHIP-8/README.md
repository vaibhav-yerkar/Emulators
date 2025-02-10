# CHIP-8

(Yet another) CHIP-8 emulator written in CPP.

There's nothing particularly special about this CHIP-8 emulator, but it does aim to be faithful to the timings
of the sub-systems. For example, it aims to run the exact number of CPU cycles before drawing the screen. It's
currently hard-coded to run the screen at 60Hz and the CPU at 700Hz, but those values are easy enough to change.

### Build

> install sdl2
> install cmake

```bash
mkdir build
cd build
cmake ../
make
```

### Usage

```bash
./chip8 ../roms/<rom_file_name>
```

### Controls

CHIP-8 has a keypad with 16 keys. This emulator uses:

```
1 2 3 4
q w e r
a s d f
z x c v
```

Apologies to anyone with a non-QWERTY keyboard...

`<Esc>` to quit.

`<F1>` to restart.

## Credits

I followed the fantastic write-up at https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
