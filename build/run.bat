@echo off
@echo.
@echo starting mini-os ...

if not exist "./mini.os" goto :errornotfound

qemu-system-i386 ^
 -drive format=raw,file=mini.os ^
 -display gtk,full-screen=on

 goto :end

 :errornotfound

@echo.
@echo error: could not start mini-os, no such file or directory.
@echo.

 :end