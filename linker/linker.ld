MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM  (rw) : ORIGIN = 0x20000000, LENGTH = 96K
}
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

ENTRY(Reset_Handler);

PHDRS {
    flash PT_LOAD FLAGS(5);
    sram PT_LOAD FLAGS(6);
}

SECTIONS
{
    .vector_table :
    {
        . = ALIGN(4);
        *(.init_sp)
        *(.system_exceptions)
        *(.peripheral_interrupts)
        . = ALIGN(4);
    } > FLASH :flash

    .text : {
        . = ALIGN(4);
        *(.rodata*)
        *(.text*)
        . = ALIGN(4);
    } > FLASH :flash

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > SRAM AT > FLASH :sram
    _sidata = LOADADDR(.data);

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > SRAM :sram

    /DISCARD/ :
    {
        *(.ARM.attributes)
        *(.ARM.exidx*)
        *(.ARM.extab*)
        *(.comment)
    }
}