#include "ota_package.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

/* Simple CRC32 stub */
static uint32_t calc_crc32(const uint8_t *data, size_t size) {
    uint32_t crc = 0xFFFFFFFF;
    /* TODO: Real CRC32 implementation */
    (void)data; (void)size; (void)crc;
    return 0xDEADBEEF; 
}

int ota_package_create(const char* input_bin, const char* output_pkg, uint32_t version) {
    FILE *fin, *fout;
    struct stat st;
    uint8_t *buffer;

    if (stat(input_bin, &st) != 0) {
        perror("stat input");
        return -1;
    }

    fin = fopen(input_bin, "rb");
    if (!fin) {
        perror("fopen input");
        return -1;
    }

    buffer = malloc(st.st_size);
    if (fread(buffer, 1, st.st_size, fin) != (size_t)st.st_size) {
        perror("fread");
        fclose(fin);
        free(buffer);
        return -1;
    }
    fclose(fin);

    ota_package_header_t header;
    memset(&header, 0, sizeof(header));
    header.magic = OTA_PKG_MAGIC;
    header.header_version = 1;
    header.image_size = st.st_size;
    header.version = version;
    header.image_crc = calc_crc32(buffer, st.st_size);

    fout = fopen(output_pkg, "wb");
    if (!fout) {
        perror("fopen output");
        free(buffer);
        return -1;
    }

    fwrite(&header, 1, sizeof(header), fout);
    fwrite(buffer, 1, st.st_size, fout);

    fclose(fout);
    free(buffer);
    
    printf("Created package '%s' (Size: %ld bytes, Ver: %u)\n", output_pkg, st.st_size + sizeof(header), version);
    return 0;
}

int ota_package_info(const char* pkg_path) {
    FILE *fin;
    ota_package_header_t header;

    fin = fopen(pkg_path, "rb");
    if (!fin) {
        perror("fopen package");
        return -1;
    }

    if (fread(&header, 1, sizeof(header), fin) != sizeof(header)) {
        fprintf(stderr, "Error: File too small or read fail.\n");
        fclose(fin);
        return -1;
    }
    fclose(fin);

    printf("Package: %s\n", pkg_path);
    if (header.magic != OTA_PKG_MAGIC) {
        printf("  INVALID MAGIC: 0x%08X\n", header.magic);
        return -1;
    }
    printf("  Magic:        0x%08X (OK)\n", header.magic);
    printf("  Image Ver:    %u\n", header.version);
    printf("  Image Size:   %u bytes\n", header.image_size);
    printf("  CRC32:        0x%08X\n", header.image_crc);

    return 0;
}
