#include "boot_manager.h"
#include "boot_state.h"
#include "image_metadata.h"
#include "flash_if.h"
#include <stdio.h>

static boot_state_t current_state = BL_STATE_INIT;
static image_header_t header_slot_a;
static image_header_t header_slot_b;
static flash_region_t active_slot = FLASH_REGION_MAX;

static void log_state(const char* msg) {
    /* TODO: Replace with proper logging system later */
    printf("[BOOT] %s\n", msg);
}

static void transition_to(boot_state_t new_state) {
    current_state = new_state;
}

void boot_manager_init(void) {
    log_state("Initializing...");
    flash_init();
    image_metadata_init_header(&header_slot_a);
    image_metadata_init_header(&header_slot_b);
}

void boot_manager_run(void) {
    while (1) {
        switch (current_state) {
            case BL_STATE_INIT:
                log_state("State: INIT");
                transition_to(BL_STATE_CHECK_SLOTS);
                break;

            case BL_STATE_CHECK_SLOTS:
                log_state("State: CHECK_SLOTS");
                /* Read headers from both slots */
                if (image_metadata_read(FLASH_REGION_SLOT_A, &header_slot_a) != STATUS_OK) {
                    log_state("Error reading Slot A header");
                    header_slot_a.state = IMAGE_STATE_INVALID;
                }
                if (image_metadata_read(FLASH_REGION_SLOT_B, &header_slot_b) != STATUS_OK) {
                    log_state("Error reading Slot B header");
                    header_slot_b.state = IMAGE_STATE_INVALID;
                }

                /* TODO: Verify headers (Magic, CRC, Signature) */
                bool a_valid = image_metadata_verify_header(&header_slot_a);
                bool b_valid = image_metadata_verify_header(&header_slot_b);

                if (!a_valid) header_slot_a.state = IMAGE_STATE_INVALID;
                if (!b_valid) header_slot_b.state = IMAGE_STATE_INVALID;
                
                transition_to(BL_STATE_SELECT_SLOT);
                break;

            case BL_STATE_SELECT_SLOT:
                log_state("State: SELECT_SLOT");
                
                /* Selection Priority:
                   1. Slot A if CONFIRMED
                   2. Slot A if TRIAL
                   3. Slot B if PENDING (Trigger Swap/Update) -> Not handled fully in this logic, assuming usage of A as primary
                   4. Slot B if CONFIRMED/VALID and Slot A invalid
                */

                /* Hypothetical Rollback Check */
                if (header_slot_a.state == IMAGE_STATE_INVALID && header_slot_b.state == IMAGE_STATE_CONFIRMED) {
                    /* A is bad, B is good. Fallback to B. */
                    active_slot = FLASH_REGION_SLOT_B;
                    log_state("Rollback to Slot B (A is INVALID)");
                    transition_to(BL_STATE_PREPARE_BOOT);
                    break;
                }

                if (header_slot_a.state == IMAGE_STATE_CONFIRMED) {
                    active_slot = FLASH_REGION_SLOT_A;
                    log_state("Booting Slot A (CONFIRMED)");
                    transition_to(BL_STATE_PREPARE_BOOT);
                } else if (header_slot_a.state == IMAGE_STATE_TRIAL) {
                    /* Decrement counter would go here */
                    active_slot = FLASH_REGION_SLOT_A;
                    log_state("Booting Slot A (TRIAL - Attempt 1/1)");
                    transition_to(BL_STATE_PREPARE_BOOT);
                } else if (header_slot_a.state == IMAGE_STATE_PENDING) {
                    /* First boot after update, mark as TRIAL */
                    log_state("New Update in Slot A (PENDING -> TRIAL)");
                    if (image_metadata_mark_trial(FLASH_REGION_SLOT_A) != STATUS_OK) {
                        log_state("Failed to mark TRIAL");
                        transition_to(BL_STATE_ERROR);
                    } else {
                        /* Reload header to get new state */
                        image_metadata_read(FLASH_REGION_SLOT_A, &header_slot_a);
                        active_slot = FLASH_REGION_SLOT_A;
                        transition_to(BL_STATE_PREPARE_BOOT);
                    }
                } else {
                    /* Slot A is Empty/Invalid, check Slot B */
                    if (header_slot_b.state != IMAGE_STATE_INVALID && header_slot_b.state != IMAGE_STATE_EMPTY) {
                         active_slot = FLASH_REGION_SLOT_B;
                         log_state("Booting Slot B (Fallback)");
                         transition_to(BL_STATE_PREPARE_BOOT);
                    } else {
                        log_state("No valid images found!");
                        transition_to(BL_STATE_ERROR);
                    }
                }
                break;

            case BL_STATE_PREPARE_BOOT:
                log_state("State: PREPARE_BOOT");
                /* TODO: Disable interrupts, reset peripherals, relocate vector table */
                
                /* Emulate a fail safe: If we are booting TRIAL, and this was real hardware, 
                   we would enable a watchdog here. If the app doesn't mark CONFIRMED within X seconds,
                   the watchdog resets, and on next boot we see TRIAL again (or decrement counter).
                   If counter == 0, we mark INVALID.
                */
                if (active_slot == FLASH_REGION_SLOT_A && header_slot_a.state == IMAGE_STATE_TRIAL) {
                     log_state("Watchdog Enabled for TRIAL boot.");
                }

                transition_to(BL_STATE_BOOT);
                break;

            case BL_STATE_BOOT:
                log_state("State: BOOT");
                /* TODO: Jump to application */
                log_state("Jumping to application... (Simulation)");
                // In a real loop we wouldn't return or loop here unless jump failed
                return; 

            case BL_STATE_ROLLBACK:
                log_state("State: ROLLBACK");
                /* TODO: Perform swap back */
                transition_to(BL_STATE_ERROR); /* Stop for now */
                break;

            case BL_STATE_ERROR:
                log_state("State: ERROR");
                /* TODO: Blink LED SOS */
                while(1); /* Hang */
                break;

            default:
                transition_to(BL_STATE_ERROR);
                break;
        }
    }
}
