#include "boot_manager.h"
#include <stdio.h> /* For TODO logging */

int main(void) {
    /* TODO: System Initialization (HAL_Init, Clock Setup) */
    
    printf("--- Bootloader Started ---\n");
    printf("Lifecycle: INIT -> CHECK -> SELECT -> BOOT\n");

    /* 
       Boot Manager Flow:
       1. Init & Read Headers
       2. Check for PENDING update in Slot A (simulated swap completion)
       3. If PENDING, mark TRIAL.
       4. If TRIAL, enable Watchdog, Boot.
       5. App MUST mark CONFIRMED to disable Watchdog on next boot.
    */

    boot_manager_init();
    boot_manager_run();
    
    /* Should not reach here */
    while(1);
    
    return 0;
}
