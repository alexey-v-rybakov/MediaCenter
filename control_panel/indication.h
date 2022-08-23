#ifndef _INDICATION_H_
#define _INDICATION_H_


void init_indication();


void on_off_digit(bool on);
void show_digit(int digit);
void on_off_dot(bool on);
void set_led_brightness(int br);
void update_indication();
void test_led();

#endif
