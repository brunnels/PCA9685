/**
 * PCA9685.cpp - Arduino library to control PCA9685 chip.
 */
#include "Arduino.h"
#include <I2C.h>
#include "PCA9685.h"

extern I2C I2c;

#define CIELPWM(a) (pgm_read_word_near(CIEL12 + a)) // CIE Lightness lookup table function
/**
 * 12 bits PWM to CIE Luminance conversion
 * L* = 116(Y/Yn)^1/3 - 16 , Y/Yn > 0.008856
 * L* = 903.3(Y/Yn), Y/Yn <= 0.008856
 */
prog_uint16_t CIEL12[] PROGMEM = { 0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 18, 20, 21, 23, 25, 27, 28, 30,
  32, 34, 36, 37, 39, 41, 43, 45, 47, 49, 52, 54, 56, 59, 61, 64, 66, 69, 72, 75, 77, 80, 83, 87,
  90, 93, 97, 100, 103, 107, 111, 115, 118, 122, 126, 131, 135, 139, 144, 148, 153, 157, 162, 167,
  172, 177, 182, 187, 193, 198, 204, 209, 215, 221, 227, 233, 239, 246, 252, 259, 265, 272, 279,
  286, 293, 300, 308, 315, 323, 330, 338, 346, 354, 362, 371, 379, 388, 396, 405, 414, 423, 432,
  442, 451, 461, 471, 480, 490, 501, 511, 521, 532, 543, 554, 565, 576, 587, 599, 610, 622, 634,
  646, 658, 670, 683, 696, 708, 721, 734, 748, 761, 775, 789, 802, 817, 831, 845, 860, 875, 890,
  905, 920, 935, 951, 967, 983, 999, 1015, 1032, 1048, 1065, 1082, 1099, 1117, 1134, 1152, 1170,
  1188, 1206, 1225, 1243, 1262, 1281, 1301, 1320, 1340, 1359, 1379, 1400, 1420, 1441, 1461, 1482,
  1504, 1525, 1547, 1568, 1590, 1613, 1635, 1658, 1681, 1704, 1727, 1750, 1774, 1798, 1822, 1846,
  1871, 1896, 1921, 1946, 1971, 1997, 2023, 2049, 2075, 2101, 2128, 2155, 2182, 2210, 2237, 2265,
  2293, 2322, 2350, 2379, 2408, 2437, 2467, 2497, 2527, 2557, 2587, 2618, 2649, 2680, 2712, 2743,
  2775, 2807, 2840, 2872, 2905, 2938, 2972, 3006, 3039, 3074, 3108, 3143, 3178, 3213, 3248, 3284,
  3320, 3356, 3393, 3430, 3467, 3504, 3542, 3579, 3617, 3656, 3694, 3733, 3773, 3812, 3852, 3892,
  3932, 3973, 4013, 4055, 4095 };

PCA9685::PCA9685(byte address)
{
  _address = address;
  _freq = 200;
}

/**
 * I2C.begin for I2C.
 */
void PCA9685::begin()
{
  I2c.begin();
  wake();
}

/**
 * Wake PCA9685 oscillator and enable auto increment
 */
void PCA9685::wake()
{
  I2c.write(_address, (byte) 0x00, (byte) 0B00100001);
}

/**
 * Set PWM Frequency 40-1000Hz, Default 200Hz
 */
void PCA9685::setPWMFrequency(uint16_t freq)
{
  uint8_t prescale = (6104 / freq) - 1; // FIXME preScale =round(25000000Hz/(4096*freq))-1
  PWMPreScale(prescale);
  _freq = freq;
}

/**
 * Get PWM Frequency
 */
uint16_t PCA9685::getPWMFrequency()
{
  uint16_t freq;
  I2c.read(_address, (byte) 0xFE, (uint8_t) 4);
  while (I2c.available()) {
    freq = I2c.receive();
  }
  return freq;
}

void PCA9685::PWM(uint8_t startChannel, uint8_t endChannel, uint16_t value)
{
  PWM12(startChannel, endChannel, CIELPWM(value));
}

/**
 * Single channel 8bit PWM only.  Slow in loop
 */
void PCA9685::PWM(uint8_t channel, uint16_t value)
{
  PWM12(channel, CIELPWM(value));
}

void PCA9685::PWM12(uint8_t startChannel, uint8_t endChannel, uint16_t value)
{
  uint8_t start = (startChannel * 4) + 6;
  int range = ((endChannel + 1) - startChannel) * 4;

  uint8_t values[range];
  memset(values, 0, range);

  int stackptr = 2;
  while(startChannel <= endChannel) {
    values[stackptr++] = lowByte(value);
    values[stackptr++] = highByte(value);
    stackptr += 2;
    startChannel++;
  }

  I2c.write(_address, start, values, range);
}

/**
 * Single channel 12bit PWM only.  Slow in loop
 */
void PCA9685::PWM12(uint8_t channel, uint16_t value)
{
  uint8_t values[2] = { lowByte(value), highByte(value) };
  channel = (channel * 4) + 8;
  I2c.write(_address, channel, values, 2);
}

/**
 * Get the current 12 bit PWM value
 */
uint16_t PCA9685::getPWM(uint8_t channel)
{
  uint8_t onLow, onHi, offLow, offHi;
  channel = (channel * 4) + 6;
  I2c.read(_address, channel, (uint8_t) 4);
  while (I2c.available()) {
    onLow = I2c.receive();
    onHi = I2c.receive();
    offLow = I2c.receive();
    offHi = I2c.receive();
  }

  return (word((offHi & 0B00001111), offLow) - word((onHi & 0B00001111), onLow));
}

/**
 * PCA9685 PWM frequency prescale
 */
void PCA9685::PWMPreScale(uint8_t prescale)
{
  I2c.write(_address, (byte) 0xFE, prescale);
}
