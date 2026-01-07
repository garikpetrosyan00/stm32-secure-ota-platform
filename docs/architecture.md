# System Architecture

## Overview
The STM32 Secure OTA Platform is designed to provide a robust, power-fail-safe mechanism for updating embedded firmware in the field. It employs a "Dual-Slot" architecture where the device can hold two complete firmware images: a primary (active) image and a secondary (update) image.

## Components & Responsibilities

### 1. Bootloader (Immutable Root of Trust)
The bootloader is the first code to execute after a system reset. It resides in the protected `Bootloader` flash region.
- **Responsibilities**:
    - **Hardware Initialization**: Minimal setup (Clocks, Flash interface).
    - **Image Verification**: Validates the cryptographic signature and integrity (CRC) of the firmware in Slot A.
    - **Update Management**: Checks for pending updates in Slot B.
    - **Swap Operation**: If a valid update is present in Slot B, the bootloader swaps the contents of Slot A and Slot B. This ensures that if the new update fails to boot, the system can revert to the previous known-good image.
    - **Boot Launch**: Sets up the vector table and jumps to the application in Slot A.

### 2. Application Firmware
This is the main business logic of the device.
- **Responsibilities**:
    - **Normal Operation**: User features, peripheral control, communication.
    - **Update Reception**: Receives new firmware chunks from the Host OTA Tool (via UART, USB, BLE, etc.).
    - **Update Storage**: Writes received data to Slot B (Secondary Slot).
    - **Update Trigger**: Verifies the download and sets the "Update Pending" flag in the metadata region to request a bootloader action on the next reset.

### 3. Common Utilities
Shared code used by both the Bootloader and Application to ensure consistency.
- **Components**:
    - **Flash abstraction**: Drivers for erasing/programming on-chip flash.
    - **Cryptography**: SHA-256 (hashing), ECDSA/RSA (signature verification), CRC32.
    - **Image Metadata**: Definitions for image headers (magic, version, size) and trailers.

### 4. Host OTA Tool
A CLI-based tool running on a Linux/host machine.
- **Responsibilities**:
    - **Image Preparation**: Signs the compiled binary with a private key.
    - **Protocol Handling**: Communicates with the running application to transfer the signed image.
    - **Status Monitoring**: Queries the device for version info and update status.

## Design Decisions

### Dual-Slot Architecture
We use an "A/B" or "Primary/Secondary" slot design.
- **Why?**:
    - **Redundancy**: Always keeps a valid fallback image.
    - **Rollback**: If the new image crashes (watchdog reset) or fails self-test, the bootloader can detect this and swap back to the old image.
    - **Safety**: Update process happens in the background to the secondary slot. Power loss during download does not brick the device because the active slot is untouched.

### Clean Separation
- **Interface**: The bootloader and application share NO global variables and valid linkage. They interact *only* through:
    1. **Flash Metadata**: Persistent flags in the image header/trailer.
    2. **Shared RAM**: A small `NoInit` RAM section for passing boot reason or quick status (optional).
- **Independence**: The application can be updated without changing the bootloader. The bootloader is generally programmed once at the factory.
