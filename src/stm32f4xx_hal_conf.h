#ifndef __STM32F4xx_HAL_CONF_H
#define __STM32F4xx_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* ############ 사용할 HAL 모듈 선택 ############ */
/* 지금은 UART만 쓰니까 필요한 것만 활성화       */

#define HAL_MODULE_ENABLED        /* 반드시 켜야 함 */
#define HAL_CORTEX_MODULE_ENABLED /* NVIC, SysTick  */
#define HAL_DMA_MODULE_ENABLED    /* UART가 내부적으로 필요 */
#define HAL_FLASH_MODULE_ENABLED  /* 기본 동작에 필요 */
#define HAL_GPIO_MODULE_ENABLED   /* 핀 제어 */
#define HAL_PWR_MODULE_ENABLED    /* 클럭 설정에 필요 */
#define HAL_RCC_MODULE_ENABLED    /* 클럭 설정 */
#define HAL_UART_MODULE_ENABLED   /* UART 출력 */

/* ############ HSE/HSI 클럭 설정 ############ */
/* Nucleo F401RE는 외부 크리스탈 없이 HSI(내부) 사용 */
#define HSE_VALUE    8000000U      /* 외부 크리스탈 없으면 안 쓰임 */
#define HSE_STARTUP_TIMEOUT  100U
#define HSI_VALUE    16000000U     /* 내부 오실레이터 16MHz */
#define LSI_VALUE    32000U
#define LSE_VALUE    32768U
#define LSE_STARTUP_TIMEOUT  5000U
#define EXTERNAL_CLOCK_VALUE  12288000U

/* ############ SysTick 설정 ############ */
#define TICK_INT_PRIORITY   0x0FU  /* HAL_Delay() 우선순위 */

/* ############ 이더넷 등 안 쓰는 기능 ############ */
#define USE_RTOS                     0U
#define USE_HAL_ADC_REGISTER_CALLBACKS    0U
#define USE_HAL_CAN_REGISTER_CALLBACKS    0U
#define USE_HAL_CEC_REGISTER_CALLBACKS    0U
#define USE_HAL_DAC_REGISTER_CALLBACKS    0U
#define USE_HAL_ETH_REGISTER_CALLBACKS    0U
#define USE_HAL_I2C_REGISTER_CALLBACKS    0U
#define USE_HAL_I2S_REGISTER_CALLBACKS    0U
#define USE_HAL_MMC_REGISTER_CALLBACKS    0U
#define USE_HAL_NAND_REGISTER_CALLBACKS   0U
#define USE_HAL_NOR_REGISTER_CALLBACKS    0U
#define USE_HAL_PCCARD_REGISTER_CALLBACKS 0U
#define USE_HAL_PCD_REGISTER_CALLBACKS    0U
#define USE_HAL_QSPI_REGISTER_CALLBACKS   0U
#define USE_HAL_RTC_REGISTER_CALLBACKS    0U
#define USE_HAL_SAI_REGISTER_CALLBACKS    0U
#define USE_HAL_SD_REGISTER_CALLBACKS     0U
#define USE_HAL_SMARTCARD_REGISTER_CALLBACKS 0U
#define USE_HAL_SPI_REGISTER_CALLBACKS    0U
#define USE_HAL_TIM_REGISTER_CALLBACKS    0U
#define USE_HAL_UART_REGISTER_CALLBACKS   0U
#define USE_HAL_USART_REGISTER_CALLBACKS  0U
#define USE_HAL_WWDG_REGISTER_CALLBACKS   0U

/* ############ 각 드라이버 헤더 include ############ */
#ifdef HAL_RCC_MODULE_ENABLED
  #include "stm32f4xx_hal_rcc.h"
#endif
#ifdef HAL_GPIO_MODULE_ENABLED
  #include "stm32f4xx_hal_gpio.h"
#endif
#ifdef HAL_DMA_MODULE_ENABLED
  #include "stm32f4xx_hal_dma.h"
#endif
#ifdef HAL_CORTEX_MODULE_ENABLED
  #include "stm32f4xx_hal_cortex.h"
#endif
#ifdef HAL_UART_MODULE_ENABLED
  #include "stm32f4xx_hal_uart.h"
#endif
#ifdef HAL_FLASH_MODULE_ENABLED
  #include "stm32f4xx_hal_flash.h"
#endif
#ifdef HAL_PWR_MODULE_ENABLED
  #include "stm32f4xx_hal_pwr.h"
#endif

#ifdef __cplusplus
}
#endif

#define assert_param(expr) ((void)0U)

#endif /* __STM32F4xx_HAL_CONF_H */
