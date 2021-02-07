#include "M5Atom.h"

#include "ina260.h"

INA260 ina260(0x44);

#define BUFF_SIZE (25 * 1000)
volatile uint16_t ua_list[BUFF_SIZE];
volatile uint16_t uv_list[BUFF_SIZE];
int buff_cnt = 0;

void setup()
{
  M5.begin();
  Serial.begin(115200);
  ina260.begin(26, 32);
}

void loop()
{
  int32_t uv = ina260.getVoltage_uv();
  int32_t ua = ina260.getCurrent_ua() + 2500;
  int32_t uw = ina260.getPower_uw();

  uv_list[buff_cnt % BUFF_SIZE] = uv / 1000;
  ua_list[buff_cnt++ % BUFF_SIZE] = ua / 1000;

  Serial.print(uv / 1000.0f, 2);
  Serial.print("mV,");
  Serial.print(ua / 1000.0f, 2);
  Serial.print("mA,");
  Serial.print(uw / 1000.0f, 2);
  Serial.print("mW,");
  Serial.println("");

  delay(100);
}
