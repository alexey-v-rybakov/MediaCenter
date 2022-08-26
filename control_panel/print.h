#ifndef _PRINT_H_
#define _PRINT_H_

#ifndef SERIAL_OUTPUT
void print_action(const ControlCommand* cc) {}
void init_print() {}
#endif

#ifdef SERIAL_OUTPUT
void print_action(const ControlCommand* cc);
void init_print();
void send_button_state(BUTTON_LIST button);
#endif

#endif
