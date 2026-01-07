#ifndef IMAGE_METADATA_H
#define IMAGE_METADATA_H

#include <stdint.h>
#include "common_defs.h"

#define IMAGE_HEADER_MAGIC      0x96F3B83D
#define IMAGE_HEADER_SIZE       1024

typedef enum {
    IMAGE_STATE_EMPTY = 0,
    IMAGE_STATE_VALID,
    IMAGE_STATE_PENDING,
    IMAGE_STATE_TRIAL,
    IMAGE_STATE_CONFIRMED,
    IMAGE_STATE_INVALID
} image_state_t;

typedef struct {
    uint32_t magic;
    uint32_t image_size;
    version_t version;
    uint32_t crc;
    image_state_t state;
    uint32_t flags;
    uint8_t reserved[32];
    /* Signature follows usually, or is part of specific TLV structure */
} image_header_t;

/**
 * @brief Initialize/Clear a header structure.
 * 
 * @param header Pointer to header.
 */
void image_metadata_init_header(image_header_t *header);

/**
 * @brief Read image header from a specific slot.
 * 
 * @param slot_region FLASH_REGION_SLOT_A or FLASH_REGION_SLOT_B.
 * @param header Destination structure.
 * @return status_t 
 */
status_t image_metadata_read(int slot_region, image_header_t *header);

/**
 * @brief Write image header to a specific slot.
 * 
 * @param slot_region FLASH_REGION_SLOT_A or FLASH_REGION_SLOT_B.
 * @param header Source structure.
 * @return status_t 
 */
status_t image_metadata_write(int slot_region, const image_header_t *header);

/**
 * @brief Validate the header magic and basic fields.
 * 
 * @param header Pointer to header.
 * @return true If magic matches.
 * @return false If invalid.
 */
bool image_metadata_verify_header(const image_header_t *header);

#endif /* IMAGE_METADATA_H */
