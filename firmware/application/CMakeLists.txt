add_executable(application
    src/main.c
)

target_include_directories(application PRIVATE
    inc
)

# Link dependencies
target_link_libraries(application PRIVATE
    common
    platform_stm32
)

# TODO: Set linker script
# target_link_options(application PRIVATE -T${CMAKE_CURRENT_SOURCE_DIR}/linker/stm32_app.ld)
