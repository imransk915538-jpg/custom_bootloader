/* stm32f051r8t6.ld */
ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 8K
}

SECTIONS
{
  .text :
  {
    KEEP(*(.isr_vector))   /* Interrupt vector table */
    *(.text*)              /* Code */
    *(.rodata*)            /* Read-only data */
    _etext = .;            /* End of text section */
  } > FLASH

  .data : AT(_etext)
  {
    _sdata = .;
    *(.data*)
    _edata = .;
  } > RAM

  .bss :
  {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > RAM

  .stack (COPY):
  {
    . = ALIGN(8);
    _estack = ORIGIN(RAM) + LENGTH(RAM); /* Top of stack */
  } > RAM
}

