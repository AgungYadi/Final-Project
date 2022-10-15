#include <Arduino.h>
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include "Adafruit_ADS1015.h"
#include "EEPROM.h"
float calibration_ph = -1.7;
DFRobot_ESP_PH_WITH_ADC ph;
Adafruit_ADS1115 ads;
float voltage, phValue = 25;

#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter;

#include <OneWire.h>
#include <DallasTemperature.h>
const int SENSOR_PIN = 14;
OneWire oneWire(SENSOR_PIN);
DallasTemperature sensors(&oneWire);
float temperature;

#include <EEPROM.h>
#define TdsSensorPin 13
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0;

const int TRIG_PIN  = 15; 
const int ECHO_PIN  = 2; 
float duration_us, distance_cm;


void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  Wire.begin();
  lcd.begin(); 
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("READY");
  delay(1000);
  
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial.println("starting ");
  EEPROM.begin(32);
  
  ph.begin();
  ads.setGain(GAIN_ONE);
  ads.begin();

  lightMeter.begin();

  sensors.begin();    // initialize the senso
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("READY");
  delay(1000);
  Serial.println("ready ");
}
void loop() {
  uint16_t lux = lightMeter.readLightLevel();
  baca_suhu();
  baca_ph();
  baca_tds();
  baca_ultra();
  

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LUX:");
  lcd.print(lux);
  lcd.setCursor(10,0);
  lcd.print("SUHU:");
  lcd.print(temperature);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("PH:");
  lcd.print(phValue);
  lcd.setCursor(10,1);
  lcd.print("TDS:");
  lcd.print(tdsValue);
  lcd.setCursor(0,2);
  lcd.print("JARAK:");
  lcd.print(distance_cm);
 

  Serial.print(lux);
  Serial.print("A ");
  Serial.print(temperature);
  Serial.print("B ");
  Serial.print(phValue);
  Serial.print("C ");
  Serial.print(tdsValue);
  Serial.print("D ");
  Serial.print(distance_cm);
  Serial.print("E ");
  Serial.println();
  
  Serial2.print(lux);
  Serial2.print("A ");
  Serial2.print(temperature);
  Serial2.print("B ");
  Serial2.print(phValue);
  Serial2.print("C ");
  Serial2.print(tdsValue);
  Serial2.print("D ");
  Serial2.print(distance_cm);
  Serial2.print("E ");
  Serial2.println();
  delay(50);
}
