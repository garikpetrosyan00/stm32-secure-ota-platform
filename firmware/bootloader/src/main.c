#include "boot_manager.h"
#include <stdio.h> /* For TODO logging */

int main(void) {
    /* TODO: System Initialization (HAL_Init, Clock Setup) */
    
    printf("--- Bootloader Started ---\n");

    boot_manager_init();
    boot_manager_run();
    
    /* Should not reach here */
    while(1);
    
    return 0;
}
