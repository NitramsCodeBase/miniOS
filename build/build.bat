@echo off
@echo.
@echo building image ...
@echo.

nasm -f bin ../boot/boot.asm -o boot.bin 
nasm -f win32 ../boot/kernel_entry.asm -o kernel_entry.obj 

@REM rem adding source .c files here

gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/kernel.c -o kernel.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/pit.c -o pit.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/io.c -o io.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/shell.c -o shell.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/command_registry.c -o command_registry.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/system.c -o system.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../libs/string.c -o string.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../libs/graphic.c -o graphic.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/apis/graphics_api.c -o graphics_api.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/apis/time.c -o time.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/apis/date.c -o date.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../apps/editor/editor.c -o editor.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/apis/mouse.c -o mouse.o

@REM rem linking... add .o files to the command list

gcc -m32 -nostdlib -Wl,-T,../linker/linker.ld -Wl,-e,_start -o kernel.exe ^
 kernel_entry.obj ^
 kernel.o ^
 pit.o ^
 io.o ^
 shell.o ^
 command_registry.o ^
 string.o ^
 graphic.o ^
 graphics_api.o ^
 system.o ^
 time.o ^
 date.o ^
 editor.o ^
 mouse.o

objcopy -O binary kernel.exe kernel.bin
copy /b boot.bin+kernel.bin miniOS.img 

@echo.
@echo deleting temporary build files...

del *.bin
del kernel_entry.obj
del *.o
del *.exe

if not exist "./miniOS.img" goto :errornotfound

@echo.
@echo success
@echo.

choice /c YN /M "Build is completed. Do you want to start it right now?"

if errorlevel 2 goto :end

@echo starting ...
./run.bat

:errornotfound
@echo.
@echo miniOS.img does not exists maybe compile error?

:end
@echo.
@echo miniOS is still in development and will be continueid in a period of time.