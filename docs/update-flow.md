# Update Flow

## Overview
The update process involves the following steps:

1. **Host Side**:
   - Build new firmware image.
   - Sign image with OTA CLI.
   - Send image via UART/USB to the device.

2. **Device Side (Application)**:
   - Receive firmware chunks.
   - Write to Slot 2 (Secondary Slot).
   - Validate checksum/signature of received image.
   - Set "Update Pending" flag in shared memory/storage.
   - Reboot.

3. **Device Side (Bootloader)**:
   - Detect "Update Pending" flag.
   - Verify Slot 2 signature.
   - If valid, swap/copy Slot 2 to Slot 1.
   - Clear "Update Pending" flag.
   - Boot Slot 1.
