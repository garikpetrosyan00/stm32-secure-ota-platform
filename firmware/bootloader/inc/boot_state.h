#ifndef BOOT_STATE_H
#define BOOT_STATE_H

typedef enum {
    BL_STATE_INIT = 0,
    BL_STATE_CHECK_SLOTS,
    BL_STATE_SELECT_SLOT,
    BL_STATE_PREPARE_BOOT,
    BL_STATE_BOOT,
    BL_STATE_ROLLBACK,
    BL_STATE_ERROR
} boot_state_t;

#endif /* BOOT_STATE_H */
