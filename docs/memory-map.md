# Memory Map

## Flash Layout
| Region | Start Address | Size | Description |
|---|---|---|---|
| Bootloader | 0x08000000 | 32KB | Bootloader code and data |
| App Slot 1 | 0x08008000 | 480KB | Primary Application Image |
| App Slot 2 | 0x08080000 | 480KB | Secondary/Update Image |
| Storage | 0x080F8000 | 32KB | Persistent Storage / Config |

## RAM Layout
| Region | Start Address | Size | Description |
|---|---|---|---|
| SRAM | 0x20000000 | 128KB | System RAM |
