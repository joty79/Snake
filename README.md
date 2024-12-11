# Snake Game

A classic Snake game implementation using SDL2.

## Building and Running

### Prerequisites
- MSYS2 with MinGW-w64
- SDL2 development libraries
- C++ compiler (g++)

### Build Instructions

#### Using MSYS2 UCRT64 Terminal

## Prerequisites

1. MSYS2 with UCRT64 (recommended) or MinGW-w64
2. SDL2 development libraries
3. GNU Make

## Installation

1. Install MSYS2:
   - Download and install MSYS2 from https://www.msys2.org/
   - Run MSYS2 and update the package database:
     ```bash
     pacman -Syu
     ```
   - Close MSYS2 after the update

2. Install Required Packages:
   - Open MSYS2 UCRT64 terminal and run:
     ```bash
     # Update package database again
     pacman -Syu

     # Install development tools
     pacman -S mingw-w64-ucrt-x86_64-toolchain
     pacman -S make

     # Install SDL2
     pacman -S mingw-w64-ucrt-x86_64-SDL2
     ```

3. Setup Environment Variables:
   - Open System Properties > Advanced > Environment Variables
   - Add to System PATH:
     - MSYS2 UCRT64 bin directory (e.g., C:\msys64\ucrt64\bin)
   - Apply changes and restart your terminal/IDE

4. Setup SDL2:
   - Copy `SDL2.dll` from UCRT64 bin directory (e.g., C:\msys64\ucrt64\bin\SDL2.dll)
   - Place it in the same directory as your executable
   - Or copy it to `C:\Windows\System32` (system-wide installation)

## Building

1. You can build the game in three ways:

   #### Using MSYS2 UCRT64 Terminal:
   ```bash
   make clean && make
   ```
   or using the shell script:
   ```bash
   ./run.sh
   ```
   (make sure to give execute permission first with: chmod +x run.sh)

   #### Using Windows Terminal (CMD/PowerShell):
   ```bash
   mingw32-make clean && mingw32-make
   ```
   or simply:
   ```bash
   make clean
   ```
   (using the provided make.bat)

2. If you get any errors:
   - Make sure you're using the correct terminal and commands
   - Check if all paths in Makefile match your MSYS2 installation
   - Verify SDL2 is properly installed

Note: Even though you're on Windows, you can use the Unix-style shell script (run.sh) 
in the MSYS2 UCRT64 terminal because it provides a Unix-like environment.

## Running

```bash
./snake.exe
```

## Game Controls

- Arrow keys: Control snake direction
- Space: Pause/Resume game
- ESC: Quit game

## Troubleshooting

1. If SDL2.dll is missing:
   - Check if SDL2.dll is in the same directory as snake.exe
   - Or install it system-wide in C:\Windows\System32

2. If compilation fails:
   - Verify you're using UCRT64 terminal (not MINGW64)
   - Check if all development tools are installed
   - Update MSYS2 and all packages

3. Common Issues:
   - "make: command not found" → Install make package
   - "SDL2/SDL.h not found" → Install SDL2 development package
   - Linker errors → Check SDL2 paths in Makefile

## Project Structure

- `snake.cpp`: Main game implementation
- `snake.h`: Game declarations and structures
- `glyphs.hpp`: Font rendering utilities
- `Makefile`: Build configuration
- `SDL2.dll`: SDL2 runtime library 