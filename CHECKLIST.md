# Snake Game - Installation Checklist

## Required Files Check
- [x] `snake.cpp` - Main game implementation
- [x] `snake.h` - Game declarations and structures
- [x] `glyphs.hpp` - Font rendering utilities
- [x] `Makefile` - Build configuration (UCRT64 compatible)
- [x] `SDL2.dll` - SDL2 runtime library
- [x] `README.md` - Installation and setup instructions

## README.md Content Check
- [x] Prerequisites section
- [x] MSYS2/UCRT64 installation steps
- [x] SDL2 setup instructions
- [x] Build instructions
- [x] Troubleshooting guide
- [x] Game controls
- [x] Project structure

## Makefile Configuration Check
- [x] UCRT64 paths configured
- [x] Unix-style commands (rm instead of del)
- [x] Correct compiler flags
- [x] Required libraries linked
- [x] Clean target properly set

## New Installation Steps
1. System Setup
   - [ ] Install MSYS2
   - [ ] Update system packages
   - [ ] Install UCRT64 toolchain
   - [ ] Install SDL2 development package
   - [ ] Install make utility

2. Environment Configuration
   - [ ] Add UCRT64 bin to PATH
   - [ ] Verify gcc/g++ installation
   - [ ] Verify make installation
   - [ ] Check SDL2 installation

3. Project Setup
   - [ ] Copy all files to project directory
   - [ ] Update Makefile paths if needed
   - [ ] Copy SDL2.dll to correct location
   - [ ] Test build with `make clean && make`
   - [ ] Test run with `./snake.exe`

## Common Issues Checklist
- [ ] MSYS2 UCRT64 terminal being used (not MINGW64)
- [ ] All development tools installed
- [ ] PATH environment variable set correctly
- [ ] SDL2.dll in correct location
- [ ] Makefile paths match installation
- [ ] No compilation errors
- [ ] No linker errors
- [ ] Game runs without runtime errors

## Final Verification
- [ ] Clean build successful
- [ ] Game starts properly
- [ ] Controls working
- [ ] Score display working
- [ ] No graphical glitches
- [ ] Exit game working

Note: Check (✓) each box as you complete the steps during installation on a new system. 