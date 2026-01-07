#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

#include <stdint.h>
#include <stdbool.h>

/* TODO: Add common error codes */
typedef enum {
    STATUS_OK = 0,
    STATUS_ERROR_GENERIC = -1
} status_t;

/* TODO: Add versioning structures */
typedef struct {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
} version_t;

#endif /* COMMON_DEFS_H */
