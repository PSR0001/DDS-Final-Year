/***********************
 * PIN 26,27
 * 00 = SINE
 * 01 = SQUARE
 * 10 = TRIANGLE
 * 11 = HALF SQUARE
 * 
 * PIN 15,4
 * 00 = 0 to 10KHz
 * 01 = 10KHz to 100KHz
 * 10 = 100KHz to 1MHz
 * 11 = 1M to REST
 * 
 * PIN 34,35
 * TUNING
 * 
 ***********************/

#include "AD9833.h"

#ifndef ESP32
#error ESP32 only example, please select appropriate board
#endif

// set pin number
const int potPin = 34;
const int potPin2 = 35;
int long potValue = 0;
int long potValue2 = 0;

// Set Pin Number for Waves
const int buttonWave1 = 26;
const int buttonWave2 = 27;
int Wave1 = 0;
int Wave2 = 0;
int last_Wave1 = 0;
int last_Wave2 = 1;

// Set Pin Numbers for Frequencies
const int buttonFreq1 = 15;
const int buttonFreq2 = 4;
int Freq1 = 0;
int Freq2 = 0;

//OFFSETS
int offset1=0;

SPIClass * myspi = new SPIClass(VSPI);
AD9833 AD(5, myspi);

void setup()
{
  Serial.begin(115200);
  delay(100);

  //Set PinMode
  pinMode(buttonFreq1, INPUT);
  pinMode(buttonFreq2, INPUT);
  pinMode(buttonWave1, INPUT);
  pinMode(buttonWave2, INPUT);
  pinMode(2, OUTPUT);

  myspi->begin();
  AD.begin();
  
  for(int i=0; i<4; i++){
    if(i % 2 == 0){
      digitalWrite(2,HIGH);
      delay(1000);
    }
    else{
      digitalWrite(2,LOW);
      delay(500);
    }
  }

}

void loop()
{
  //Read the states
  Freq1 = digitalRead(buttonFreq1);
  Freq2 = digitalRead(buttonFreq2);
  Wave1 = digitalRead(buttonWave1);
  Wave2 = digitalRead(buttonWave2);

  
/*===========For Frequency Range Select===============*/
  //0 to 10KHz <=> 00
  if(Freq1 == 0 && Freq2 == 0){
//    Serial.println("00");
      potValue = map(analogRead(potPin),0,4096,0,10000);       
      potValue2 = map(analogRead(potPin2),0,4096,0,10000);

      potValue /= 100;
      potValue *= 100;
      potValue2 /= 100;

      Serial.print(potValue);
      Serial.print(" + ");
      Serial.print(potValue2);
      Serial.print(" = ");
      Serial.println(potValue+potValue2);
      
      AD.setFrequency(potValue+potValue2, 0);
  }
  
  // 10KHz to 100KHz <=> 01
  else if(Freq1 == 0 && Freq2 == 1){
//    Serial.println("01");
      potValue = map(analogRead(potPin),0,4096,10000,100000);       
      potValue2 = map(analogRead(potPin2),0,4096,0,10000);

      potValue /= 1000;
      potValue *= 1000;
      potValue2 /= 100;
      potValue2 *= 10;
      offset1 = potValue/ 10000;

      Serial.print(potValue);
      Serial.print(" + ");
      Serial.print(potValue2);
      Serial.print(" + ");
      Serial.print(offset1);
      Serial.print(" = ");
      Serial.println(potValue+potValue2 + offset1);

      AD.setFrequency(potValue+potValue2+offset1, 0);
  }
  // 100KHz to 1MHz <=> 10
  else if(Freq1 == 1 && Freq2 == 0){
//    Serial.println("10");
      potValue = map(analogRead(potPin),0,4096,100,10000);
      potValue2 = map(analogRead(potPin2),0,4096,0,10000);

      potValue /= 100;
      potValue *= 10000;
      potValue2 /= 100;
      potValue2 *= 100;

      Serial.print(potValue);
      Serial.print(" + ");
      Serial.print(potValue2);
      Serial.print(" = ");
      Serial.println(potValue+potValue2);

      AD.setFrequency(potValue+potValue2, 0);
  }
  // 1MHz to rest <=> 11
  else{
//     Serial.println("11");
      potValue = map(analogRead(potPin),0,4096,1000,10000);
      potValue2 = map(analogRead(potPin2),0,4096,0,10000);

      potValue /= 100;
      potValue *= 100000;
      potValue2 /= 100;
      potValue2 *= 1000;

      Serial.print(potValue);
      Serial.print(" + ");
      Serial.print(potValue2);
      Serial.print(" = ");
      Serial.println(potValue+potValue2);
      AD.setFrequency(potValue+potValue2, 0);
  }

/*==================For Wave Select===============*/
if(Wave1 != last_Wave1 || Wave2 != last_Wave2){
  //Sine
  if(Wave1 == 0 && Wave2 == 0){
    Serial.println("Sine");
    AD.setWave(AD9833_SINE);
  }
  
  // Square
  else if(Wave1 == 0 && Wave2 == 1){
    Serial.println("Square");
    AD.setWave(AD9833_SQUARE1);
  }
  
  // Triangular
  else if(Wave1 == 1 && Wave2 == 0){
    Serial.println("Triangular");
    AD.setWave(AD9833_TRIANGLE);
  }
  // Sawtooth
  else{
      Serial.println("Sawtooth");
      AD.setWave(AD9833_SQUARE2);
  }

  last_Wave1 = Wave1;
  last_Wave2 = Wave2;
}

  delay(100);
}
