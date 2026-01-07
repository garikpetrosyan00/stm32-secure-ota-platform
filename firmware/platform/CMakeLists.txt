add_library(platform_stm32 STATIC
    stm32/platform_init.c
)

target_include_directories(platform_stm32 PUBLIC
    stm32
)

# Link against actual STM32 HAL/LL or CMSIS in the future
