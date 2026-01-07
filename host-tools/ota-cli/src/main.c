#include "ota_package.h"
#include "ota_transport.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_usage(const char* prog) {
    printf("Usage:\n");
    printf("  %s pack <input.bin> <output.pkg> [version]\n", prog);
    printf("  %s info <package.pkg>\n", prog);
    printf("  %s send <package.pkg> <port>\n", prog);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "pack") == 0) {
        if (argc < 4) {
            print_usage(argv[0]);
            return 1;
        }
        uint32_t ver = 1;
        if (argc >= 5) ver = atoi(argv[4]);
        return ota_package_create(argv[2], argv[3], ver);
    } 
    else if (strcmp(argv[1], "info") == 0) {
        if (argc < 3) {
            print_usage(argv[0]);
            return 1;
        }
        return ota_package_info(argv[2]);
    } 
    else if (strcmp(argv[1], "send") == 0) {
        if (argc < 4) {
            print_usage(argv[0]);
            return 1;
        }
        return ota_transport_send(argv[2], argv[3]);
    } 
    else {
        printf("Unknown command: %s\n", argv[1]);
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}
