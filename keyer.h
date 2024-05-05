#ifndef KEYER_H_
#define KEYER_H_

extern void keyer_init(void);
extern void keyer_task(void);
extern bool keyer_dit;
extern bool keyer_dah;

typedef enum {
    VBAND_PADDLE = 0,
    VBAND_STRAIGHT,
} vband_keyer_t;

extern vband_keyer_t vband_mode;

#endif /* KEYER_H_ */
