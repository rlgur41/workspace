#include <Wire.h>

#define PCA9685_ADDRESS 0x40
#define LINE1_ON_L 0x6
#define LINE1_ON_H 0x7
#define LINE1_OFF_L 0x8
#define LINE1_OFF_H 0x9
#define PCA9685_PRESCALE 0xFE

#define PRE_SCALE    0xFE

#define FREQUENCY   500
#define ADDRESS     0x40


#define PCA9685_I2C_BASE_ADDRESS 0x40
#define PCA9685_MODE1 0x00 // location for Mode1 register address
#define PCA9685_MODE2 0x01 // location for Mode2 reigster address

void writeRegister(int regAddress, byte data) {
  Wire.beginTransmission(0x40);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

void writeData(int number, word duration)
{
    Wire.beginTransmission(0x40);
    Wire.write(LINE1_ON_L + 4*number);
    Wire.write(lowByte(0));
    Wire.write(highByte(0));
    Wire.write(lowByte(duration));
    Wire.write(highByte(duration));
    Wire.endTransmission();
}

word readRegister(int regAddress) {

  word returnword = 0x00;
  Wire.beginTransmission(0x40);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom((int)0x40, 1);
    
//    int c=0;
  //Wait for our 2 bytes to become available
  while (Wire.available()) {
        //high byte
//        if (c==0)   { returnword = Wire.read() << 8; }
        //low byte
  //      if (c==1)   { 
    returnword |= Wire.read(); 
    //}
        //c++;
    }
    
  return returnword;
}


uint16_t getFrequency()

{

  uint8_t prescale;

  Wire.beginTransmission(0x40);

  Wire.write((uint8_t)PCA9685_PRESCALE);

  Wire.endTransmission();

   Wire.requestFrom(0x40, 1);

  if (Wire.available())

  {

    prescale = Wire.read();
    

  }

  return round((float)25000000 / (float)(((long)prescale + 1) * (long)4096));

}

void setFreq(uint16_t freq) {
  uint8_t prescale = round(25000000.0f / (float) (freq * 4096L)) - 1;
  uint8_t oldmode = readRegister(PCA9685_MODE1);
  uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
  writeRegister(PCA9685_MODE1, newmode); // go to sleep
  writeRegister(PRE_SCALE, prescale); // set the prescaler
  writeRegister(PCA9685_MODE1, oldmode);
  delay(5);
  writeRegister(PCA9685_MODE1, oldmode | 0xA1);

}

void setup()
{
 
   Wire.begin();
   Serial.begin(9600);
   delay(1);

  Serial.println(getFrequency());
  setFreq(500);
  delay(10);
  Serial.println(getFrequency());
  delay(1);
   writeRegister(PCA9685_MODE1, (byte)0x01); // reset the device
   Serial.println(readRegister(PCA9685_MODE1));
  delay(1);
   writeRegister(PCA9685_MODE1, (byte)B10100000);  // set up for auto increment
  delay(1);
   writeRegister(PCA9685_MODE2, (byte)0x10); // set to output
  delay(100);
  
  writeData(0, 2000);
   Serial.println(2000);
  delay(3000);

  for (int i = 0 ; i <= 14; i++) {    /* Low 1400 ~ 0 High*/
    writeData(0, 1400 - i * 100);
     Serial.println(1400 - i * 100);
    delay(1000);
  }
  
  writeData(0, 2000);
   Serial.println(3000);

  
}


void loop()
{
 

}
