# System Architecture

## Overview
The STM32 Secure OTA Platform consists of a dual-slot bootloader and an application firmware, separated by a clean interface.

## Components
1. **Bootloader**: Responsible for verifying and booting the application. Handles OTA updates.
2. **Application**: The main business logic firmware.
3. **Common**: Shared utilities and definitions used by both bootloader and application.
4. **Platform**: Hardware abstraction layer for STM32.
5. **OTA CLI**: Host-side tool to manage firmware updates.

## Boot Process
1. Reset
2. Bootloader startup
3. Verify Application Slot 1 (Primary)
4. If valid, jump to App
5. If invalid, check Slot 2 (Secondary/Update)
6. If update pending, swap/copy slots
