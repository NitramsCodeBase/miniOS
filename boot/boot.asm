BITS 16
ORG 0x7C00

CODE_SEG            equ 0x08
DATA_SEG            equ 0x10

; ------------------------------------------------------------------
; Update this value from your build based on kernel.bin size:
;   sectors = (kernel_size + 511) / 512
; Example: 34664 bytes -> 68 sectors
; ------------------------------------------------------------------
SECTORS_TO_READ     equ 70

; ------------------------------------------------------------------
; Future-proof loading strategy:
; - load kernel high in memory so it never overwrites the bootloader
; - use BIOS extended disk read (LBA)
; - read in chunks so BIOS per-call limits do not break larger kernels
; ------------------------------------------------------------------
KERNEL_LOAD_SEG     equ 0x1000          ; physical 0x10000
KERNEL_LOAD_OFF     equ 0x0000
KERNEL_LOAD_ADDR    equ 0x10000
LBA_CHUNK_SECTORS   equ 32              ; safe chunk size per BIOS call

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
    mov [boot_drive], dl

    ; Check BIOS extended disk read support
    mov ah, 0x41
    mov bx, 0x55AA
    mov dl, [boot_drive]
    int 0x13
    jc disk_error
    cmp bx, 0xAA55
    jne disk_error

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
    jmp gui_print_boot

gui_continue_boot:
    mov ax, 0x0013
    int 0x10
    mov byte [BOOT_ACTIVE], BOOT_MODE_GUI
    jmp continue_boot

gui_print_boot:
    lodsb
    or al, al
    jz gui_boot_delay
    mov ah, 0x0E
    int 0x10
    jmp gui_print_boot

gui_boot_delay:
    call delay_2s
    jmp gui_continue_boot

show_boot_message:
    mov si, boot_msg

text_print_boot:
    lodsb
    or al, al
    jz text_boot_delay
    mov ah, 0x0E
    int 0x10
    jmp text_print_boot

text_boot_delay:
    call delay_2s

continue_boot:
    mov byte [BOOT_REQUEST], BOOT_MODE_TEXT

    ; Reset disk
    mov ah, 0x00
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    ; Initialize chunked LBA loader state
    mov ax, SECTORS_TO_READ
    mov [sectors_left], ax
    mov ax, KERNEL_LOAD_SEG
    mov [current_segment], ax
    mov dword [current_lba_low], 1       ; kernel starts immediately after boot sector
    mov dword [current_lba_high], 0

.read_loop:
    mov ax, [sectors_left]
    cmp ax, 0
    je kernel_loaded

    cmp ax, LBA_CHUNK_SECTORS
    jbe .use_remaining
    mov ax, LBA_CHUNK_SECTORS

.use_remaining:
    mov [chunk_count], ax

    ; Build Disk Address Packet
    mov byte  [dap_packet_size], 0x10
    mov byte  [dap_reserved], 0x00
    mov ax, [chunk_count]
    mov word  [dap_sector_count], ax
    mov word  [dap_buffer_offset], KERNEL_LOAD_OFF
    mov ax, [current_segment]
    mov word  [dap_buffer_segment], ax
    mov eax, [current_lba_low]
    mov dword [dap_lba_low], eax
    mov eax, [current_lba_high]
    mov dword [dap_lba_high], eax

    ; Extended read (LBA)
    mov si, dap
    mov ah, 0x42
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    ; sectors_left -= chunk_count
    mov ax, [sectors_left]
    sub ax, [chunk_count]
    mov [sectors_left], ax

    ; current_lba += chunk_count
    mov ax, [chunk_count]
    xor dx, dx
    add word [current_lba_low], ax
    adc word [current_lba_low+2], dx
    adc word [current_lba_high], dx
    adc word [current_lba_high+2], dx

    ; current_segment += chunk_count * 512 / 16 = chunk_count * 32 paragraphs
    mov ax, [chunk_count]
    shl ax, 5
    add word [current_segment], ax

    jmp .read_loop

kernel_loaded:
    call enable_a20
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

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

; BIOS Disk Address Packet (16 bytes)
dap:
dap_packet_size     db 0
dap_reserved        db 0
dap_sector_count    dw 0
dap_buffer_offset   dw 0
dap_buffer_segment  dw 0
dap_lba_low         dd 0
dap_lba_high        dd 0

boot_msg            db "starting mini-os ...", 0
boot_mini_gui_msg   db "starting graphical interface ...", 0
disk_error_msg      db "Disk read error!", 0
boot_drive          db 0

sectors_left        dw 0
chunk_count         dw 0
current_segment     dw 0
current_lba_low     dd 0
current_lba_high    dd 0

BITS 32
protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000

    ; Optional debug marker: "PX"
    mov dword [0xB8000], 0x1F581F50

    mov eax, KERNEL_LOAD_ADDR
    jmp eax

times 510 - ($ - $$) db 0
dw 0xAA55
