#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include "common_defs.h"

/* TODO: Define bootloader state machine states */
typedef enum {
    BOOT_STATE_INIT,
    BOOT_STATE_CHECK_SLOTS,
    BOOT_STATE_UPDATE,
    BOOT_STATE_JUMP_TO_APP,
    BOOT_STATE_ERROR
} boot_state_t;

void bootloader_run(void);

#endif /* BOOTLOADER_H */
