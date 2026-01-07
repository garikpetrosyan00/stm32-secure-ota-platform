#include "app_config.h"
#include "common_defs.h"
#include "boot_confirm.h"
#include <stdio.h>

int main(void) {
    /* TODO: System Initialization */
    printf("--- Application Started ---\n");
    
    /* TODO: Perform Self-Tests (Peripherals, RAM, Sensors) */
    
    /* If tests pass, confirm the boot */
    if (boot_confirm_running_image() == STATUS_OK) {
        printf("Boot Confirmed (Marked as Valid/Confirmed)\n");
    } else {
        printf("Boot Confirm Failed or Already Confirmed\n");
    }
    
    /* TODO: Application logic loop */
    while(1) {
        /* TODO: Process tasks */
    }
    
    return 0;
}
