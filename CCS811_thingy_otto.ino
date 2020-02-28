/***************************************************************************
  This is a library for the CCS811 air

  This sketch reads the sensor

  Designed specifically to work with the Adafruit CCS811 breakout
  ----> http://www.adafruit.com/products/3566

  These sensors use I2C to communicate. The device's I2C address is 0x5A

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include "Adafruit_CCS811.h"
#include <DHT.h>
#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_CCS811 ccs;
int ledDry = 9; //led til tørhed
int ledFugt = 12; //led til fugt
int ledCo2 = 11; //led til Co2
int ledKold = 10; //led til koldhed
int ledVarm = 420; //led til varme

void setup() {
  Serial.begin(9600);
  dht.begin(); //start DHT
  pinMode(ledVarm, OUTPUT); //Led outputs
  pinMode(ledKold, OUTPUT);
  pinMode(ledCo2, OUTPUT);
  pinMode(ledFugt, OUTPUT);
  pinMode(ledDry, OUTPUT);  
  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  // Wait for the sensor to be ready
  while(!ccs.available());
}

void loop() {
  int fugt = dht.readHumidity();
  int temp = dht.readTemperature();
  int co2 = ccs.geteCO2();
  if(ccs.available()){
    if(!ccs.readData()){
    Serial.print("Humidity: ");
    Serial.print(fugt);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.print(" Celsius");
    Serial.print(" AirQuality=");
    Serial.print(co2);
    Serial.println(" PPM");
    if (temp < 20)
      digitalWrite(ledKold, HIGH);
    else if (temp > 30)
      digitalWrite(ledVarm, HIGH);
    else{
      digitalWrite(ledVarm, LOW);
      digitalWrite(ledKold, LOW);
    }
    if (fugt > 60)
      digitalWrite(ledFugt, HIGH);
    else if (fugt < 40)
      digitalWrite(ledDry, HIGH);
    else{
      digitalWrite(ledFugt, LOW);
      digitalWrite(ledDry, LOW);
    }
    if (co2 > 1000)
      digitalWrite(ledCo2, HIGH);
    delay(1000);
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
  delay(500);
}
