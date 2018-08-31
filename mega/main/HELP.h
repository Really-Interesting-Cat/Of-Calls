#ifndef __HELP__
#define __HELP__

void led_strip_init(void);
void in_danger_led(int pin);
void safe_led(void);
int what_help(void);
void in_danger(int pin);
void IRQ_DANGER(void);

#endif
