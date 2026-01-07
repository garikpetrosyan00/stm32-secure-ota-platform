# Memory Map detailing Flash and RAM layout

## Flash Memory Layout (1MB Total)

The flash memory is divided into four main regions: Bootloader, Primary Slot (Active), Secondary Slot (Update), and Persistent Storage.

| Region | Start Address | Size | Access | Description |
|---|---|---|---|---|
| **Bootloader** | `0x08000000` | 32 KB | R/X | Immutable bootloader. Secure entry point. |
| **Slot A (Primary)** | `0x08008000` | 480 KB | R/X | Active Application Firmware. |
| -- Header | `0x08008000` | 1 KB | R | Image Header (Magic, Version, Size, Sig). |
| -- App Code | `0x08008400` | 478 KB | R/X | Main Application executable code. |
| -- Trailer | `0x0807FC00` | 1 KB | R/W | Swap status / Image Trailer. |
| **Slot B (Secondary)** | `0x08080000` | 480 KB | R/W | Update Candidate / Backup. |
| -- Header | `0x08080000` | 1 KB | R/W | Image Header for update candidate. |
| -- App Code | `0x08080400` | 478 KB | R/W | Update payload. |
| -- Trailer | `0x080F7C00` | 1 KB | R/W | Swap status / Image Trailer. |
| **Storage** | `0x080F8000` | 32 KB | R/W | Key-Value Store, Config (Filesystem/NVS). |

### ASCII Diagram

```text
+---------------------+ 0x08100000 (End of Flash)
|      Storage        | 32KB
+---------------------+ 0x080F8000
|                     |
|      Slot B         | 480KB
|   (Update/Backup)   |
|                     |
+---------------------+ 0x08080000
|                     |
|      Slot A         | 480KB
|     (Active)        |
|                     |
+---------------------+ 0x08008000
|     Bootloader      | 32KB
+---------------------+ 0x08000000
```

## RAM Layout

| Region | Start Address | Size | Description |
|---|---|---|---|
| **SRAM** | `0x20000000` | 128 KB | System Random Access Memory |
| -- NoInit | `0x20000000` | 4 KB | Shared RAM / Boot Info (Preserved across soft reset) |
| -- Boot/App RAM | `0x20001000` | 124 KB | Stack + Heap + BSS + Data |

## Metadata Definitions

### Image Header (1KB)
Found at the beginning of Slot A and Slot B.
- **Magic Number**: Validity marker.
- **Version**: `Major.Minor.Patch`.
- **Image Size**: Length of the firmware binary.
- **CRC/Hash**: Integrity check.
- **Signature**: Security verification.

### Image Trailer (1KB)
Found at the end of Slot A and Slot B.
- **Swap Status**: Indicates if a swap is requested, in-progress, or completed.
- **Copy Status**: Tracks progress of sector swapping (for power-fail recovery).
