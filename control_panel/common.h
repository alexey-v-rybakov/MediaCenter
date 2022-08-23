#ifndef _COMMON_H_
#define _COMMON_H_

#define SERIAL_OUTPUT
#define I2C_CTRL_BLOCK_ADDR 8

//
// Перечисление источников входного сигнала
//
typedef enum SOURCE 
{
    UNSELECTED     =    0,  // не выбрано
    LP                  ,   // проигрыватель
    RADIO               ,   // радио
    CHROMOCAST          ,   // Google chromecast на TV
    SOURCE_RESERV       ,   // зарезервировано
    NUM_SOURCE
} ;

typedef enum BUTTON_STATE
 {
    RELEASED = 0,
    PUSHED      ,
    PUSHED_LONG
 };

typedef enum ACTION_LIST
{
    SOURCE_SELECT = 0,
    PT_1             ,
    PT_2             ,
    BUTTON_1         ,
    BUTTON_2         ,
    BUTTON_3         ,
    BUTTON_4         ,
    ON_OFF           ,
    NUM_OF_ACTION
};
//
// Структура, которая сохраняет состояние системы
//
typedef struct ControlCommand
 {
     int    action;
     int    state;
 };

#endif
