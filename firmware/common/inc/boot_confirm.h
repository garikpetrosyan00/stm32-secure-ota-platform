#ifndef BOOT_CONFIRM_H
#define BOOT_CONFIRM_H

#include "common_defs.h"

/**
 * @brief Confirms that the currently running image is valid.
 *        This should be called by the application after successfully 
 *        initializing and performing self-checks.
 * 
 *        If the image is in TRIAL state, this moves it to CONFIRMED.
 *        If already CONFIRMED, this does nothing.
 * 
 * @return status_t STATUS_OK on success.
 */
status_t boot_confirm_running_image(void);

#endif /* BOOT_CONFIRM_H */
