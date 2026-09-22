#include <stdint.h>

#define AHB1_BASE 0x40020000UL

#define PORT_MODE_OFFSET 0x0UL
#define OUTPUT_DATA_OFFSET 0X14UL
#define RCC_OFFSET 0x3800UL
#define AHB1_CLOCK_ENABLE_OFFSET 0X30UL

#define PORT_MODE_R (*(volatile uint32_t *)(AHB1_BASE + PORT_MODE_OFFSET))
#define OUTPUT_DATA_R (*(volatile uint32_t *)(AHB1_BASE + OUTPUT_DATA_OFFSET))
#define AHB1_CLOCK_ENABLE_R (*(volatile uint32_t *)(AHB1_BASE + RCC_OFFSET + AHB1_CLOCK_ENABLE_OFFSET))

int main(void) {
  // Enable access to the clock on AHB1 for GPIOA
  AHB1_CLOCK_ENABLE_R |= 1U<<0;

  // According to the spec output mode for pin 5 should be set to 01 in bit 11 and 10
  PORT_MODE_R |= 1U<<10;
  PORT_MODE_R &= ~(1U<<11);
  
  while(1) {
    // Set PA5 data to high
    OUTPUT_DATA_R |= 1U<<5;
    for(volatile int i = 0; i < 300000;i++) {}
    // Set PA5 data to low
    OUTPUT_DATA_R &= ~(1U<<5);
    for(volatile int i = 0; i < 300000;i++) {}
  }
  return 0;
}

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
  // memset .bss to zero, and copy .data section to RAM region
  extern long _sbss, _ebss, _sdata, _edata, _sidata;  // linker symbols from link.ld
  for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
  for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

  main();             // Call main()
  for (;;) (void) 0;  // Infinite loop in the case if main() returns
}

extern void _estack(void);  // Defined in link.ld

// Vector table: 16 Arm system exceptions + 97 STM32F446 peripheral interrupts
// (positions 0..96, last entry at offset 0x1C0). Placed in .vectors by link.ld.
__attribute__((section(".vectors"))) void (*const tab[16 + 97])(void) = {
    _estack, _reset};
