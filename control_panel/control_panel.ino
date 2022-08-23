#include <SD.h>

#include "hardware.h"
#include "common.h"
#include <Wire.h>
#include <TimerOne.h>
#include "print.h"
#include "button.h"
#include <MD_CirQueue.h>
#include "indication.h"

int source_selector_voltage[NUM_SOURCE] = {0, 510, 610, 710, 1020};

//
// Размер очереди сообщений. При переполнении - перезагрузка
const uint8_t QUEUE_SIZE = 40;
MD_CirQueue g_command_queue(QUEUE_SIZE, sizeof(ControlCommand));





ActionPin* g_action_pins[ACTION_LIST::NUM_OF_ACTION];

//--------------------------------------------------------------------------------------------------------------------------
//
// инициализация прошивки
//
//--------------------------------------------------------------------------------------------------------------------------
void setup()
{
    init_print();

    g_action_pins[ACTION_LIST::SOURCE_SELECT]  = new MultiButton(A7, ACTION_LIST::SOURCE_SELECT, source_selector_voltage, sizeof(source_selector_voltage)/sizeof(int));
    g_action_pins[ACTION_LIST::PT_1]           = new Potintiometr(A3, ACTION_LIST::PT_1);
    g_action_pins[ACTION_LIST::PT_2]           = new Potintiometr(A2, ACTION_LIST::PT_2);
    g_action_pins[ACTION_LIST::BUTTON_1]       = new Button(A0, ACTION_LIST::BUTTON_1, BUTTON_TYPE::NON_FIX_BUTTON);
    g_action_pins[ACTION_LIST::BUTTON_2]       = new Button(9, ACTION_LIST::BUTTON_2, BUTTON_TYPE::FIX_BUTTON);
    g_action_pins[ACTION_LIST::BUTTON_3]       = new Button(8, ACTION_LIST::BUTTON_3, BUTTON_TYPE::NON_FIX_BUTTON);
    g_action_pins[ACTION_LIST::BUTTON_4]       = new Button(7, ACTION_LIST::BUTTON_4, BUTTON_TYPE::FIX_BUTTON);
    g_action_pins[ACTION_LIST::ON_OFF]         = new Button(2, ACTION_LIST::ON_OFF, BUTTON_TYPE::NON_FIX_BUTTON);
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
void update_state()
{
  for (int i = 0; i < ACTION_LIST::NUM_OF_ACTION; i++)
  if (g_action_pins[i])
    if (g_action_pins[i]->process())
      {
        ControlCommand cc;
        g_action_pins[i]->get_action(&cc);
        if (!g_command_queue.push((uint8_t *)&cc))
          ASSERT(); 
      }
  update_indication();
}

int on_off = 1;
int led_count = 0;
int count = 0;


void loop()
{
      

  count++;
  
  delay(500); 
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
      //
      if (cc.action == ON_OFF)
       if (cc.state == PUSHED)
        test_led(); 
      //
      if (cc.action == ACTION_LIST::PT_1)
        {
          set_led_brightness((cc.state*100) / 116);
        
        }
       
      print_action(&cc);
   }
   show_digit(count);
   
}
