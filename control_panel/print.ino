#include "common.h"
#include "ram.h"
#include "button.h"

#ifdef SERIAL_OUTPUT

//MD_CirQueue rs_232_command(250, sizeof(char));

#define MAX_RS232_COMMAND 25
String  g_rs232_command;     
bool    g_ignore_rs232 = false; 

void init_print()
{
    Serial.begin(57600);
    Serial.println("INFO : Programm is running...");
}

void serialEvent() 
{
  while (Serial.available()) 
  {
    // получаем новый байт:
    char inChar = (char)Serial.read();
    if (g_ignore_rs232 == false)
     {
    
    g_rs232_command += inChar;
    if (inChar == '\n') 
    {
      g_rs232_command.replace("\n","");
      process_rs_232_command();
      g_rs232_command = "";
    } 
    if (g_rs232_command.length() == MAX_RS232_COMMAND)
     {
       g_rs232_command = "";
       g_ignore_rs232  = true;
       Serial.println("ERROR : RS232 command is too long");
     }
     }
     else
      {
        if (inChar == '\n')
          g_ignore_rs232 = false;
      }
  }
}

void send_button_state(BUTTON_LIST button)
 {
  if (g_action_pins[button])
       {
         Serial.print("BUTTON");
         Serial.print(button,DEC);
         Serial.print("=");
         Serial.print(g_action_pins[button]->get_state(),DEC);
         Serial.println();  
       }
  }

//
// Обработка команды RS232, после выполнения этой функции буфер g_rs232_command буфер должен быть пустым
//
void process_rs_232_command()
 {
  if (g_rs232_command.compareTo("get_free_ram") == 0)
   {
     Serial.print("ANSWER : get_free_ram : ");
     Serial.println(get_free_ram(), DEC);
     return;
   }
  if (g_rs232_command.compareTo("get_full_state") == 0)
   {
     for (int i = 0; i < BUTTON_LIST::NUM_OF_BUTTON; i++)
      send_button_state(i);
     return;
   }   
  else
   Serial.println("ERROR : RS232 command is unknow");
 }


void print_source(const char* title, int state)
{
  /*  Serial.print(title);
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
        break        case SOURCE::SOURCE_RESERV:
            Serial.println("SOURCE_RESERV");   
        break;
        default:
            Serial.println("!!!!!!!!! inpossible !!!!!!!!!");
        break;
    };    */
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
 {/*
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
     }*/
 }

#endif
