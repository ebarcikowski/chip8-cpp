# Overview
Basic Chip8 emulator in C++.

There are several of these emulators around the internet and on github.
I did this originally because I thought writting a Chip8 emulator would
make for a good first rust project.  However, since I did not know much about
Chip8, I thought I would do one in C++ first.

There are lots of resources for Chip8 on the web and on github.  I used the
following sites:

http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
This author does a nice job laying out how Chip8 works, including nice source
snippets.  

https://en.wikipedia.org/wiki/CHIP-8
The wiki page describes all the opcodes so is a needed resource.

https://github.com/alexanderdickson/Chip-8-Emulator
I use ROMs from this github repository and browsed his source as well.

# Build

You will need a compiler that supports C++11, cmake, and SDL.  So far,
I have only built this on Linux with g++ 7.  I'll test this with some
other OS's and compilers and will hopefully update these notes.

I don't currently have a Ubuntu installation, but the following should work.

```
apt-get install build-essential cmake libsdl2-dev libsdl2-mixer-dev
```

Or on arch

```
pacman -S cmake sdl2 sd2_mixer
```

Then, make a build directory, run cmake, and make.  As,
```
mkdir build
cmake ..
make 
```

The binary will be in the `chip8` directory, if it built successfully.

# Usage

Simply run

```
chip8 <rom>
```

There are two roms included in the `res/roms` directory.

# Game Keys

Available keys for games are the following:

```
+-+-+-+-+
|1|2|3|4|
+-+-+-+-+
|Q|W|E|R|
+-+-+-+-+
|A|S|D|F|
+-+-+-+-+
|Z|X|C|V|
+-+-+-+-+
```

# Tests
If you want to run the tests for some reason, you need to include the
googletest submodule and C++17 compatible compiler.  Clone this with the
`--recursive` flag and run cmake with BUILD_WITH_TESTS=On.  I.e.,
```
cmake -DBUILD_WITH_TEST=On ..
```
