# nucleo-f401re 프로젝트

STM32F401RE Nucleo 보드용 베어메탈(bare-metal) 임베디드 펌웨어 프로젝트입니다.
ARM GCC 툴체인과 STM32 HAL 라이브러리를 사용하며, UART로 "Hello from STM32!"를 
1초마다 전송하는 동작을 수행합니다.

---

## 디렉토리 구조 및 역할

```
nucleo-f401re/
├── src/                        # 애플리케이션 소스코드
├── startup/                    # ARM CPU 초기화 코드 (어셈블리)
├── linker/                     # 링커 스크립트 (메모리 레이아웃 정의)
├── Drivers/                    # 사용자 정의 드라이버 (현재 비어 있음)
├── cmsis_core/                 # ARM CMSIS-Core 헤더 (git submodule)
├── cmsis-device-f4/            # STM32F4 디바이스 정의 (git submodule)
├── stm32f4xx_hal_driver/       # STM32 HAL 드라이버 라이브러리 (git submodule)
├── STM32CubeF4/                # STM32CubeF4 전체 SDK 패키지 (git submodule)
├── Makefile                    # 빌드 설정
├── firmware.elf                # 빌드 결과물 - 디버깅용 실행 파일
├── firmware.bin                # 빌드 결과물 - 플래싱용 바이너리 이미지
└── firmware.map                # 빌드 결과물 - 링커 맵 파일
```

---

## 각 디렉토리 상세 설명

### `src/` — 애플리케이션 소스코드
실제 펌웨어 로직이 작성되는 곳입니다.

- `main.c` — 시스템 클럭 설정(84 MHz), USART2 초기화(115200 bps), 1초 주기 UART 
  전송 루프
- `stm32f4xx_hal_conf.h` — 사용할 HAL 모듈(RCC, GPIO, UART, DMA, Flash, PWR 등)
  을 활성화하는 설정 헤더

### `startup/` — ARM 스타트업 코드
MCU 전원 인가 또는 리셋 직후, C 런타임 환경이 구성되기 전에 실행되는 어셈블리 
코드입니다.

- `startup_stm32f401xe.s` — 인터럽트 벡터 테이블 정의, 스택 초기화, 
  `.data`/`.bss` 섹션 초기화, `main()` 호출 진입

### `linker/` — 링커 스크립트
컴파일된 코드와 데이터가 MCU 메모리의 어느 주소에 배치될지를 정의합니다.

- `STM32F401RETx_FLASH.ld` — Flash 512KB (`0x08000000`), RAM 96KB (`0x20000000`) 
  배치 정의 `.isr_vector`, `.text`, `.data`, `.bss` 섹션의 주소 및 순서를 지정함

### `cmsis_core/` — ARM CMSIS-Core 헤더 (git submodule)
ARM이 정의한 Cortex-M 프로세서 공통 추상화 계층입니다.

- `core_cm4.h` — Cortex-M4 레지스터, NVIC, SysTick, FPU 접근 API 제공
- 컴파일러/아키텍처에 독립적인 이식성 있는 코드 작성 기반 제공

### `cmsis-device-f4/` — STM32F4 디바이스 정의 (git submodule)
STM32F4 시리즈 MCU의 특정 레지스터 맵과 초기화 코드를 제공합니다.

- `Include/stm32f401xe.h` — 모든 주변장치 레지스터 주소 및 비트 정의
- `Include/system_stm32f4xx.h` / `Source/Templates/system_stm32f4xx.c` — 시스템 
  클럭 초기화 함수(`SystemInit()`) 구현

### `stm32f4xx_hal_driver/` — STM32 HAL 드라이버 (git submodule)
STMicroelectronics가 제공하는 하드웨어 추상화 계층(HAL) 라이브러리입니다.

- `Inc/` — 100개 이상의 주변장치 드라이버 헤더 (GPIO, UART, RCC, DMA, ADC 등)
- `Src/` — 각 주변장치의 HAL 구현체
- 실제 빌드에 포함되는 모듈: `stm32f4xx_hal.c`, `stm32f4xx_hal_rcc.c`, 
  `stm32f4xx_hal_gpio.c`, `stm32f4xx_hal_uart.c`, `stm32f4xx_hal_dma.c`, 
  `stm32f4xx_hal_cortex.c`

