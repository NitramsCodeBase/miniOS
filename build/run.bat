@echo off
@echo.
@echo starting miniOS ...

if not exist "./miniOS.img" goto :errornotfound

qemu-system-i386 ^
 -drive format=raw,file=miniOS.img ^
 -display gtk,full-screen=on

 goto :end

 :errornotfound

@echo.
@echo error: could not start miniOS, no such file or directory.
@echo.

 :end