#include <I2C.h>
#include <PCA9685.h>
PCA9685 whiteDrv(0x43);

void setup()
{
  whiteDrv.begin();
  Serial.begin(9600);

  whiteDrv.setPWMFrequency(200);
  Serial.println(whiteDrv.getPWMFrequency());

  //turn everything off
  whiteDrv.PWM(0, 15, 0);  
}

void loop()
{
  int i = 0;
  int j = 0;
  int k = 0;

  while (i < 6) {
    j = 0;
    while (j <= 255) {
      whiteDrv.PWM(i, j);
      j++;
    }
    i++;
  }

  while (i >= 0) {
    j = 255;
    while (j >= 0) {
      whiteDrv.PWM(i, j);
      j--;
    }
    i--;
  }
  
  Serial.println(whiteDrv.getPWM(0));

  delay(500);

  while (k <= 255) {
    whiteDrv.PWM(0, 5, k);
    k++;
  }
  
  Serial.println(whiteDrv.getPWM(0));

  delay(1000);
  
  while (k >= 0) {
    whiteDrv.PWM(0, 5, k);
    k--;
  }
  
  Serial.println(whiteDrv.getPWM(0));

  delay(500);
}
