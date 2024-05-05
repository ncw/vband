#include <stdint.h>
#include "hardware/gpio.h"
#include "bsp/board.h"
#include "keyer.h"

// GPIO pins
#define KEYER_OUT_GPIO 22
#define LEFT_PADDLE_GPIO 17
#define RIGHT_PADDLE_GPIO 16

// State of the keyer
bool keyer_dit;
bool keyer_dah;

// Read the state of the keyer
void keyer_read_paddles(void) {
    keyer_dit = !gpio_get(LEFT_PADDLE_GPIO);
    keyer_dah = !gpio_get(RIGHT_PADDLE_GPIO);
    board_led_write(keyer_dit ^ keyer_dah);
}

// Every 1ms poll the keyer
void keyer_task(void) {
    // Poll every 1ms
    const uint32_t interval_ms = 1;
    static uint32_t start_ms = 0;

    if ( board_millis() - start_ms < interval_ms) {
        return; // not enough time
    }
    start_ms += interval_ms;
    
    keyer_read_paddles();
}

void keyer_init(void) {
    // Initialise key input
    gpio_init(LEFT_PADDLE_GPIO);
    gpio_set_dir(LEFT_PADDLE_GPIO, GPIO_IN);
    gpio_pull_up(LEFT_PADDLE_GPIO);
    
    gpio_init(RIGHT_PADDLE_GPIO);
    gpio_set_dir(RIGHT_PADDLE_GPIO, GPIO_IN);
    gpio_pull_up(RIGHT_PADDLE_GPIO);

    keyer_read_paddles();
}
