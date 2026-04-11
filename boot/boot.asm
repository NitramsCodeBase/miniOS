BITS 16
ORG 0x7C00

CODE_SEG            equ 0x08
DATA_SEG            equ 0x10
SECTORS_TO_READ     equ 41

BOOT_REQUEST        equ 0x0500
BOOT_ACTIVE         equ 0x0501
OS_ACTIVE           equ 0x0502
BOOT_TIMER_DELAY    equ 0x6111

BOOT_MODE_TEXT      equ 0
BOOT_MODE_GUI       equ 1

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Read requested boot mode
    mov al, [BOOT_REQUEST]
    cmp al, BOOT_MODE_GUI
    je gui_mode

text_mode:
    mov ax, 0x0003
    int 0x10
    mov byte [BOOT_ACTIVE], BOOT_MODE_TEXT
    mov al, [OS_ACTIVE]
    cmp al, 0
    je show_boot_message
    jmp continue_boot   

gui_mode:
    mov si, boot_mini_gui_msg
    jmp .print_boot

.continue_boot:
    mov ax, 0x0013
    int 0x10
    mov byte [BOOT_ACTIVE], BOOT_MODE_GUI
    jmp continue_boot

.print_boot:
    lodsb
    or al, al
    jz .boot_delay
    mov ah, 0x0E
    int 0x10
    jmp .print_boot

.boot_delay:
    call delay_2s
    jmp gui_mode.continue_boot

show_boot_message:
    ; Show boot message
    mov si, boot_msg

.print_boot:
    lodsb
    or al, al
    jz .boot_delay
    mov ah, 0x0E
    int 0x10
    jmp .print_boot

.boot_delay:
    call delay_2s

continue_boot:
    ; Clear request so next reboot defaults unless kernel sets it again
    mov byte [BOOT_REQUEST], BOOT_MODE_TEXT

    ; Load kernel to 0x1000
    mov bx, 0x1000
    mov ah, 0x02
    mov al, SECTORS_TO_READ ; disk sectors to read
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x80
    int 0x13
    jc disk_error

    ; Enable A20
    call enable_a20

    ; Load GDT
    lgdt [gdt_descriptor]

    ; Enable protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump into 32-bit mode
    jmp CODE_SEG:protected_mode

disk_error:
    mov si, disk_error_msg

print_error:
    lodsb
    or al, al
    jz halt
    mov ah, 0x0E
    int 0x10
    jmp print_error

halt:
    cli
.hang:
    hlt
    jmp .hang

enable_a20:
    in al, 0x92
    or al, 00000010b
    out 0x92, al
    ret

delay_2s:
    mov cx, BOOT_TIMER_DELAY
.delay_outer:
    mov dx, BOOT_TIMER_DELAY
.delay_inner:
    dec dx
    jnz .delay_inner
    dec cx
    jnz .delay_outer
    ret

gdt_start:
gdt_null:
    dq 0x0000000000000000

gdt_code:
    dq 0x00CF9A000000FFFF

gdt_data:
    dq 0x00CF92000000FFFF

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

boot_msg            db "Booting miniSHELL ...", 0
boot_mini_gui_msg   db "Booting miniOS ...", 0
disk_error_msg      db "Disk read error!", 0

BITS 32
protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000

    jmp 0x1000

times 510 - ($ - $$) db 0
dw 0xAA55