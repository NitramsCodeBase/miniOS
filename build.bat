@echo off
@echo.
@echo building image ...
del kernel_entry.obj
del kernel.o
del kernel.exe
del kernel.bin
del os-image.bin
nasm -f win32 kernel_entry.asm -o kernel_entry.obj >nul
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c kernel.c -o kernel.o >nul
gcc -m32 -nostdlib -Wl,-T,linker.ld -Wl,-e,_start -o kernel.exe kernel_entry.obj kernel.o >nul
objcopy -O binary kernel.exe kernel.bin >nul
copy /b boot.bin+kernel.bin os-image.bin >nul
@echo.
@echo os-image.bin is successfully created.
