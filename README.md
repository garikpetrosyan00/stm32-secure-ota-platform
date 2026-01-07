# STM32 Secure Dual-Slot OTA Platform

## 1. Project Overview

The **STM32 Secure Dual-Slot OTA Platform** is a robust, production-grade firmware architecture designed for reliable Over-The-Air (OTA) updates on embedded systems. It addresses the critical need for fail-safe firmware updates by implementing a dual-bank swapping mechanism with trial boot verification and automatic rollback capabilities.

This platform provides a foundational implementation of a clean, hardware-agnostic bootloader and a sample application, coupled with Linux-based host tooling. It is designed for embedded firmware teams looking to integrate secure update capabilities into their STM32-based products.

## 2. Key Features

*   **Dual-Slot Architecture:** Supports active (Slot A) and update (Slot B) firmware partitions for redundancy.
*   **Fail-Safe Updates:** Implements atomic state transitions to prevent bricking during power loss or update failures.
*   **Trial Boot & Rollback:** Newly installed images enter a "Trial" state. If the application fails to confirm its health (e.g., due to a crash or watchdog reset), the bootloader automatically rolls back to the previous known-good image.
*   **Application-Driven Confirmation:** The application firmware explicitly confirms a successful boot, ensuring that only fully functional updates are committed.
*   **Metadata-Based Validation:** Uses structured image headers and metadata for integrity checks and state management.
*   **Host OTA CLI:** A dedicated Linux command-line tool for packaging firmware and simulating transport.
*   **Modular Design:** Clean separation between the Bootloader, Application, and Common shared logic implementation.

## 3. Architecture Overview

The system is composed of four main components:

1.  **Bootloader (`firmware/bootloader`)**: The immutable first-stage loader responsible for slot management, image verification, and state transitions. It executes at reset.
2.  **Application (`firmware/application`)**: The main business logic firmware. It includes logic to perform self-tests and signal "Boot Confirmation" to the bootloader.
3.  **Common Layer (`firmware/common`)**: Shared libraries defining flash interfaces, image metadata structures, and the boot confirmation API. This ensures ABI compatibility between bootloader and application.
4.  **Host OTA Tool (`host-tools/ota-cli`)**: A Linux CLI utility to wrap raw binary firmware into signed/CRC-checked OTA packages and initiate transfers.

For detailed architectural diagrams and memory maps, please refer to:
*   [Architecture Documentation](docs/architecture.md)
*   [Memory Map](docs/memory-map.md)
*   [Update Flow State Machine](docs/update-flow.md)

## 4. OTA Lifecycle

The update process follows a strict state machine:

1.  **Normal Operation**: System boots from the PROVISIONED or CONFIRMED active slot.
2.  **Download**: The application receives a new update package and writes it to the secondary (Update) slot.
3.  **Swap Request**: The application marks the Update slot as `PENDING`.
4.  **Reboot**: System resets. The Bootloader detects the `PENDING` slot.
5.  **Trial Boot**: The Bootloader validates the new image, updates metadata to `TRIAL`, and jumps to it.
6.  **Verification**:
    *   **Success**: The new application initializes, passes self-tests, and calls `boot_confirm_running_image()`. The state becomes `CONFIRMED`.
    *   **Failure**: If the application crashes or hangs, a watchdog (hardware) resets the system.
7.  **Rollback**: On the next boot, the Bootloader sees an unconfirmed `TRIAL` image (or expired attempt counter) and reverts to the previous valid slot.

## 5. Repository Structure

```
.
├── docs/                   # Design documentation
├── firmware/
│   ├── application/        # Main application source
│   ├── bootloader/         # Bootloader source
│   └── common/             # Shared interfaces and logic
├── host-tools/
│   └── ota-cli/            # Linux Host CLI tool for packaging
├── scripts/                # Utility scripts
└── CMakeLists.txt          # Root build configuration
```

## 6. Build Instructions

The project uses CMake and is designed to be cross-compiled for ARM Cortex-M targets (e.g., using `arm-none-eabi-gcc`). For logic verification, it can also be built for the host (Linux/macOS).

### Prerequisites
*   CMake (3.16+)
*   Build Essentials (make, gcc) or ARM Toolchain

### Building Firmware & Tools
```bash
mkdir build && cd build
cmake ..
make
```

 This will generate:
- `bin/bootloader`: The bootloader executable.
- `bin/application`: The application executable.
- `bin/ota-cli`: The host-side OTA tool.

## 7. Using the OTA CLI

The `ota-cli` tool manages firmware packages.

### Create an OTA Package
Wraps a raw binary with the OTA header.
```bash
./bin/ota-cli pack <firmware.bin> <output.pkg> [version]
# Example:
./bin/ota-cli pack app_v2.bin update_v2.pkg 2
```

### Inspect a Package
Verifies magic bytes and displays header info.
```bash
./bin/ota-cli info update_v2.pkg
```

### Send Update (Simulation)
Simulates sending the package over a serial port.
```bash
./bin/ota-cli send update_v2.pkg /dev/ttyUSB0
```

## 8. Current Limitations

This repository is a **foundational reference implementation**. The following features are intentionally stubbed or omitted for clarity and platform-agnosticism:

*   **No Cryptography**: CRC32 is stubbed; cryptographic signature verification (ECDSA/RSA) is not implemented.
*   **No HAL Integration**: Flash erase/write operations are simulated. Users must implement `flash_if.c` using their specific MCU's HAL (e.g., STM32 HAL/LL).
*   **No Hardware Watchdog**: The rollback logic relies on a reset occurring, but the hardware watchdog configuration is left as a TODO.
*   **No Physical Transport**: The `ota-transport` layer is a logging stub and does not actually drive UART/USB/Network interfaces.

## 9. Future Extensions

*   **Secure Boot**: Integrate MbedTLS or WolfSSL for image signature verification.
*   **Encrypted Images**: Support AES-encrypted firmware images for confidentiality.
*   **Hardware Interface**: Implement concrete Flash and UART drivers for the STM32L4/F4 series.
*   **Network Transport**: Add MQTT/HTTP transport for cloud-connected updates.

## 10. License

This project is released under the **MIT License**.
