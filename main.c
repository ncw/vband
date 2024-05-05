#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hardware/gpio.h"
#include "bsp/board.h"
#include "tusb.h"
#include "keyboard.h"
#include "keyer.h"

// Main code
int main(void) {
    board_init();
    tusb_init();
    keyer_init();

    while (1) {
        tud_task();
        keyer_task();
        keyboard_task();
        sleep_ms(1);
    }
    
    return 0;
}
