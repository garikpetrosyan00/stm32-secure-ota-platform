#include "image_metadata.h"
#include "flash_if.h"
#include "crc32.h"
#include <string.h> /* For memset/memcpy */

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

bool image_metadata_verify_header(flash_region_t region, const image_header_t *header) {
    if (!header) {
        return false;
    }
    
    if (header->magic != IMAGE_HEADER_MAGIC) {
        return false;
    }
    
    if (header->image_size == 0 || header->image_size > (512 * 1024)) { /* Sanity max size, e.g., 512KB */
        return false;
    }

    /* Verify CRC of the image payload */
    uint32_t calculated_crc = 0xFFFFFFFF; /* Standard seed */
    uint8_t buffer[256]; /* Small buffer for reading chunks */
    uint32_t bytes_processed = 0;
    uint32_t image_offset = IMAGE_HEADER_SIZE; /* Image payload starts after the header */

    while (bytes_processed < header->image_size) {
        uint32_t bytes_to_read = sizeof(buffer);
        if ((header->image_size - bytes_processed) < bytes_to_read) {
            bytes_to_read = header->image_size - bytes_processed;
        }

        flash_status_t f_status = flash_read(region, image_offset + bytes_processed, buffer, bytes_to_read);
        if (f_status != FLASH_OK) {
            return false; /* Failed to read image data */
        }

        calculated_crc = crc32_compute(buffer, bytes_to_read, calculated_crc);
        bytes_processed += bytes_to_read;
    }

    if (calculated_crc != header->crc) {
        return false; /* CRC mismatch */
    }

    return true;
}

static status_t update_image_state(flash_region_t slot_region, image_state_t new_state) {
    if (slot_region != FLASH_REGION_SLOT_A && slot_region != FLASH_REGION_SLOT_B) {
        return STATUS_ERROR_GENERIC;
    }

    /* In a real implementation, we might write to a separate metadata page
       or program a specific bit-field without erasing the whole header if flash supports it.
       For this abstraction, we will READ -> MODIFY -> ERASE/WRITE the header. 
       Note: This is risky if power fails during erase/write, ideally use a journal or separate word write.
    */
    
    image_header_t header;
    if (image_metadata_read(slot_region, &header) != STATUS_OK) {
        return STATUS_ERROR_GENERIC;
    }

    header.state = new_state;

    /* Stub: In real flash, we can't overwrite bytes from 1->0 or enum change without erase. 
       For this logic level, we assume we can update it (e.g. by rewriting header).
       TODO: Implement safe metadata update (e.g. swap scratchpad or bit-based flags).
    */
    
    /* Simulate Flash Write of updated header */
    /* flash_write(slot_region, offsetof(image_header_t, state), &new_state, sizeof(new_state)); */
    /* Updating the RAM copy for logic flow simulation during development if we had one */
    
    return STATUS_OK; 
}

status_t image_metadata_mark_trial(flash_region_t slot_region) {
    return update_image_state(slot_region, IMAGE_STATE_TRIAL);
}

status_t image_metadata_mark_confirmed(flash_region_t slot_region) {
    return update_image_state(slot_region, IMAGE_STATE_CONFIRMED);
}

status_t image_metadata_mark_invalid(flash_region_t slot_region) {
    return update_image_state(slot_region, IMAGE_STATE_INVALID);
}
