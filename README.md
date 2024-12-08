# Snake Game

A classic Snake game implemented in C++ using SDL2.

## Prerequisites

1. MSYS2 with MinGW-w64
2. SDL2 development libraries

## Installation

1. Install MSYS2:
   - Download and install MSYS2 from https://www.msys2.org/
   - Run MSYS2 and update the package database:
     ```bash
     pacman -Syu
     ```

2. Install MinGW-w64 and SDL2:
   - Open MSYS2 MinGW64 terminal and run:
     ```bash
     pacman -S mingw-w64-x86_64-gcc
     pacman -S mingw-w64-x86_64-SDL2
     ```

3. Setup Environment Variables:
   - Open System Properties > Advanced > Environment Variables
   - Add to System PATH:
     - MSYS2 MinGW64 bin directory (e.g., C:\msys64\mingw64\bin)
   - Apply changes and restart your terminal/IDE

4. Setup:
   - Copy `SDL2.dll` from MinGW64 bin directory to the same directory as your executable
   - Or copy it to `C:\Windows\System32`

## Building

Important: Use `mingw32-make` instead of `make`:
```bash
mingw32-make
```

## Running

```bash
./snake.exe
```

## Controls

- Arrow keys to move
- Space to pause
- ESC to quit 