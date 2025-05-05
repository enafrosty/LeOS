ENTRY(_start)

SECTIONS {
    /* Place Multiboot header at the very start of the file */
    .multiboot : {
        *(.multiboot)
    }

    . = 0x100000;

    .text : {
        *(.text)
        *(.rodata*)
    }
    .data : { *(.data) }
    .bss : { *(COMMON) *(.bss) }
    /DISCARD/ : { *(.comment) *(.eh_frame) }
}