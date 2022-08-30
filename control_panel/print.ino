#include "common.h"
#include "ram.h"
#include "button.h"


#ifdef SERIAL_OUTPUT

#define MAX_RS232_COMMAND 25

typedef void (*rs232_process)(char* cmd);


class RS232_Command
 {
  public:
    RS232_Command(rs232_process _prc_func)
     {
       prc_func = _prc_func;
       m_pop_index = 0;
     }
  private:
    rs232_process prc_func;
    char      m_rs232_command[MAX_RS232_COMMAND];
    uint8_t   m_push_index;
    uint8_t   m_pop_index;
    bool      m_ignore;
  public:
    void clear()
     {
        m_ignore = false;
        memset(m_rs232_command, 0, sizeof(m_rs232_command));
        m_push_index = 0;
     }
    bool push(char cmd_char)
     {
        if (m_ignore == false)
          {
            m_rs232_command[m_push_index] = cmd_char;
            if (cmd_char == '\n') 
             {
               preprocess_command();
               return true;
             } 
           m_push_index++;  
           if (m_push_index == MAX_RS232_COMMAND)
            {
             strcpy(m_rs232_command, "error_command_long");
             preprocess_command();
             m_ignore  = true;
             return true;
            }
         }
        else
         {
          if (cmd_char == '\n')
            clear();
         }
       return false;  
     }   
    public:
     bool get_int_param(int& param)
      {
        char* p = get_param();
        if (p == NULL)
          return false;

         for (int i = 0; i < strlen(p); i++) {
          if (!isdigit(p[i])) return false;
        }


        
        param = atoi(p);
        return true;
      }

      bool get_bool_param(bool& param)
      {
        int b;
        if (!get_int_param(b))
         return false;
        param = (b == 0)?false:true; 
        return true;
      }
    
     char* get_param()
      {
        uint8_t pop_index_tmp = m_pop_index + strlen(m_rs232_command + m_pop_index) + 1;
        if (pop_index_tmp >= MAX_RS232_COMMAND)
         return NULL;
        if (m_rs232_command[pop_index_tmp] == 0)
         return NULL;
        m_pop_index = pop_index_tmp;
        return (m_rs232_command + m_pop_index); 
      }
    private:
    void preprocess_command()
     {
      for (int i = 0; i < MAX_RS232_COMMAND; i++)
       {
        if (m_rs232_command[i] == 0) break;
        if ((m_rs232_command[i] == ':')||(m_rs232_command[i] == '\n')) m_rs232_command[i] = 0; 
       } 
       m_pop_index = 0;
       prc_func(m_rs232_command);
       clear();
     }
 };



RS232_Command g_rs232_command(process_rs_232_command);

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
    g_rs232_command.push(inChar);
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
void process_rs_232_command(char* cmd)
 {
  //---------------------------------------
  Serial.print("cmd : "); Serial.println(cmd);
  //---------------------------------------
//  while (true)
//   {
//    char* param  = g_rs232_command.get_param();
 //   if (param == NULL)
 //    break;
//    Serial.print("param : "); Serial.println(param); 
 //  }

  
if (strcmp(cmd, "set_led_brightness") == 0)
   {
    int state;
    if (!g_rs232_command.get_int_param(state)) return;
     {
      set_led_brightness(state);
      Serial.print("set_led_brightness:");Serial.println(state, DEC);     
     }
      return;
   }

  
  if (strcmp(cmd, "get_free_ram") == 0)
   {
     Serial.print("ANSWER : get_free_ram : ");
     Serial.println(get_free_ram(), DEC);
     return;
   }
  if (strcmp(cmd, "get_full_state") == 0)
   {
     for (int i = 0; i < BUTTON_LIST::NUM_OF_BUTTON; i++)
      send_button_state(i);
     return;
   }   
  if (strcmp(cmd, "digit_off") == 0)
   {
    bool state;
    if (!g_rs232_command.get_bool_param(state)) return;
     {
      on_off_digit(state);
      Serial.print("digit_off:");Serial.println(state, DEC);    
     }
    return;
   }
  if (strcmp(cmd, "set_digit") == 0)
   {
    int state;
    if (!g_rs232_command.get_int_param(state)) return;
     {
      show_digit(state);
      Serial.print("set_digit:");Serial.println(state, DEC);    
     }
      return;
   }
   
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
