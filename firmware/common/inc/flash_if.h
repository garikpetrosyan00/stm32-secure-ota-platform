#ifndef FLASH_IF_H
#define FLASH_IF_H

#include <stdint.h>
#include <stdbool.h>

/* Flash Address and Size definitions */
typedef uint32_t flash_addr_t;
typedef uint32_t flash_size_t;

/* Flash Regions */
typedef enum {
    FLASH_REGION_BOOTLOADER = 0,
    FLASH_REGION_SLOT_A,
    FLASH_REGION_SLOT_B,
    FLASH_REGION_STORAGE,
    FLASH_REGION_METADATA_A, /* Optional: if metadata is separate */
    FLASH_REGION_METADATA_B,
    FLASH_REGION_MAX
} flash_region_t;

/* Flash Status Codes */
typedef enum {
    FLASH_OK = 0,
    FLASH_ERROR = -1,
    FLASH_ERROR_INVALID_PARAM = -2,
    FLASH_ERROR_LOCKED = -3,
    FLASH_ERROR_TIMEOUT = -4
} flash_status_t;

/**
 * @brief Initialize the flash interface.
 * 
 * @return flash_status_t FLASH_OK on success.
 */
flash_status_t flash_init(void);

/**
 * @brief Read data from flash.
 * 
 * @param region Target region to read from.
 * @param offset Offset within the region.
 * @param buffer Destination buffer.
 * @param size Number of bytes to read.
 * @return flash_status_t 
 */
flash_status_t flash_read(flash_region_t region, flash_addr_t offset, uint8_t *buffer, flash_size_t size);

/**
 * @brief Write data to flash.
 * 
 * @param region Target region to write to.
 * @param offset Offset within the region.
 * @param buffer Source buffer.
 * @param size Number of bytes to write.
 * @return flash_status_t 
 */
flash_status_t flash_write(flash_region_t region, flash_addr_t offset, const uint8_t *buffer, flash_size_t size);

/**
 * @brief Erase a flash region.
 * 
 * @param region Target region to erase.
 * @return flash_status_t 
 */
flash_status_t flash_erase(flash_region_t region);

/**
 * @brief Get the absolute start address of a region (Internal/HAL use mostly).
 * 
 * @param region Target region.
 * @return flash_addr_t Absolute address, or 0 if invalid.
 */
flash_addr_t flash_get_start_addr(flash_region_t region);

/**
 * @brief Get the size of a region.
 * 
 * @param region Target region.
 * @return flash_size_t Size in bytes.
 */
flash_size_t flash_get_region_size(flash_region_t region);

#endif /* FLASH_IF_H */
