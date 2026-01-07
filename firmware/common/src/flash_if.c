#include "flash_if.h"
#include <stddef.h>

/* TODO: These should be defined based on the actual linker script symbols or definitions */
#define FLASH_BOOTLOADER_START  0x08000000
#define FLASH_BOOTLOADER_SIZE   (32 * 1024)

#define FLASH_SLOT_A_START      0x08008000
#define FLASH_SLOT_A_SIZE       (480 * 1024)

#define FLASH_SLOT_B_START      0x08080000
#define FLASH_SLOT_B_SIZE       (480 * 1024)

typedef struct {
    flash_addr_t start;
    flash_size_t size;
    bool erasable;
    bool writable;
} region_config_t;

static const region_config_t region_map[FLASH_REGION_MAX] = {
    [FLASH_REGION_BOOTLOADER] = { FLASH_BOOTLOADER_START, FLASH_BOOTLOADER_SIZE, false, false }, /* Protected */
    [FLASH_REGION_SLOT_A]     = { FLASH_SLOT_A_START,     FLASH_SLOT_A_SIZE,     true,  true  },
    [FLASH_REGION_SLOT_B]     = { FLASH_SLOT_B_START,     FLASH_SLOT_B_SIZE,     true,  true  },
    /* Metadata region is currently undefined in basic map, but available for custom use */
    [FLASH_REGION_METADATA]   = { 0,                      0,                     false, false },
};

static bool is_valid_region(flash_region_t region) {
    return (region < FLASH_REGION_MAX);
}

flash_status_t flash_init(void) {
    /* TODO: Initialize HAL Flash Driver here */
    return FLASH_OK;
}

flash_status_t flash_read(flash_region_t region, flash_addr_t offset, uint8_t *buffer, flash_size_t size) {
    if (!is_valid_region(region) || buffer == NULL) {
        return FLASH_ERROR_INVALID_PARAM;
    }

    const region_config_t *cfg = &region_map[region];

    if (offset + size > cfg->size) {
        return FLASH_ERROR_INVALID_PARAM;
    }

    flash_addr_t abs_addr = cfg->start + offset;

    /* TODO: Replace with HAL/LL flash read or direct memcpy if memory mapped */
    /* Note: STM32 Flash is memory mapped, so direct read is often possible,
       but going through specific accessor is safer for portability */
    
    // For stub purposes, we won't actually read hardware memory unless we are sure valid logic is present.
    // However, since it is memory mapped, specific logic might be:
    // const uint8_t *flash_ptr = (const uint8_t *)(uintptr_t)abs_addr;
    // memcpy(buffer, flash_ptr, size);
    (void)abs_addr; // Suppress unused warning for stub

    return FLASH_OK;
}

flash_status_t flash_write(flash_region_t region, flash_addr_t offset, const uint8_t *buffer, flash_size_t size) {
    if (!is_valid_region(region) || buffer == NULL) {
        return FLASH_ERROR_INVALID_PARAM;
    }

    const region_config_t *cfg = &region_map[region];

    if (!cfg->writable) {
        return FLASH_ERROR_LOCKED;
    }

    if (offset + size > cfg->size) {
        return FLASH_ERROR_INVALID_PARAM;
    }

    flash_addr_t abs_addr = cfg->start + offset;

    /* TODO: Add HAL Flash Unlock / Program / Lock sequence */
    (void)abs_addr; // Suppress unused warning

    return FLASH_OK;
}

flash_status_t flash_erase(flash_region_t region) {
    if (!is_valid_region(region)) {
        return FLASH_ERROR_INVALID_PARAM;
    }

    const region_config_t *cfg = &region_map[region];

    if (!cfg->erasable) {
        return FLASH_ERROR_LOCKED;
    }

    /* TODO: Call HAL erase function for specific pages/sectors in this region */

    return FLASH_OK;
}

flash_addr_t flash_get_start_addr(flash_region_t region) {
    if (!is_valid_region(region)) {
        return 0;
    }
    return region_map[region].start;
}

flash_size_t flash_get_region_size(flash_region_t region) {
    if (!is_valid_region(region)) {
        return 0;
    }
    return region_map[region].size;
}
