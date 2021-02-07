#include "M5Atom.h"

uint16_t analogRead_value = 0;
uint16_t digitalRead_value = 0;

void readEarthSensor()
{
  analogRead_value = analogRead(32);
  digitalRead_value = digitalRead(26);
  Serial.printf(" %d, %d\n",  digitalRead_value, analogRead_value);
}

void setup()
{
  M5.begin(true, false, true);
  delay(10);
  Serial.begin(115200);
  M5.dis.clear();
  pinMode(26, INPUT);
  Serial.println("\nEarth Test");
}

void loop()
{
  // Read earth sensor value
  readEarthSensor();
  delay(1000);
}