# Synopsis
Basic Chip8 emulator in C++.

There are several of these emulators around the internet and on github.
I did this originally because I thought writting a Chip8 emulator would
make for a good first rust project, however, since I did not know much about
Chip8, I thought I would do one in C++ first.

# Install

You will need a compiler that supports C++11, cmake, and SDL.  I don't
currently have a Ubuntu installation, but the following should work

```
apt-get install build-essential cmake libsdl2-dev libsdl2-mixer-dev
```

Or on arch
```
pacman -S cmake sdl2 sd2_mixer

# Usage

Simply run
```
chip8 <rom>
```

