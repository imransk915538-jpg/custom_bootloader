#include <stdint.h>

/*-------------------------------------------------------------
 *  Symbols from linker script
 *-------------------------------------------------------------*/
extern uint32_t _etext;   /* End of code (start of .data init values in Flash) */
extern uint32_t _sdata;   /* Start of .data section in RAM */
extern uint32_t _edata;   /* End of .data section in RAM */
extern uint32_t _sbss;    /* Start of .bss section in RAM */
extern uint32_t _ebss;    /* End of .bss section in RAM */
extern uint32_t _estack;  /* Top of stack (from linker script) */

/*-------------------------------------------------------------
 *  Function prototypes
 *-------------------------------------------------------------*/
void Reset_Handler(void);
void Default_Handler(void);

/* Application entry point */
int main(void);

/*-------------------------------------------------------------
 *  Weak aliases for all exception handlers
 *-------------------------------------------------------------*/
void NMI_Handler(void)              __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)              __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)           __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)          __attribute__((weak, alias("Default_Handler")));

/* Peripheral interrupt handlers */
void WWDG_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void PVD_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void RTC_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void RCC_CRS_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void EXTI0_1_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void EXTI2_3_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void EXTI4_15_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void TSC_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA1_CH1_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void DMA1_CH2_3_DMA2_CH1_2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH4_5_6_7_DMA2_CH3_4_5_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void ADC_COMP_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_UP_TRG_COM_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void TIM3_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void TIM6_DAC_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void TIM7_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void TIM14_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void TIM15_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void TIM16_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void TIM17_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void I2C1_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void I2C2_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
void USART3_4_5_6_7_8_IRQHandler(void)__attribute__((weak, alias("Default_Handler")));
void CEC_CAN_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void USB_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));

/*-------------------------------------------------------------
 *  Vector table
 *-------------------------------------------------------------*/
__attribute__((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))(&_estack),      // Initial stack pointer
    Reset_Handler,                   // Reset handler
    NMI_Handler,
    HardFault_Handler,
    0, 0, 0, 0, 0, 0, 0,            // Reserved
    SVC_Handler,
    0, 0,
    PendSV_Handler,
    SysTick_Handler,

    /* External Interrupts (STM32F051) */
    WWDG_IRQHandler,
    PVD_IRQHandler,
    RTC_IRQHandler,
    FLASH_IRQHandler,
    RCC_CRS_IRQHandler,
    EXTI0_1_IRQHandler,
    EXTI2_3_IRQHandler,
    EXTI4_15_IRQHandler,
    TSC_IRQHandler,
    DMA1_CH1_IRQHandler,
    DMA1_CH2_3_DMA2_CH1_2_IRQHandler,
    DMA1_CH4_5_6_7_DMA2_CH3_4_5_IRQHandler,
    ADC_COMP_IRQHandler,
    TIM1_BRK_UP_TRG_COM_IRQHandler,
    TIM1_CC_IRQHandler,
    TIM2_IRQHandler,
    TIM3_IRQHandler,
    TIM6_DAC_IRQHandler,
    TIM7_IRQHandler,
    TIM14_IRQHandler,
    TIM15_IRQHandler,
    TIM16_IRQHandler,
    TIM17_IRQHandler,
    I2C1_IRQHandler,
    I2C2_IRQHandler,
    SPI1_IRQHandler,
    SPI2_IRQHandler,
    USART1_IRQHandler,
    USART2_IRQHandler,
    USART3_4_5_6_7_8_IRQHandler,
    CEC_CAN_IRQHandler,
    USB_IRQHandler
};

/*-------------------------------------------------------------
 *  Reset Handler
 *-------------------------------------------------------------*/
void Reset_Handler(void)
{
    uint32_t *src, *dst;

    /* Copy .data section from Flash to SRAM */
    src = &_etext;
    dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* Zero initialize the .bss section */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    /* Call the application's entry point */
    main();

    /* If main() returns, stay here */
    while (1);
}

/*-------------------------------------------------------------
 *  Default Handler (for unused IRQs)
 *-------------------------------------------------------------*/
void Default_Handler(void)
{
    while (1);
}

