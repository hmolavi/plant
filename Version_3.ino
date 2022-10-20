//  Hardware:   waterpump(relay),  moisuture sensor, 2 LEDs, 16X2 LCD
//
//      Idea:   Turn the pump only when the moisture drops below a described level
//              Show all the information through the 16x2 display
//              Implement a data logging mechanism using the arduino shield
//
//      Goals:  Water the plant after a double check (1 minuet interval )
//              Display water moisture, value and time
//              Run through display with a button
//              Save to sd card using another button
//

// Libraries

#include <millisDelay.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <SD.h> 
#include <SPI.h> 

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Ports
const int Water_Pump = 8;
const int Moisture_Sensor = A0;
const int LED_Drysoil = 10;               // red LED 10
const int LED_Fill_Water = 11;           // blue LED

// Global Constant values
const char *MONTH_NAMES[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
const int AIR_VAL = 620;                  // value when sensor is not in water
const int WATER_VAL = 310;                // value when sensor is in the water
const int DRY_SOIL_PERCENTAGE = 50;// what is interpreted as dry percentage
const char* filename = "time.txt";

// Global Values
bool dryState = false;
tmElements_t tm;
File file;

///////////////////////
//  Core Functions   //
///////////////////////
void setup() {
  // Pins 
  pinMode(Water_Pump, OUTPUT);
  pinMode(LED_Drysoil, OUTPUT);
  pinMode(LED_Fill_Water, OUTPUT);
  pinMode(Moisture_Sensor, INPUT);

  // Setting the relay off 
  digitalWrite(Water_Pump, LOW);

  Wire.begin();
    
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  
  file = SD.open(filename, FILE_WRITE);
  if (!SD.begin(10)) {
    Serial.println("Error : Push the reset button");
    while(true); 
  }

  // Takes care of all of time setting
  setTime();

  digitalWrite(LED_Drysoil, HIGH);
  delay(2000);
  digitalWrite(LED_Drysoil, LOW);

  Serial.println("Smart Watering");
  Serial.println("-------------------");
}

void loop() {
  
  RTC.read(tm);
  file.print(tm.Second);
  Serial.println(tm.Second);
  file.println();
  file.flush();
  digitalWrite(LED_Drysoil, HIGH);
  
  delay(500);                  // Turns on for 5 second 
  
  RTC.read(tm);
  file.print(tm.Second);
  Serial.println(tm.Second);
  file.println();
  file.flush();
  digitalWrite(LED_Drysoil, LOW);

  delay(500);                  // Turns on for 5 second 

}

//////////////////////////////
//  Implemented Functions   //
//////////////////////////////

void setTime(){
  bool parse=false;
  bool config=false;

  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      config = true;
    }
  }

  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(200);
  if (parse && config) {
    Serial.print("DS1307 configured Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  } else if (parse) {
    Serial.println("DS1307 Communication Error :-{");
    Serial.println("Please check your circuitry");
  } else {
    Serial.print("Could not parse info from the compiler, Time=\"");
    Serial.print(__TIME__);
    Serial.print("\", Date=\"");
    Serial.print(__DATE__);
    Serial.println("\"");
  }
}

bool getTime(const char *str){
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str){
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, MONTH_NAMES[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}

// Serial Output
void serialReport(int moisturePercentage, int moistureValue){
  
  String t_bckslsh = "\\", semicolon = ":", indent = "\t", t_moistureP = "\t\t Moisture%: ",t_moistureV = "\t Moisture Val: " ;
  //Serial.print(year+t_bckslsh+month+t_bckslsh+day+ indent + Hour+semicolon+minuet + t_moistureP+moisturePercentage + t_moistureV+moistureValue);
  Serial.println(indent + t_moistureP+moisturePercentage + t_moistureV+moistureValue);
}

// Turn on relay (turn on/off)
void waterValve(bool state){
  if( state ){
    digitalWrite(Water_Pump, HIGH);
  }
  else{
    digitalWrite(Water_Pump, LOW);
  }
}

// Turn on yellow LED (soil state)
void dryStateToggle(bool state){
  if (state){
    digitalWrite(LED_Drysoil, HIGH);
  }
  else{
    digitalWrite(LED_Drysoil, LOW);
  }
  dryState = state;
}

// Returns moisture percentage: p, value: v or both: b 
int calculateMoisture(char a) {
  
  const int Check_times = 20;

  int Moisture_Value[Check_times];

  for (int i = 0; i <= Check_times; i++) {
    Moisture_Value[i] = (analogRead(Moisture_Sensor));
  }

  int Moisture_Value_length = sizeof(Moisture_Value) / sizeof(Moisture_Value[0]);
  qsort(Moisture_Value, Moisture_Value_length, sizeof(Moisture_Value[0]), sort_desc);

  int Moisture_Percentage = map(Moisture_Value[int(Check_times/2)], AIR_VAL, WATER_VAL, 0, 100);
  
  //Moisture_Percentage -= 10;      // cause of the power shortage from supplying lcd display
  
  if (Moisture_Percentage <= DRY_SOIL_PERCENTAGE) {
    dryStateToggle(true);
  }
  else {
    dryStateToggle(false);
  }

  if ( a == 'p' ){
    return Moisture_Percentage;
  } else if ( a == 'v'){
    return Moisture_Value[int(Check_times/2)];
  } else {
    return Moisture_Percentage, Moisture_Value[int(Check_times/2)];
  }
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