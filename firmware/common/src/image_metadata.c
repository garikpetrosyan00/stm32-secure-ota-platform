#include "image_metadata.h"
#include "flash_if.h"
#include <string.h>

void image_metadata_init_header(image_header_t *header) {
    if (header) {
        memset(header, 0, sizeof(image_header_t));
        header->magic = IMAGE_HEADER_MAGIC;
    }
}

status_t image_metadata_read(flash_region_t slot_region, image_header_t *header) {
    if ((slot_region != FLASH_REGION_SLOT_A && slot_region != FLASH_REGION_SLOT_B) || !header) {
        return STATUS_ERROR_GENERIC;
    }

    /* Read header from the start of the slot */
    /* Assuming header is at offset 0 of the slot */
    flash_status_t f_status = flash_read(slot_region, 0, (uint8_t*)header, sizeof(image_header_t));
    
    if (f_status != FLASH_OK) {
        return STATUS_ERROR_GENERIC;
    }

    return STATUS_OK;
}

status_t image_metadata_write(flash_region_t slot_region, const image_header_t *header) {
    if ((slot_region != FLASH_REGION_SLOT_A && slot_region != FLASH_REGION_SLOT_B) || !header) {
        return STATUS_ERROR_GENERIC;
    }

    /* Write header to the start of the slot */
    /* Note: Flash must be erased before writing if not already */
    flash_status_t f_status = flash_write(slot_region, 0, (const uint8_t*)header, sizeof(image_header_t));

    if (f_status != FLASH_OK) {
        return STATUS_ERROR_GENERIC;
    }

    return STATUS_OK;
}

bool image_metadata_verify_header(const image_header_t *header) {
    if (!header) {
        return false;
    }

    if (header->magic != IMAGE_HEADER_MAGIC) {
        return false;
    }

    /* TODO: Add more checks (size sanity, version sanity) */

    return true;
}
