[BITS 16]
[ORG 0x7C00]

; Bootloader entry point
start:
    cli                     ; Disable interrupts
    xor ax, ax             ; Clear ax
    mov ds, ax             ; Set data segment to 0
    mov es, ax             ; Set extra segment to 0
    mov ss, ax             ; Set stack segment to 0
    mov sp, 0x7C00         ; Set stack pointer

    ; Print message
    mov si, msg
    call print_string

    ; Load GRUB from disk
    mov ah, 0x02           ; Read sectors
    mov al, 0x10           ; Number of sectors to read
    mov ch, 0x00           ; Cylinder 0
    mov cl, 0x02           ; Sector 2
    mov dh, 0x00           ; Head 0
    mov dl, 0x80           ; Drive 0x80 (first hard disk)
    mov bx, 0x1000         ; Load at 0x1000:0x0000
    mov es, bx
    xor bx, bx
    int 0x13               ; BIOS disk read

    jc disk_error          ; Jump if error

    ; Jump to loaded GRUB
    jmp 0x1000:0x0000

disk_error:
    mov si, error_msg
    call print_string
    cli
    hlt

print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

msg db "Loading GRUB...", 0
error_msg db "Disk read error!", 0

; Boot signature
times 510-($-$$) db 0
dw 0xAA55 