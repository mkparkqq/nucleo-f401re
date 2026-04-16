 TARGET = firmware

# 툴체인
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SZ      = arm-none-eabi-size

# CPU 설정 (F401RE = Cortex-M4 + FPU)
CPU     = -mcpu=cortex-m4
FPU     = -mfpu=fpv4-sp-d16
FLOAT   = -mfloat-abi=hard
MCU     = $(CPU) -mthumb $(FPU) $(FLOAT)

# 소스 파일
C_SOURCES = \
    src/main.c \
    cmsis-device-f4/Source/Templates/system_stm32f4xx.c \
    stm32f4xx_hal_driver/Src/stm32f4xx_hal.c \
    stm32f4xx_hal_driver/Src/stm32f4xx_hal_uart.c \
    stm32f4xx_hal_driver/Src/stm32f4xx_hal_rcc.c \
    stm32f4xx_hal_driver/Src/stm32f4xx_hal_gpio.c \
    stm32f4xx_hal_driver/Src/stm32f4xx_hal_cortex.c

ASM_SOURCES = startup/startup_stm32f401xe.s

# 인클루드 경로
C_INCLUDES = \
    -Istm32f4xx_hal_driver/Inc \
    -Isrc \
	-Icmsis-device-f4/Include \
	-Icmsis_core/CMSIS/Driver/Include \
	-Icmsis_core/CMSIS/Core/Include


# 컴파일 플래그
CFLAGS  = $(MCU) -DSTM32F401xE -DUSE_HAL_DRIVER
CFLAGS += $(C_INCLUDES)
CFLAGS += -Wall -fdata-sections -ffunction-sections
CFLAGS += -g -gdwarf-2          # GDB 디버그 심볼 포함
CFLAGS += -O0                   # 최적화 끄기 (디버깅용)

# 링커 플래그
LDSCRIPT = linker/STM32F401RETx_FLASH.ld
LDFLAGS  = $(MCU) -specs=nano.specs -T$(LDSCRIPT)
LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(TARGET).map

# 빌드 규칙
OBJECTS  = $(C_SOURCES:.c=.o)
OBJECTS += $(ASM_SOURCES:.s=.o)

all: $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.s
	$(CC) -c $(MCU) $< -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f $(OBJECTS) $(TARGET).elf $(TARGET).bin $(TARGET).map

flash: $(TARGET).bin
	openocd -f interface/stlink.cfg \
	        -f target/stm32f4x.cfg \
	        -c "program $(TARGET).bin 0x08000000 verify reset exit"

.PHONY: all clean flash
