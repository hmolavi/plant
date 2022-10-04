//  Hardware:   waterpump(relay)
//
//      Idea:   Turn the pump on every once in a while



// Ports
const int Water_Pump = 8;


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
  delay(threeHours);            // Turns on every 3 hours
}