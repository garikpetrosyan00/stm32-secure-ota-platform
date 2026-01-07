#include "boot_confirm.h"
#include "image_metadata.h"
#include "flash_if.h"

status_t boot_confirm_running_image(void) {
    /* 
     * In a real system, we need to know which slot we are running from.
     * This info is usually passed from bootloader via shared RAM, registers,
     * or by checking vector table location (SCB->VTOR).
     */
     
    /* TODO: Determine active slot dynamically. 
       For this skeleton, we assume we are running from Slot A 
       (since that's where we jump to).
    */
    flash_region_t active_slot = FLASH_REGION_SLOT_A;

    image_header_t header;
    if (image_metadata_read(active_slot, &header) != STATUS_OK) {
        return STATUS_ERROR_GENERIC;
    }

    if (header.state == IMAGE_STATE_CONFIRMED) {
        /* Already confirmed, nothing to do */
        return STATUS_OK;
    }

    /* Mark confirmed */
    return image_metadata_mark_confirmed(active_slot);
}
