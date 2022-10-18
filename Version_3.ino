//  Hardware:   waterpump(relay),  moisuture sensor, 2 LEDs, 16X2 LCD
//
//      Idea:   Turn the pump only when the moisture drops below a described level
//              Show all the information through the 16x2 display
//              Implement a data logging mechanism using the arduino shield
//
//      Goals:  Have the chip run 
//

// Libraries
#include <millisDelay.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Ports
const int Water_Pump = 8;
const int Moisture_Sensor = A0;
const int LED_Drysoil = 10;               // red LED
const int LED_Fill_Water = 11;           // blue LED


// Global Values
bool dryState = false;

// Core Functions 
void setup() {
  pinMode(Water_Pump, OUTPUT);
  digitalWrite(Water_Pump, LOW);
  delay(3000);
}

void loop() {

  digitalWrite(Water_Pump, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(5*1000);                  // Turns on for 5 second 
  
  digitalWrite(Water_Pump, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delay(threeHours);              // Turns on every 3 hours
}


// Implemented Functions

void waterValve(){
  return nullptr;
}

void dryStateToggle(bool state){
  
  if (state){
    digitalWrite(LED_Drysoil, HIGH);
  }
  else{
    digitalWrite(LED_Drysoil, LOW);
  }

  dryState = state;
}

int Calculate_Moisture() {
  /// Takes a me average of moisture levels read from the sensor
  
  /// Change this value if you want to change the # of sensor reads 
  const int Check_times = 20;

  int Moisture_Value[Check_times], Moisture_Percentage;

  for (int i = 0; i <= Check_times; i++) {
    Moisture_Value.append(analogRead(Moisture_Sensor));
  }
  Moisture_Value /= Check_times;
  Moisture_Percentage = map(Moisture_Value, Air_Value, Water_Value, 0, 100);
  
  //Moisture_Percentage -= 10;      // cause of the power shortage from supplying lcd display
  
  if (Moisture_Percentage <= Dry_Soil_Moisture_Percentage) {
    digitalWrite(LED_Drysoil, HIGH);
  }
  else {
    digitalWrite(LED_Drysoil, LOW);
  }

  return Moisture_Percentage;
}