### `STM32CubeF4/` — STM32CubeF4 전체 SDK (git submodule)
ST의 공식 펌웨어 패키지로, 참고용 예제 및 미들웨어가 포함된 대형 리포지토리입니다.

- `Drivers/` — 추가 드라이버 및 BSP
- `Middlewares/` — USB, FatFS, FreeRTOS, CMSIS-RTOS 등
- `Projects/` — 다양한 보드용 참고 프로젝트
- 직접 컴파일에 포함되지는 않으며, 참고 및 코드 재사용 목적으로 유지

### `Drivers/` — 사용자 정의 드라이버
현재 비어 있습니다. 향후 보드 전용 드라이버(센서, 모터 등)를 여기에 추가합니다.

---

## 빌드 결과물 파일 설명

### `firmware.elf` — ELF 실행 파일 (디버깅용)
- **ELF(Executable and Linkable Format)** 형식의 완전한 실행 파일
- 컴파일된 코드, 초기화 데이터, **심볼 테이블**, **DWARF 디버그 정보** 포함
- GDB + OpenOCD를 통한 소스 레벨 디버깅(`break main.c:42` 등)에 사용
- 크기가 크고(~89 KB) MCU에 직접 플래싱하지 않음

### `firmware.bin` — 바이너리 이미지 (플래싱용)
- ELF에서 심볼/디버그 정보를 제거한 **순수 기계어 바이너리**
- `.text`(코드), `.data`(초기화 데이터) 섹션만 포함
- MCU Flash 메모리 주소 `0x08000000`에 그대로 기록(write)되는 파일
- 크기가 작음(~6.8 KB), `make flash` 명령으로 ST-Link를 통해 보드에 전송

### `firmware.map` — 링커 맵 파일 (분석용)
- 링커가 생성하는 사람이 읽을 수 있는 **메모리 배치 보고서**
- 모든 심볼의 주소, 각 오브젝트 파일이 차지하는 메모리 크기, 섹션 배치 정보 포함
- 크래시 주소(`HardFault` 발생 PC 값 등)를 소스코드 위치로 역추적할 때 활용
- 링커 가비지 컬렉션(`--gc-sections`)으로 제거된 섹션 목록도 확인 가능

---

## 빌드 시스템

**툴체인:** `arm-none-eabi-gcc` (ARM 베어메탈 GCC)

| 명령어 | 동작 |
|--------|------|
| `make all` | `firmware.elf` 및 `firmware.bin` 생성 |
| `make clean` | 빌드 결과물 전체 삭제 |
| `make flash` | OpenOCD + ST-Link로 보드에 펌웨어 플래싱 |

**주요 컴파일 플래그:**
- `-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard` — Cortex-M4 하드웨어 FPU 사용
- `-DSTM32F401xE -DUSE_HAL_DRIVER` — 전처리기 매크로
- `-O0 -g -gdwarf-2` — 최적화 없음, GDB 디버그 심볼 포함
- `-ffunction-sections -fdata-sections -Wl,--gc-sections` — 미사용 코드 제거

---

## 보드 연결 및 디버깅

### 보드 연결 확인

```bash
# ST-Link 프로브 감지 여부 확인 (가장 빠름)
st-info --probe

# OpenOCD로 연결 테스트 (연결되면 GDB 서버 포트 3333 오픈 후 Ctrl+C로 종료)
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg
```

정상 연결 시 `st-info --probe` 출력 예시:
```
Found 1 stlink programmers
  version:    V2J45
  serial:     ...
  flash:      524288 (pagesize: 16384)
  sram:       98304
  chipid:     0x0423
  descr:      F4xx
```

### 펌웨어 빌드 및 플래싱

```bash
# 빌드 + 플래싱 한 번에
make flash

# 빌드만
make all

# 이미 빌드된 firmware.bin을 수동으로 플래싱 (st-flash 직접 사용)
st-flash write firmware.bin 0x08000000

# 플래시 후 리셋 (재실행)
st-flash reset
```

