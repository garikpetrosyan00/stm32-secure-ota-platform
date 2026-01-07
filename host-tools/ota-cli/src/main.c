#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("STM32 Secure OTA CLI Tool\n");

    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        return 1;
    }

    /* TODO: Implement CLI commands for OTA */
    /* e.g., default sign <file>, upload <port> <file> */

    return 0;
}
