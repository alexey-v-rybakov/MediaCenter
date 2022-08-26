#ifndef _COMMON_H_
#define _COMMON_H_

#define SERIAL_OUTPUT
#define I2C_CTRL_BLOCK_ADDR 8



//
// Перечисление источников входного сигнала
//
#define SOURCE 5

typedef enum BUTTON_STATE
 {
    RELEASED = 0,
    PUSHED      ,
    PUSHED_LONG
 };


typedef enum BUTTON_LIST
{
    SOURCE_SELECT = 0,
    PT_1             ,
    PT_2             ,
    BUTTON_1         ,
    BUTTON_2         ,
    BUTTON_3         ,
    BUTTON_4         ,
    ON_OFF           ,
    NUM_OF_BUTTON    

};


typedef enum RS232_COMMAND
{
    GET_FREE_RAM  = 0,
    GET_FULL_STATE   ,
    NUM_OF_RS232_COMMAND   

};

typedef enum ACTION_LIST
{
    BUTTON_ACTION = 0,
    RS232_ACTION     ,
    NUM_OF_ACTION    

};

typedef struct ButtonParam
 {
   BUTTON_LIST button_number;
   int         button_state;
 };

typedef struct RS232Param
 {
   RS232_COMMAND cmd_code;
   uint8_t       param_a;
   uint8_t       param_b;
 };


typedef union CommandParam
 {
    ButtonParam button_param;
    RS232Param  rs232_param;
 };

//
// Структура, которая сохраняет состояние системы
//
typedef struct ControlCommand
 {
     int    action;
     CommandParam param;
 };

#endif
