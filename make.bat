@echo off
IF "%1"=="clean" (
    mingw32-make clean && mingw32-make
) ELSE (
    mingw32-make %*
) 