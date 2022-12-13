#include <SD.h>

const int chipSelect = 10;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
    // open the file for writing:


  writeToCard();

  readFromCard();

}

void loop() {

}

void writeToCard(){
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile) {
    Serial.println("Writing to datalog.txt...");
    dataFile.println("Hello, world");
    dataFile.close();
    Serial.println("Done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("Error opening datalog.txt");
  }

  dataFile.close();
}

void readFromCard(){
    // open the file for reading:
  File dataFile = SD.open("datalog.txt");

  if (dataFile) {
    // read the file one character at a time:
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
    Serial.println();
  } else {
    // if the file didn't open, print an error:
    Serial.println("Error opening datalog.txt");
  }
}
