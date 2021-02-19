#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <M5StickC.h>

//日輪刀

#define PIN 32
#define LED_NUM 29
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
#include "mizu.inc"
#include "honoo.inc"
#include "kaminari.inc"
#include "mario.inc"

float ax, ay, az;
float cvec[3];
int cvec_cnt = 0;

void setup()
{
    M5.begin(false, true, true);
    M5.MPU6886.Init();

    strip.begin();
    strip.setBrightness(30);
    strip.show(); // Initialize all pixels to 'off'
}

void mizu_no_kokyuu(uint8_t wait_ms)
{
    uint16_t i, j;
    uint16_t line = (sizeof(mizu) / sizeof(mizu[0][0])) / strip.numPixels();
    uint32_t start;
    uint32_t start2 = micros();
    for (j = 0; j < line; j++)
    {
        start = micros();
        for (i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, mizu[i][j]);
        }
        strip.show();
        delayMicroseconds((wait_ms * 1000) - (micros() - start));
    }
    Serial.println(micros() - start2);
}

void honoo_no_kokyuu(uint8_t wait_ms)
{
    uint16_t i, j;
    uint16_t line = (sizeof(honoo) / sizeof(honoo[0][0])) / strip.numPixels();
    uint32_t start;
    uint32_t start2 = micros();
    for (j = 0; j < line; j++)
    {
        start = micros();
        for (i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, honoo[i][j]);
        }
        strip.show();
        delayMicroseconds((wait_ms * 1000) - (micros() - start));
    }
    Serial.println(micros() - start2);
}

void kaminari_no_kokyuu(uint8_t wait_ms)
{
    uint16_t i, j;
    uint16_t line = (sizeof(kaminari) / sizeof(kaminari[0][0])) / strip.numPixels();
    uint32_t start;
    uint32_t start2 = micros();
    for (j = 0; j < line; j++)
    {
        start = micros();
        for (i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, kaminari[i][j]);
        }
        strip.show();
        delayMicroseconds((wait_ms * 1000) - (micros() - start));
    }
    Serial.println(micros() - start2);
}

void kaminari_no_mario(uint8_t wait_ms)
{
    uint16_t i, j;
    uint16_t line = (sizeof(mario) / sizeof(mario[0][0])) / strip.numPixels();
    uint32_t start;
    uint32_t start2 = micros();
    for (j = 0; j < line; j++)
    {
        start = micros();
        for (i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, mario[i][j]);
        }
        strip.show();
        delayMicroseconds((wait_ms * 1000) - (micros() - start));
    }
    Serial.println(micros() - start2);
}

int btn = 0;
void loop()
{
    M5.MPU6886.getAccelData(&ax, &ay, &az);

    cvec[cvec_cnt++ % 3] = sqrt((ax * ax) + (ay * ay) + (az * az));
    delay(10);

    M5.update();
    if (M5.BtnA.wasPressed())
    {
        btn++;
    }

    if ((cvec[0] + cvec[1] + cvec[2]) / 3 > 2.0)
    {
        cvec[0] = 0;
        cvec[1] = 0;
        cvec[2] = 0;
        switch (btn)
        {
        case 0:
            mizu_no_kokyuu(5);
            break;
        case 1:
            honoo_no_kokyuu(5);
            break;
        case 2:
            kaminari_no_kokyuu(5);
            break;
        case 3:
            kaminari_no_mario(5);
            break;
        default:
            btn = 0;
            break;
        }
        strip.clear();
        strip.show();
    }
}