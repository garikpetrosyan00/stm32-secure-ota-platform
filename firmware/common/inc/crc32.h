#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Compute CRC32 (Standard 0x04C11DB7)
 * 
 * @param data Pointer to data buffer
 * @param length Length of data in bytes
 * @param seed Initial CRC value (usually 0xFFFFFFFF)
 * @return uint32_t Final CRC32 value
 */
uint32_t crc32_compute(const uint8_t *data, uint32_t length, uint32_t seed);

#endif /* CRC32_H */
