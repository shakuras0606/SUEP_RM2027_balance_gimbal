/*
 * GNU linker script for STM32F407IGHx (RoboMaster C board), GNU Arm Embedded toolchain.
 *
 * Memory layout mirrors the Keil target standard_robot:
 *   IROM 0x08000000 0x00100000 (1024 KiB flash)
 *   IRAM 0x20000000 0x00020000 (128 KiB SRAM1+SRAM2)
 * The CCM RAM at 0x10000000 (64 KiB) is unused, exactly like the Keil
 * auto-generated scatter file, which places all RW+ZI data in IRAM1.
 *
 * Stack (8 KiB) and heap (8 KiB) sizes match the Keil startup file:
 *   Stack_Size EQU 0x2000 / Heap_Size EQU 0x2000
 * The heap is consumed by newlib malloc through _sbrk() in cmake/syscalls.c.
 */

ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);
_Min_Heap_Size = 0x2000;
_Min_Stack_Size = 0x2000;

MEMORY
{
    RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
}

PHDRS
{
    flash PT_LOAD FLAGS(5);
    data PT_LOAD FLAGS(6);
    bss PT_LOAD FLAGS(6);
}

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH :flash

    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)
        KEEP(*(.init))
        KEEP(*(.fini))
        . = ALIGN(4);
        _etext = .;
    } >FLASH :flash

    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH :flash

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH :flash

    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH :flash

    .preinit_array :
    {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array*))
        PROVIDE_HIDDEN(__preinit_array_end = .);
    } >FLASH :flash

    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array*))
        PROVIDE_HIDDEN(__init_array_end = .);
    } >FLASH :flash

    .fini_array :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array*))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } >FLASH :flash

    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >RAM AT> FLASH :data

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >RAM AT> RAM :bss

    ._user_heap_stack (NOLOAD) :
    {
        . = ALIGN(8);
        PROVIDE(end = .);
        PROVIDE(_end = .);
        PROVIDE(__HeapBase = .);
        . = . + _Min_Heap_Size;
        PROVIDE(__HeapLimit = .);
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } >RAM AT> RAM :bss

    .ARM.attributes 0 (INFO) :
    {
        *(.ARM.attributes)
    }
}

ASSERT(ADDR(.isr_vector) == ORIGIN(FLASH), "vector table must start at flash origin")
ASSERT(SIZEOF(.isr_vector) > 0, "vector table is empty")
ASSERT(_estack == ORIGIN(RAM) + LENGTH(RAM), "stack top must be at the end of RAM")
