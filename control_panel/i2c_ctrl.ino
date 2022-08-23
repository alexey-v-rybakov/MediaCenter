#include "common.h"
#include <Wire.h>

void init_i2c()
{
    Wire.begin(I2C_CTRL_BLOCK_ADDR); 
       
    Wire.onReceive(receiveEvent); 
    Wire.onRequest(requestEvent); 
    
}

void receiveEvent(int howMany) 
{
// while (0 <Wire.available()) {
//    char c = Wire.read();      /* receive byte as a character */
//    Serial.print(c);           /* print the character */
//  }
// Serial.println();             /* to newline */
}

// function that executes whenever data is requested from master
void requestEvent() 
{
 //Wire.write("Hi Master");  /*send string on request */
}
