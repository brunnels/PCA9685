#include <Wire.h>
#include <PCA9685.h>
PCA9685 driver1(0x41);

void setup() 
{
  driver1.begin();
  Serial.begin(9600);
}

void loop() 
{ 
  driver1.PWMSingle(0, 255); delay (3000); 
  Serial.println(driver1.getPWM(0));
  Serial.println("full");
}