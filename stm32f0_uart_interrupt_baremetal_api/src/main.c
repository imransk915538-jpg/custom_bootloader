#include "stm32f0xx.h"
#include "stm32f0xx_gpio_driver.h"
#include "stm32f0xx_rcc_driver.h"
#include "stm32f0xx_usart_driver.h"
#include "stm32f0xx_led_driver.h"

#define RAM_APP_ADDR  0x20000000   // start of SRAM on STM32F051

USART_Handle_t usart1Handle;

void USART1_GPIOInit(void);
void jump_to_application(uint32_t app_addr);

/* ============================================================
   MAIN: POLLING BOOTLOADER
   ============================================================ */
int main(void)
{
    /* -------- 1. Init GPIO + LED ---------- */
    USART1_GPIOInit();
    LED_Init();
    LED_Blink();

    /* -------- 2. Init UART1 (polling mode) ---------- */
    usart1Handle.pUSARTx = USART1;
    usart1Handle.USART_Config.USART_Baud          = 115200;
    usart1Handle.USART_Config.USART_Mode          = 3;     // TX+RX
    usart1Handle.USART_Config.USART_WordLength    = 8;
    usart1Handle.USART_Config.USART_ParityControl = 0;
    usart1Handle.USART_Config.USART_NoOfStopBits  = 1;
    USART_Init(&usart1Handle);

    /* -------- 3. Receive firmware size (4 bytes) ---------- */
    uint32_t fw_size = 0;
    USART_ReceiveData(&usart1Handle, (uint8_t*)&fw_size, 4);

    /* Basic sanity check */
    if (fw_size == 0 || fw_size > 800)
    {
        LED_On();
        while(1);  // invalid size
    }

    /* -------- 4. Receive firmware into RAM ---------- */
    uint8_t *ram = (uint8_t*)RAM_APP_ADDR;
    USART_ReceiveData(&usart1Handle, ram, fw_size);

    LED_Blink();  // indicate done

    /* -------- 5. Jump to application in RAM ---------- */
    jump_to_application(RAM_APP_ADDR);

    while(1);
}


/* ============================================================
   JUMP TO APPLICATION
   ============================================================ */
void jump_to_application(uint32_t app_addr)
{
    /* -------- A. Remap SRAM to 0x00000000 ---------- */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->CFGR1 = (2 << SYSCFG_CFGR1_MEM_MODE_Pos);

    /* -------- B. Load new MSP & PC from RAM vector ---------- */
    uint32_t new_msp = *(uint32_t*)app_addr;
    uint32_t new_pc  = *(uint32_t*)(app_addr + 4);

    /* -------- C. Disable interrupts (recommended) ---------- */
    __disable_irq();

    /* -------- D. Set new stack and jump ---------- */
    __set_MSP(new_msp);
    ((void (*)(void))new_pc)();
}


/* ============================================================
   USART GPIO Init
   ============================================================ */
void USART1_GPIOInit(void)
{
    GPIO_PinConfig_t gpio;

    RCC_PClkControl_GPIO(GPIOA, ENABLE);

    /* PA9 -> TX (AF1) */
    gpio.GPIO_PinNumber = 9;
    gpio.GPIO_PinMode   = 2;
    gpio.GPIO_PinSpeed  = 3;
    gpio.GPIO_PinPuPd   = 0;
    GPIO_Init(GPIOA, &gpio);
    GPIO_SetAltFunction(GPIOA, 9, 1);

    /* PA10 -> RX (AF1) */
    gpio.GPIO_PinNumber = 10;
    gpio.GPIO_PinMode   = 2;
    gpio.GPIO_PinSpeed  = 3;
    gpio.GPIO_PinPuPd   = 0;
    GPIO_Init(GPIOA, &gpio);
    GPIO_SetAltFunction(GPIOA, 10, 1);
}

