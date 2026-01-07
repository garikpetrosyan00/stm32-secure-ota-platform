#include "ota_transport.h"
#include <stdio.h>

int ota_transport_send(const char* pkg_path, const char* port) {
    printf("--- OTA Transport Mock ---\n");
    printf("Opening port: %s\n", port);
    printf("Reading package: %s\n", pkg_path);
    printf("sending header... OK\n");
    printf("sending chunks... [==========] 100%%\n");
    printf("waiting for ack... OK\n");
    printf("Transfer Complete.\n");
    return 0;
}
