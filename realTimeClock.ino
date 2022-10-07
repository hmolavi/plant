//      https://create.arduino.cc/projecthub/sraykov08/data-logger-shield-sd-card-and-real-time-clock-5a439f

#include <Wire.h>
#include <ds3231.h>
#include <SPI.h> 
#include <SD.h>  

const char* filename = "time.txt";

File file;

struct ts t;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_INTCN);
  if (!SD.begin(10)) {
    Serial.println("Error : Push the reset button");
    for (;;); 
  }
  
  file = SD.open(filename, FILE_WRITE);
  /*----------------------------------------------------------------------------
    In order to synchronise your clock module, insert timetable values below !
    ----------------------------------------------------------------------------*/
  t.hour = 12;//hours
  t.min = 54;//minutes
  t.sec = 20;//seconds
  t.mday = 2;//date
  t.mon = 12;// month
  t.year = 2120;//year

  DS3231_set(t);
}

void loop() {
  DS3231_get(&t);
  Serial.print("Date : ");
  file.print("Date : ");
  file.flush();
  Serial.print(t.mday);
  file.print(t.mday);
  file.flush();
  Serial.print("/");
  file.print("/");
  file.flush();
  Serial.print(t.mon);
  file.print(t.mon);
  file.flush();
  Serial.print("/");
  file.print("/");
  file.flush();
  Serial.print(t.year);
  file.print(t.year);
  file.println();
  file.flush();
  Serial.print("\t Hour : ");
  file.print("\t Hour : ");
  file.flush();
  Serial.print(t.hour);
  file.print(t.hour);
  file.flush();
  Serial.print(":");
  file.print(":");
  file.flush();
  Serial.print(t.min);
  file.print(t.min);
  file.flush();
  Serial.print(".");
  file.print(".");
  file.flush();
  Serial.println(t.sec);
  file.println();
  file.flush();

  delay(1000);
}