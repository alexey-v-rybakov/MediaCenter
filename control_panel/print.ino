#include "common.h"
#ifdef SERIAL_OUTPUT

void init_print()
{
    Serial.begin(115200);
    Serial.println("Programm is running...");
}

void print_source(const char* title, int state)
{
    Serial.print(title);
    switch (state)
    {
        case SOURCE::UNSELECTED:
            Serial.println("UNSELECTED");
        break;
        case SOURCE::LP:
            Serial.println("LP");
        break;
        case SOURCE::RADIO:
            Serial.println("RADIO");
        break;
        case SOURCE::CHROMOCAST:
            Serial.println("CHROMOCAST");
        break;
        case SOURCE::SOURCE_RESERV:
            Serial.println("SOURCE_RESERV");   
        break;
        default:
            Serial.println("!!!!!!!!! inpossible !!!!!!!!!");
        break;
    };    
}

void print_analog(const char* title, int state)
{
    Serial.print(title);
    Serial.println(state, DEC);
}

void print_button(const char* title, int state)
{
    Serial.print(title);
    switch (state)
    {
        case BUTTON_STATE::RELEASED:
            Serial.println("RELEASED");
        break;
        case BUTTON_STATE::PUSHED:
            Serial.println("PUSHED");
        break;
        case BUTTON_STATE::PUSHED_LONG:
            Serial.println("PUSHED_LONG");
        break;
        default:
            Serial.println("!!!!!!!!! inpossible !!!!!!!!!");
        break;
    };    
}

void print_action(const ControlCommand* cc)
 {
     switch (cc->action)
     {
        case ACTION_LIST::SOURCE_SELECT:
            print_source("source    =  ", cc->state);
        break;
        case ACTION_LIST::PT_1:
        {
         // int a = (*100) / 116;
            print_analog("pt1       =  ", cc->state);
        }
        break;
        case ACTION_LIST::PT_2:
        {
         // int a = (cc->state*100) / 116;
            print_analog("pt2       =  ", cc->state);
        }
        break;
        case ACTION_LIST::BUTTON_1:
            print_button("button1   =  ", cc->state);
        break;
        case ACTION_LIST::BUTTON_2:
            print_button("button2   =  ", cc->state);
        break;
        case ACTION_LIST::BUTTON_3:
            print_button("button3   =  ", cc->state);
        break;
        case ACTION_LIST::BUTTON_4:
            print_button("button4   =  ", cc->state);
        break;
        case ACTION_LIST::ON_OFF:
            print_button("on-off    =  ", cc->state);
        break;
        default:
        break;
     }
 }

#endif
