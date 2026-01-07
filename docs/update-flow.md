# Secure OTA Update Flow

This document details the state machine and sequence of events for a firmware update.

## State 1: Normal Operation
- **Application** is running in Slot A.
- **Bootloader** is dormant.
- Slot B is either empty or contains an old backup.

## State 2: Update Request & Download
1. **Trigger**: Host tool connects to Application (e.g., via UART).
2. **Handshake**: Host requests "Start Update"; App validates compatibility (Product ID, Hardware ID).
3. **Transfer**:
   - Host sends firmware chunks (e.g., 1KB blocks).
   - App erases Slot B pages as needed.
   - App writes data to Slot B.
4. **Completion**:
   - Host sends "End of Image".
   - App calculates SHA-256 of Slot B content.
   - App compares calculated hash with the hash in the new Image Header.
   - If valid, App writes a "Magic Pending" flag to the **Slot B Trailer**.

## State 3: Reset & Boot verification
1. **Reboot**: App triggers a system reset (`NVIC_SystemReset`).
2. **Bootloader Startup**:
   - Checks **Slot B Trailer**. Finds "Magic Pending".
   - Verifies digital signature (ECDSA/RSA) of Slot B Image.
   - **If Invalid**: Erases Slot B Trailer (clears pending). Boots Slot A.
   - **If Valid**: Starts **Swap Operation**.

## State 4: Swap Operation (Critical Region)
The Bootloader physically exchanges the contents of Slot A and Slot B using a scratchpad sector or swap algorithm.
- **Copy**: Move Block N from Slot B -> Slot A (and vice-versa).
- **Power Fail Safety**: Status is written to flash after every block swap. If power dies, bootloader resumes swap on next boot.

## State 5: Trial Boot
1. Swap is complete. The "New" firmware is now in Slot A.
2. Bootloader sets a "Trial Boot" counter (e.g., 1 attempt).
3. Bootloader jumps to Slot A (New Firmware).

## State 6: Confirmation or Rollback
### Scenario A: Success (Commit)
1. New Application starts up.
2. Performs self-test (Check peripherals, init RAM).
3. **Application Action**: Calls `boot_confirm_running_image()`.
   - Reads metadata.
   - If state is TRIAL, changes it to CONFIRMED.
   - Writes updated metadata to flash.
4. Future boots will see CONFIRMED state and skip watchdog/rollback logic.

### Scenario B: Failure (Rollback)
1. New Application crashes (Watchdog) OR fails self-test OR simply resets without setting "Image OK".
2. System Resets.
3. Bootloader sees "Trial Boot" has expired/failed.
4. Bootloader **Reverts Swap**: Swaps Slot A (bad) back to Slot B, and Slot B (original good) back to Slot A.
5. Bootloader boots original image. System is restored.
