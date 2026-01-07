#include "crc32.h"

#define CRC32_POLY 0x04C11DB7

/* Bitwise implementation to avoid large lookup tables in constrained environments */
uint32_t crc32_compute(const uint8_t *data, uint32_t length, uint32_t seed) {
    uint32_t crc = seed;
    
    while (length--) {
        crc ^= (uint32_t)(*data++) << 24;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x80000000) {
                crc = (crc << 1) ^ CRC32_POLY;
            } else {
                crc = (crc << 1);
            }
        }
    }
    
    return crc;
}
