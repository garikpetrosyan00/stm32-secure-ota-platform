#!/bin/bash

# TODO: invoke STM32 Cube Programmer or OpenOCD
echo "Flashing firmware..."
# openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program build/firmware/bootloader/bootloader verify reset exit"
