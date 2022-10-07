//  Hardware:   waterpump(relay),  moisuture sensor
//
//      Idea:   Turn the pump only when the moisture drops below a described level
//              Waters the plant in 5 second intervals 

/
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

// Moisture Sensitivity
const int Air_Value = 620;
const int Water_Value = 310;

const int Dry_Soil_Moisture_Percentage = 50;

// Time Variable
long int threeHours = 3*60*60*1000;

// Functions
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

void TurnWaterOn(){
  return;
}

// qsort requires you to create a sort function
int sort_desc(const void *cmp1, const void *cmp2){
  // Need to cast the void * to int *
  int a = *((int *)cmp1);
  int b = *((int *)cmp2);
  // The comparison
  return a > b ? -1 : (a < b ? 1 : 0);
  // A simpler, probably faster way:
  //return b - a;
}


// Take a median of multiple reads and returns a %
int Calculate_Moisture() {

  /// Change this value if you want to change the # of sensor reads 
  const int Check_times = 20;

  int Moisture_Value[Check_times];

  for (int i = 0; i <= Check_times; i++) {
    Moisture_Value[i] = (analogRead(Moisture_Sensor));
  }

  int Moisture_Value_length = sizeof(Moisture_Value) / sizeof(Moisture_Value[0]);
  qsort(Moisture_Value, Moisture_Value_length, sizeof(Moisture_Value[0]), sort_desc);

  int Moisture_Percentage = map(Moisture_Value[int(Check_times/2)], Air_Value, Water_Value, 0, 100);
  
  //Moisture_Percentage -= 10;      // cause of the power shortage from supplying lcd display
  
  if (Moisture_Percentage <= Dry_Soil_Moisture_Percentage) {
    digitalWrite(LED_Drysoil, HIGH);
  }
  else {
    digitalWrite(LED_Drysoil, LOW);
  }

  return Moisture_Percentage;
}