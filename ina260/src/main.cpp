#include "M5Atom.h"

#include "ina260.h"

INA260 ina260(0x44);

#define BUFF_SIZE (1000)
volatile uint32_t us_list[BUFF_SIZE];
volatile int16_t ua_list[BUFF_SIZE];
volatile uint16_t uv_list[BUFF_SIZE];
int buff_cnt = 0;

void setup()
{
  M5.begin();
  Serial.begin(115200);
  ina260.begin(26, 32);
  buff_cnt = 0;
}

void loop()
{
  int32_t uv; // = ina260.getVoltage_uv();
  int32_t ua; // = ina260.getCurrent_ua() + 2500;
  int32_t uw; // = ina260.getPower_uw();
  uint32_t us;
  uint32_t flush_size;

  while (1)
  {
    uv = ina260.getVoltage_uv();
    ua = ina260.getCurrent_ua() + 2500;
    uw = ina260.getPower_uw();

    us_list[buff_cnt] = micros();
    uv_list[buff_cnt] = uv / 1000;
    ua_list[buff_cnt] = ua / 1000;
    buff_cnt = (buff_cnt + 1) % BUFF_SIZE;

    if (buff_cnt == 0)
    {
      flush_size = BUFF_SIZE;
      for (int i = 0; i < flush_size; i++)
      {
        us = us_list[i];
        uv = uv_list[i];
        ua = ua_list[i];

        Serial.print(us);
        Serial.print("us,");
        Serial.print(uv);
        Serial.print("mV,");
        Serial.print(ua);
        Serial.print("mA,");
        Serial.println("");
      }
    }
    delayMicroseconds(100);
  }
}
