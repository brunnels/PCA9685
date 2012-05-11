/**
* Arduino library to control PCA9685 chip.
*/

#ifndef PCA9685_h
#define PCA9685_h

#include "Arduino.h"
#include <I2C.h>

class PCA9685
{
  public:
    PCA9685(byte address);
    void        begin();
    void        wake();
    void        setPWMFrequency(uint16_t frequency);
    uint16_t    getPWMFrequency();
    void        PWM(uint8_t startChannel, uint8_t endChannel, uint16_t value);
    void        PWM(uint8_t channel , uint16_t value);
    void        PWM12(uint8_t startChannel, uint8_t endChannel, uint16_t value);
    void        PWM12(uint8_t channel , uint16_t value);


    uint16_t    getPWM(uint8_t channel);
  private:
    byte        _address;
    void        PWMPreScale(uint8_t prescale);
};
#endif