`make flash`는 내부적으로 아래 OpenOCD 명령을 실행합니다:
```bash
openocd -f interface/stlink.cfg \
        -f target/stm32f4x.cfg \
        -c "program firmware.bin 0x08000000 verify reset exit"
```

**`make flash` 동작 원리:**

```
[Mac]                [ST-Link]              [STM32F401RE]
  │                     │                        │
  │──── USB ───────────>│                        │
  │   (OpenOCD가        │──── SWD ─────────────> │
  │    ST-Link에        │  (SWDCLK/SWDIO         │
  │    명령 전달)       │   2선 디버그 프로토콜) │
```

1. **`interface/stlink.cfg`** — OpenOCD가 USB로 Nucleo 보드 내장 ST-Link 칩에 연결
2. **`target/stm32f4x.cfg`** — ST-Link가 SWD(Serial Wire Debug) 2선 프로토콜로 
  STM32 MCU의 디버그 포트에 연결. CPU를 halt 상태로 전환
3. **`program firmware.bin 0x08000000`** — `firmware.bin`의 내용을 MCU 내부 
  Flash 메모리 시작 주소 `0x08000000`부터 페이지 단위로 erase → write
4. **`verify`** — 기록된 Flash 내용을 다시 읽어 `firmware.bin`과 비교, 불일치 시 오류
5. **`reset`** — MCU 시스템 리셋 트리거 → `startup_stm32f401xe.s`의 Reset_Handler부터 실행
6. **`exit`** — OpenOCD 종료, SWD/USB 연결 해제

플래싱 완료 후 자동으로 보드를 리셋하여 펌웨어가 즉시 실행됩니다.

### GDB 디버깅

터미널을 두 개 열어 진행합니다.

**터미널 1 — OpenOCD GDB 서버 실행:**
```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg
# 포트 3333(GDB), 4444(telnet) 오픈 후 대기
```

**터미널 2 — GDB 연결:**
```bash
arm-none-eabi-gdb firmware.elf

# GDB 프롬프트에서:
(gdb) target remote :3333    # OpenOCD에 연결
(gdb) monitor reset halt     # 보드 리셋 후 정지
(gdb) load                   # firmware.elf 플래싱
(gdb) break main             # main()에 브레이크포인트 설정
(gdb) continue               # 실행 재개
```

**자주 쓰는 GDB 명령어:**
```
break <함수명 또는 파일:줄번호>   브레이크포인트 설정
info break                        브레이크포인트 목록
delete <번호>                     브레이크포인트 삭제
next (n)                          다음 줄 실행 (함수 진입 안 함)
step (s)                          다음 줄 실행 (함수 진입)
continue (c)                      다음 브레이크포인트까지 실행
print <변수명>                    변수 값 출력
info registers                    레지스터 값 출력
x/10x <주소>                      메모리 덤프 (16진수, 10개)
monitor reset halt                보드 리셋 후 정지
quit                              GDB 종료
```

### 시리얼 출력 확인 (UART)

펌웨어가 USART2로 출력하는 문자열은 ST-Link 내장 USB-UART 브릿지를 통해 호스트에서 확인할 수 있습니다.

**시리얼 포트:** `/dev/cu.usbmodem11203` (연결 시마다 번호가 달라질 수 있음)

```bash
# screen 사용 (macOS 기본 포함)
screen /dev/cu.usbmodem11203 115200

# 종료: Ctrl+A 입력 후 K 입력, 확인 프롬프트에서 Y

# 포트 이름이 바뀌었을 때 재탐색
ls /dev/cu.usbmodem*
```

정상 동작 시 1초마다 아래 문자열이 출력됩니다:
```
Hello from STM32!
Hello from STM32!
...
```

---

## 하드웨어 정보

- **MCU:** STM32F401RE (ARM Cortex-M4 @ 84 MHz, 하드웨어 FPU 탑재)
- **Flash:** 512 KB (`0x08000000`)
- **RAM:** 96 KB (`0x20000000`)
- **시스템 클럭:** HSI 16 MHz → PLL → 84 MHz (PLLM=16, PLLN=336, PLLP=4)
- **UART:** USART2, 115200 bps, 8N1 (ST-Link USB-to-UART 브릿지 경유)
