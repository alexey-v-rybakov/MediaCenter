#include "indication.h"

#define NUM_LEDS 33
#include "FastLED.h"
#define PIN 10
CRGB leds[NUM_LEDS];

static bool g_dot_state   = false;
static bool g_digit_state = false;
static int  g_cur_br      = 0;
static int  g_need_br     = g_cur_br;

void init_indication()
{
    pinMode(11, OUTPUT);
    pinMode(13, OUTPUT);
      
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    

    on_off_digit(false);
    show_digit(0);

    FastLED.addLeds<WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalPixelString );
    FastLED.setBrightness(g_cur_br);

    return;
}

#define STEP_PER_TICK 3 
void update_indication()
 {
    if (g_need_br == g_cur_br) 
      return;
    if (g_need_br > g_cur_br)
      {
        g_cur_br += STEP_PER_TICK;
        if (g_cur_br > g_need_br)
          g_cur_br = g_need_br;
      }
    else 
     {
       g_cur_br -= STEP_PER_TICK;
       if (g_cur_br < g_need_br)
        g_cur_br = g_need_br;

     }  
    FastLED.setBrightness(g_cur_br);
    FastLED.show();
 }

void set_led_brightness(int br)
{
  if (br > 100) br  = 100;
  if (br < 0)   br  = 0;
  g_need_br = br;
}

void on_off_digit(bool on)
 {  
     digitalWrite(11, on?LOW:HIGH);
     digitalWrite(13, (on&g_dot_state)?HIGH:LOW);
     g_digit_state = on;
 }

 void on_off_dot(bool on)
 {  
    g_dot_state = on;
    on_off_digit(g_digit_state);
 }

void show_digit(int digit)
 {
    digitalWrite(6, (digit%10 & 0x01)?HIGH:LOW);
    digitalWrite(5, (digit%10 & 0x02)?HIGH:LOW);
    digitalWrite(4, (digit%10 & 0x04)?HIGH:LOW);
    digitalWrite(3, (digit%10 & 0x08)?HIGH:LOW);
 }

static int cled = 0;


CRGB clr[] = 
{
CRGB::AliceBlue ,
CRGB::Aquamarine,
CRGB::Black,
CRGB::Brown,
CRGB::Chocolate,
CRGB::Crimson,
CRGB::DarkGoldenrod,
CRGB::DarkKhaki,
CRGB::DarkOrchid,
CRGB::DarkSlateBlue,
CRGB::DarkViolet,
CRGB::DimGrey,
CRGB::ForestGreen,
CRGB::Gold,
CRGB::Green,
CRGB::IndianRed,
CRGB::Lavender,
CRGB::LightBlue,
CRGB::LightGreen,
CRGB::LightSeaGreen ,
CRGB::LightSteelBlue ,
CRGB::Linen ,
CRGB::MediumBlue ,
CRGB::MediumSlateBlue ,
CRGB::MidnightBlue ,
CRGB::NavajoWhite ,
CRGB::OliveDrab ,
CRGB::PaleGoldenrod ,
CRGB::PapayaWhip ,
CRGB::Plaid ,
CRGB::Red ,
CRGB::Salmon ,
CRGB::Sienna ,
CRGB::SlateGray ,
CRGB::SteelBlue ,
CRGB::Tomato ,
CRGB::White ,
CRGB::FairyLight };


void test_led()
 {


  
     for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = clr[cled];
       /*if (i > 27)
        leds[i] = CRGB::AliceBlue;
        else
        {leds[i] = CRGB::AliceBlue ;
        
        }*/
      }
  //leds[cled%NUM_LEDS] = CRGB::Green; 

  cled++;
  if (cled >= (sizeof(clr)/sizeof(CRGB)))
    cled = 0;
  FastLED.show();
 }





/* count++;
  
  if (on_off == 0)
    leds[led_count] = CHSV(0, 0, 0);
  else 
    {
      //if (count%3 == 0)
     //  leds[led_count] = CRGB::Blue; //CHSV(255, 0, 255);
      //  if (count%3 == 1)
      //  leds[led_count] = CRGB::Green; //CHSV(255, 0, 255);
      //   if (count%3 == 2)
     //   leds[led_count] = CRGB::Red; //CHSV(255, 0, 255);
        leds[led_count] = CRGB::White;
    }
    
  led_count++;
  if (led_count >= NUM_LEDS)
   {
    led_count = 0;
    if (on_off == 0) on_off = 1;
    else             on_off = 0;
   }
    
  FastLED.show();
  
  delay(50);         // скорость движения радуги*/
