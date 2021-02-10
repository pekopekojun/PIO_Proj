#include "ina260.h"
#include <Wire.h>

// INA260 Registers
#define INA260_REG_CONFIGURATION 0x00 // Configuration Register (R/W)
#define INA260_REG_CURRENT 0x01       // Current (R)
#define INA260_REG_BUS_VOLTAGE 0x02   // Bus Voltage (R)
#define INA260_REG_POWER 0x03         // Power (R)

#define INA260_REG_MASK_ENABLE 0x06 // Mask/Enable (R/W)
#define INA260_REG_ALERT_LIMIT 0x07 // Alert Limit (R/W)
#define INA260_REG_DIE_ID 0xFF      // Die ID (R)

INA260::INA260(int8_t saddr)
{
  s_addr = saddr;
}

void INA260::writeRegister(uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(s_addr);
  Wire.write(reg);
  Wire.write((value >> 8) & 0xFF);
  Wire.write(value & 0xFF);
  Wire.endTransmission();
}

uint16_t INA260::readRegister(uint8_t reg)
{
  uint16_t res = 0x0000;

  Wire.beginTransmission(s_addr);
  Wire.write(reg);

  if (Wire.endTransmission() == 0)
  {
    if (Wire.requestFrom(s_addr, 2) >= 2)
    {
      res = Wire.read() * 256;
      res += Wire.read();
    }
  }

  return res;
}

int32_t INA260::conv_sign(int reg)
{
  int32_t reg_tmp = reg;

  if ((reg_tmp & 0x8000UL) != 0)
  {
    reg_tmp = (~reg_tmp) & 0x7fff;
    reg_tmp++;
    reg_tmp *= -1;
  }
  return reg_tmp;
}

uint16_t INA260::getRawCurrent()
{
  return (uint16_t)readRegister(INA260_REG_CURRENT);
}

uint16_t INA260::getRawVoltage()
{
  return (uint16_t)readRegister(INA260_REG_BUS_VOLTAGE);
}

uint16_t INA260::getRawPower()
{
  return (uint16_t)readRegister(INA260_REG_POWER);
}

int32_t INA260::getCurrent_ua()
{
  return conv_sign(getRawCurrent()) * 1250L;
}

int32_t INA260::getVoltage_uv()
{
  return getRawVoltage() * 1250L;
}

int32_t INA260::getPower_uw()
{
  return conv_sign(getRawPower()) * 10000L;
}

void INA260::setConfig(uint16_t config)
{
  writeRegister(INA260_REG_CONFIGURATION, config);
}

uint16_t INA260::getConfig()
{
  return (uint16_t)readRegister(INA260_REG_CONFIGURATION);
}

bool INA260::begin(int sda, int scl, uint32_t frequency)
{
  bool ret = Wire.begin(sda, scl, frequency);
  /*
  writeRegister(INA260_REG_CONFIGURATION, INA260_CONF_RESET_INACTIVE |
                                              INA260_CONF_MODE_CONT_SHUNT_AND_BUS |
                                              INA260_CONF_SHCT_1100uS |
                                              INA260_CONF_VBUSCT_1100uS |
                                              INA260_CONF_AVG_16);
  */
  writeRegister(INA260_REG_CONFIGURATION, INA260_CONF_RESET_INACTIVE |
                                              INA260_CONF_MODE_CONT_SHUNT_AND_BUS |
                                              INA260_CONF_SHCT_1100uS |
                                              INA260_CONF_VBUSCT_1100uS |
                                              INA260_CONF_AVG_1);
  return ret;
}
