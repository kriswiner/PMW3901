/* 06/16/2017 Copyright Tlera Corporation
 *  
 *  Created by Kris Winer
 *  
 This sketch is to operate the 
 
 The sketch uses default SPI pins on the Dragonfly development board.

 Sketch copies from Bitcraze PMW3901 library-https://github.com/bitcraze/Bitcraze_PMW3901

 Library may be used freely and without limit with attribution.
 
  */
#include <SPI.h>
#include "Bitcraze_PMW3901.h"

#define SerialDebug true  // set to true to get Serial output for debugging
#define myLed 26

#define CSPIN  10  // default chip select for SPI
#define EN     31  // board enable is pulled up, set to TTL LOW to disable

int16_t deltaX,deltaY;

Bitcraze_PMW3901 flow(CSPIN); // Instantiate PMW3901

void setup() {
  Serial.begin(115200);
  delay(4000);
  Serial.println("Serial enabled!");

  // Configure led
  pinMode(myLed, OUTPUT);
  digitalWrite(myLed, HIGH); // start with led off since active LOW on Dragonfly

  // Configure SPI Flash chip select
  pinMode(CSPIN, OUTPUT);
  digitalWrite(CSPIN, HIGH);

 // pinMode(EN, OUTPUT);  // pulled up by 10 K resistor, set to LOW to disable sensor

  SPI.begin(); // initiate SPI 
  delay(1000);

  digitalWrite(myLed, LOW);

  uint8_t product_ID;
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CSPIN, LOW);
  SPI.transfer(0x00);
  product_ID = SPI.transfer(0);
  digitalWrite(CSPIN, HIGH);
  SPI.endTransaction();
  Serial.print("Product ID = 0x"); Serial.print(product_ID, HEX); Serial.println(" should be 0x49");

  uint8_t revision_ID;
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CSPIN, LOW);
  SPI.transfer(0x01);
  revision_ID = SPI.transfer(0);
  digitalWrite(CSPIN, HIGH);
  SPI.endTransaction();
  Serial.print("Revision ID = 0x"); Serial.println(revision_ID, HEX); 

  uint8_t inverse_product_ID;
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CSPIN, LOW);
  SPI.transfer(0x5F);
  inverse_product_ID = SPI.transfer(0);
  digitalWrite(CSPIN, HIGH);
  SPI.endTransaction();
  Serial.print("Inverse Product ID = 0x"); Serial.print(inverse_product_ID, HEX); Serial.println(" should be 0xB6"); 

  digitalWrite(myLed, HIGH);

  if (!flow.begin()) {
  Serial.println("Initialization of the flow sensor failed");
  while(1) { }
  
  }

  /* end of setup */

}

void loop() {
  
  // Get motion count since last call
  flow.readMotionCount(&deltaX, &deltaY);

  Serial.print("X: ");
  Serial.print(deltaX);
  Serial.print(", Y: ");
  Serial.print(deltaY);
  Serial.print("\n");

  delay(100);
}
