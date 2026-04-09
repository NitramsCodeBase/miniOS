@echo off
@echo.
@echo starting miniOS ...

qemu-system-i386 ^
 -drive format=raw,file=miniOS.img ^
 -display gtk,full-screen=on