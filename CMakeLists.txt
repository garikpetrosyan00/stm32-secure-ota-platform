cmake_minimum_required(VERSION 3.16)

project(stm32-secure-ota-platform VERSION 0.1)

# Options
option(BUILD_HOST_TOOLS "Build host tools (OTA CLI)" ON)
option(BUILD_FIRMWARE "Build firmware" ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(BUILD_FIRMWARE)
    message(STATUS "Configuring Firmware Build...")
    # Here we would typically set up cross-compilation toolchains
    # For now, we will just add the subdirectory
    add_subdirectory(firmware/common)
    add_subdirectory(firmware/platform)
    add_subdirectory(firmware/bootloader)
    add_subdirectory(firmware/application)
endif()

if(BUILD_HOST_TOOLS)
    message(STATUS "Configuring Host Tools Build...")
    add_subdirectory(host-tools/ota-cli)
endif()
