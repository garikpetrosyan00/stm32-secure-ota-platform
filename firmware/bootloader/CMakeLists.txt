add_executable(bootloader
    src/main.c
    src/boot_manager.c
)

target_include_directories(bootloader PRIVATE
    inc
)

# Link dependencies
target_link_libraries(bootloader PRIVATE
    common
    platform_stm32
)

# TODO: Set linker script
# target_link_options(bootloader PRIVATE -T${CMAKE_CURRENT_SOURCE_DIR}/linker/stm32_bootloader.ld)
