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
                /* Simple logic: Prefer CONFIRMED, then valid PENDING/TRIAL */
                /* For this skeleton, we just pick Slot A if valid, else B, else ERROR */
                
                /* Real logic for rollback/swap would check PENDING state in Slot B */
                if (header_slot_b.state == IMAGE_STATE_PENDING) {
                     /* TODO: Handle Swap Request */
                     log_state("Update Pending in Slot B. (Swap TODO)");
                }

                if (header_slot_a.state != IMAGE_STATE_INVALID) {
                    active_slot = FLASH_REGION_SLOT_A;
                    log_state("Selected Slot A");
                    transition_to(BL_STATE_PREPARE_BOOT);
                } else if (header_slot_b.state != IMAGE_STATE_INVALID) {
                    active_slot = FLASH_REGION_SLOT_B;
                    log_state("Selected Slot B (Fallback)");
                    transition_to(BL_STATE_PREPARE_BOOT);
                } else {
                    log_state("No valid images found!");
                    transition_to(BL_STATE_ERROR);
                }
                break;

            case BL_STATE_PREPARE_BOOT:
                log_state("State: PREPARE_BOOT");
                /* TODO: Disable interrupts, reset peripherals, relocate vector table */
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
