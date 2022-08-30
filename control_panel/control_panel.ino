#include <SD.h>
#include "hardware.h"
#include "common.h"
#include <Wire.h>
#include <TimerOne.h>
#include "print.h"
#include "button.h"
#include <MD_CirQueue.h>
#include "indication.h"
#include "ram.h"


const int source_selector_voltage[] = {0, 512, 614, 767, 1023};

//
// Размер очереди сообщений. При переполнении - перезагрузка
const PROGMEM uint8_t QUEUE_SIZE = 10;
MD_CirQueue g_command_queue(QUEUE_SIZE, sizeof(ControlCommand));
ActionPin* g_action_pins[BUTTON_LIST::NUM_OF_BUTTON];

//--------------------------------------------------------------------------------------------------------------------------
//
// инициализация прошивки
//
//--------------------------------------------------------------------------------------------------------------------------
void setup()
{
   init_print();
   
    g_action_pins[BUTTON_LIST::SOURCE_SELECT]  = new MultiButton(A7, BUTTON_LIST::SOURCE_SELECT, source_selector_voltage, sizeof(source_selector_voltage)/sizeof(int));
    g_action_pins[BUTTON_LIST::PT_1]           = new Potintiometr(A3, BUTTON_LIST::PT_1);
    g_action_pins[BUTTON_LIST::PT_2]           = new Potintiometr(A2, BUTTON_LIST::PT_2);
    g_action_pins[BUTTON_LIST::BUTTON_1]       = new Button(A0, BUTTON_LIST::BUTTON_1, BUTTON_TYPE::NON_FIX_BUTTON);
    g_action_pins[BUTTON_LIST::BUTTON_2]       = new Button(9, BUTTON_LIST::BUTTON_2, BUTTON_TYPE::FIX_BUTTON);
    g_action_pins[BUTTON_LIST::BUTTON_3]       = new Button(8, BUTTON_LIST::BUTTON_3, BUTTON_TYPE::NON_FIX_BUTTON);
    g_action_pins[BUTTON_LIST::BUTTON_4]       = new Button(7, BUTTON_LIST::BUTTON_4, BUTTON_TYPE::FIX_BUTTON);
    g_action_pins[BUTTON_LIST::ON_OFF]         = new Button(2, BUTTON_LIST::ON_OFF, BUTTON_TYPE::NON_FIX_BUTTON);
    //
    g_command_queue.begin();
    //fTimer
    Timer1.initialize(50000);
    Timer1.attachInterrupt(update_state);
    //
    init_indication();
    on_off_digit(true);
    test_led(); 
}

//---------------------------------------------------------------------------------------------
//
// Обновляем состояние пинов
//
//---------------------------------------------------------------------------------------------
static int s = 0;
void update_state()
{
  for (int i = 0; i < BUTTON_LIST::NUM_OF_BUTTON; i++)
  if (g_action_pins[i])
    if (g_action_pins[i]->process())
      {
        ControlCommand cc;
        cc.action = ACTION_LIST::BUTTON_ACTION;
        g_action_pins[i]->get_action(&(cc.param.button_param));
        if (!g_command_queue.push((uint8_t*)&cc))
          ASSERT(); 
      }
 update_indication();
}

int on_off = 1;
int led_count = 0;
int count = 0;


void loop()
{
      
 while (true)
   {
      bool is_empty;
      noInterrupts();
      is_empty = g_command_queue.isEmpty();
      interrupts();
      if (is_empty)
        break;
      ControlCommand cc;
      noInterrupts();
      g_command_queue.pop((uint8_t*)&cc);
      interrupts();
      if (cc.action == ACTION_LIST::BUTTON_ACTION)
        send_button_state(cc.param.button_param.button_number);
      //set_led_brightness(100);
        
    /*  if (cc.action == NUM_OF_ACTION)
      {
        
        }
      
      //
      if (cc.action == ON_OFF)
       if (cc.state == PUSHED)
        test_led(); 
      //
      if (cc.action == ACTION_LIST::PT_1)
        {
          set_led_brightness((cc.state*100) / 116);
        
        }
       */
      //print_action(&cc);
   }
  // show_digit(count);
}
