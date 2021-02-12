#include "M5Atom.h"
#include <FastLED.h>
#include "ina260.h"

INA260 ina260(0x44);

#define BUFF_SIZE (15000)

volatile uint32_t us_list[BUFF_SIZE];
volatile int16_t ma_list[BUFF_SIZE];
volatile uint8_t mv_100_list[BUFF_SIZE];
int buff_cnt = 0;

//const int NUM_LEDS = 25;
const int LED_PIN = 27;
static CRGB leds[NUM_LEDS];

void setup()
{
  M5.begin(true, true, false);
  Serial.begin(115200);
  ina260.begin(26, 32);
  buff_cnt = 0;

  FastLED.addLeds<WS2812, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
}

void drawpix(uint32_t i, uint32_t c)
{
  leds[i] = c; /*GGRRBB*/
  FastLED.show();
}

void clearpix(void)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = 0;
  }
  FastLED.show();
}

void loop()
{
  int32_t mv;
  int32_t ma;
  int32_t uv;
  int32_t ua;
  uint32_t us;
  uint32_t flush_size = 0;
  uint32_t old_color = 0;
  uint32_t new_color = 0;

  while (1)
  {
    uv = ina260.getVoltage_uv();
    ua = ina260.getCurrent_ua() + 2500;

    us_list[buff_cnt] = micros();
    mv_100_list[buff_cnt] = uv / 100000;
    ma_list[buff_cnt] = ua / 1000;
    buff_cnt = (buff_cnt + 1) % BUFF_SIZE;
    /*GGRRBB*/

    new_color = (buff_cnt * 255) / BUFF_SIZE;
    if (new_color != old_color)
    {
      drawpix((buff_cnt * 25) / BUFF_SIZE, ((255 - new_color) << 16) | (new_color << 8));
    }
    old_color = new_color;

    if (buff_cnt == 0)
    {
      flush_size = BUFF_SIZE;
    }

    M5.update();
    if (M5.Btn.wasPressed())
    {
      flush_size = buff_cnt;
      buff_cnt = 0;
    }

    if (buff_cnt == 0)
    {
      Serial.println("us,mv,ma,#####");
      for (int i = 0; i < flush_size; i++)
      {
        us = us_list[i];
        mv = mv_100_list[i];
        ma = ma_list[i];

        Serial.print(us);
        Serial.print(",");
        Serial.print(mv / 10);
        Serial.print(".");
        Serial.print(mv % 10);
        Serial.print(",");
        Serial.print(ma);
        Serial.print("");
        Serial.println("");

        new_color = (i * 25) / flush_size;
        if (new_color != old_color)
        {
          drawpix(25 - new_color, 0);
        }
        old_color = new_color;
      }
      clearpix();
    }
    //delayMicroseconds(100);
  }
}
