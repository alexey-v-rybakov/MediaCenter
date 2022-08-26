#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "common.h"



#define TICK_PER_SECONDS    20
#define JITTER_COUNT        2
#define LONG_PUSH_COUNT     (TICK_PER_SECONDS*3)    
#define POTENTIOMETR_JITTER 4

enum
{
    DIGITAL         = 0,
    ANALOG             ,
    DIGITAL_PULLUP     ,
    ANALOG_PULLUP   
};


class ActionPin
 {
     public:
        ActionPin(int pin_number, BUTTON_LIST action, int pin_type)
        {
            m_action = action;
            m_pin    = pin_number;
            m_pin_type = pin_type;
            if ((m_pin_type == DIGITAL_PULLUP)|| (m_pin_type == ANALOG_PULLUP))
                pinMode(m_pin, INPUT_PULLUP);
            else
                pinMode(m_pin, INPUT);
        }

        void get_action(ButtonParam* cmd)
         {
             cmd->button_number = m_action;
             cmd->button_state  = get_state();
             return;
         }

        virtual bool process()
         {
             if ((m_pin_type == DIGITAL)||(m_pin_type == DIGITAL_PULLUP))
                m_state = digitalRead(m_pin);
             else   
                m_state = analogRead(m_pin);   
             return false;
         }

         virtual int get_state()
         {
            return m_state;
         }

        

     protected:
        
        
    private:  
        int         m_state; 
        int         m_pin;
        int         m_pin_type;
        BUTTON_LIST m_action;
         
 };

 class Potintiometr : public ActionPin
  {
      public:
        Potintiometr(int pin_number, BUTTON_LIST action):ActionPin(pin_number, action, ANALOG)
        {
            m_old_state = INT16_MAX;
        }

        virtual bool process()
        {
            ActionPin::process();
            if (abs(m_old_state - ActionPin::get_state()) > POTENTIOMETR_JITTER)
            {
                m_old_state = ActionPin::get_state();
                return true;
            }
            return false;
        }

      private:
        int  m_old_state;  
    
  }; 

 class MultiButton : public ActionPin
 {
     public:
        MultiButton(int pin_number, BUTTON_LIST action, const int* voltage, int n_buttons):ActionPin(pin_number, action, ANALOG)
        {
            m_voltage = new int[n_buttons];
            m_n_buttons = n_buttons;
            m_button_selected = INT16_MAX;
            memcpy(m_voltage, voltage, n_buttons*sizeof(int));

        }

        virtual bool process()
        {
            int delta       = INT16_MAX;
            int m_tmp_state;
            ActionPin::process();
            for (int i = 0; i < m_n_buttons; i++)
                {      
                    int dt = abs(ActionPin::get_state() - m_voltage[i]); 
                    if (dt < delta) 
                    {
                        m_tmp_state = i; 
                        delta = dt;
                    }
                }
            if (m_tmp_state != m_button_selected)
                {
                    m_button_selected = m_tmp_state;
                    return true;
                }   
            return false;     

        }
        virtual int get_state()
        {
           return m_button_selected; 
        }

        
    private:
        int* m_voltage;
        int  m_n_buttons;
        int  m_button_selected;

 };

typedef enum BUTTON_TYPE
{
    NON_FIX_BUTTON = 0,
    FIX_BUTTON

};

 class Button : public ActionPin
 {
     public:
        Button(int pin_number, BUTTON_LIST action, BUTTON_TYPE button_type):ActionPin(pin_number, action, DIGITAL_PULLUP)
        {
            m_button_state      = BUTTON_STATE::RELEASED;
            m_button_tmp_state  = BUTTON_STATE::RELEASED;
            m_button_type       = button_type;
            m_press_count       = 0;

        }

        virtual bool process()
        {
            int  pin_state;
            int  old_state = m_button_state;
            ActionPin::process();
            pin_state = ActionPin::get_state();
            if (pin_state == 0)
            {
              if (m_button_tmp_state == BUTTON_STATE::RELEASED)
               {
                   m_button_tmp_state   = BUTTON_STATE::PUSHED;
                   m_press_count        = 1; 
               } 
              else
               {
                   m_press_count++;
                   if (m_press_count >= LONG_PUSH_COUNT)
                    {
                        m_button_state = BUTTON_STATE::PUSHED_LONG;
                        m_press_count  = LONG_PUSH_COUNT;
                    }
                   else
                    {
                        if (m_press_count > JITTER_COUNT)
                            m_button_state = BUTTON_STATE::PUSHED;
                    } 
               }  

            }
            else // не ноль, кнопку отжали
            {
                if (m_button_tmp_state == BUTTON_STATE::PUSHED) 
                {
                    m_button_tmp_state = BUTTON_STATE::RELEASED;
                    m_press_count        = 1; 
                }
                else
                {
                    m_press_count++;
                    if (m_press_count > JITTER_COUNT)
                        {
                            m_button_state = BUTTON_STATE::RELEASED;
                            m_press_count = 0;
                        }

                }

            }
            if (m_button_type == BUTTON_TYPE::FIX_BUTTON)
                if (m_button_state == BUTTON_STATE::PUSHED_LONG)
                    m_button_state = BUTTON_STATE::PUSHED;

            return (old_state != m_button_state);
        }

        virtual int get_state()
        {
            return m_button_state;
        }
    private:
        int         m_button_state;
        int         m_button_tmp_state;
        int         m_press_count;
        BUTTON_TYPE m_button_type;
 };
extern ActionPin* g_action_pins[];

#endif
