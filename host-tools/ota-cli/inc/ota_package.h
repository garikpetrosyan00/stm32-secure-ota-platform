#ifndef OTA_PACKAGE_H
#define OTA_PACKAGE_H

#include <stdint.h>
#include <stdbool.h>

#define OTA_PKG_MAGIC 0x4F544150 /* OTAP */

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint32_t header_version;
    uint32_t image_size;
    uint32_t image_crc;
    uint32_t version;
    uint32_t flags;
    uint8_t  signature[64];
    uint8_t  reserved[16];
} ota_package_header_t;

/**
 * @brief Create a firmware package.
 * 
 * @param input_bin Path to input binary.
 * @param output_pkg Path to output package.
 * @param version Image version.
 * @return int 0 on success, <0 on failure.
 */
int ota_package_create(const char* input_bin, const char* output_pkg, uint32_t version);

/**
 * @brief Print package info.
 * 
 * @param pkg_path Path to package.
 * @return int 0 on success.
 */
int ota_package_info(const char* pkg_path);

#endif /* OTA_PACKAGE_H */
