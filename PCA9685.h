/**
* Arduino library to control PCA9685 chip.
*/

#ifndef PCA9685_h
#define PCA9685_h

#include "Arduino.h"
#include <Wire.h>

class PCA9685 
{
  public:
    PCA9685(uint8_t address);
    void        begin();
    void        wake();    
    void        setPWMFrequency(uint16_t frequency);
    uint16_t    getPWMFrequency();
    void        PWMSingle(uint8_t , uint16_t);
    void        PWMSingle12(uint8_t , uint16_t);
    uint16_t    getPWM(uint8_t channel);
  private:
    uint8_t     _address;
    uint16_t    _freq;
    void        PWMPreScale(uint8_t prescale);
};
#endif