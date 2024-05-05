#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hardware/gpio.h"
#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"
#include "keyboard.h"
#include "keyer.h"

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void keyboard_mount_cb(void) {
}

// Invoked when device is unmounted
void keyboard_umount_cb(void) {
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void keyboard_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void keyboard_resume_cb(void) {
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

// used to avoid sending multiple consecutive zero reports for the keyboard
static bool keyboard_last_report_not_empty = false;

static void keyboard_send_hid_report(uint8_t report_id) {
    // skip if hid is not ready yet
    if (!tud_hid_ready()) {
        return;
    }

    switch(report_id) {
    case REPORT_ID_KEYBOARD: {
        uint8_t keycode[6] = {};
        int i = 0;
        
        // In Vband paddle mode we send left CTRL for dit and
        // right CTRL for dah
        if (keyer_dit) {
            keycode[i++] = HID_KEY_CONTROL_LEFT;
        }
        if (keyer_dah) {
            keycode[i++] = HID_KEY_CONTROL_RIGHT;
        }
        
        if (i != 0) {
            tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
            keyboard_last_report_not_empty = true;
        } else {
            // send empty key report if previously has key pressed
            if (keyboard_last_report_not_empty) {
                tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
            }
            keyboard_last_report_not_empty = false;
        }
    }
        break;
        
    default:
        break;
    }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void keyboard_task(void) {
    // Poll every 10ms
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if ( board_millis() - start_ms < interval_ms) {
        return; // not enough time
    }
    start_ms += interval_ms;

    // Remote wakeup
    if (tud_suspended() && (keyer_dit || keyer_dah || board_button_read())) {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    } else {
        // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
        keyboard_send_hid_report(REPORT_ID_KEYBOARD);
    }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len) {
    (void) instance;
    (void) len;

    uint8_t next_report_id = report[0] + 1;
    
    if (next_report_id < REPORT_ID_COUNT) {
        keyboard_send_hid_report(next_report_id);
    }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    // TODO not Implemented
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    (void) instance;

    if (report_type == HID_REPORT_TYPE_OUTPUT) {
        // Set keyboard LED e.g Capslock, Numlock etc...
        if (report_id == REPORT_ID_KEYBOARD) {
            // bufsize should be (at least) 1
            if ( bufsize < 1 ) {
                return;
            }
            
            uint8_t const kbd_leds = buffer[0];
            
            if (kbd_leds & KEYBOARD_LED_CAPSLOCK) {
                // Capslock On
            } else {
                // Caplocks Off
            }
        }
    }
}
