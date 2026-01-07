#ifndef BOOT_MANAGER_H
#define BOOT_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize the boot manager logic.
 */
void boot_manager_init(void);

/**
 * @brief Run the main bootloader flow.
 *        This function contains the state machine and may not return
 *        if a successful boot happens.
 */
void boot_manager_run(void);

#endif /* BOOT_MANAGER_H */
