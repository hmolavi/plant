//  Hardware:   waterpump(relay),  moisuture sensor
//
//      Idea:   Turn the pump only when the moisture drops below a described level
//              Waters the plant in 5 second intervals 

// Ports
const int Water_Pump = 8;
const int Moisture_Sensor = A0;


// Moisture Sensitivity
const int Air_Value = 620;
const int Water_Value = 310;

const int dryPercentage = 50;

// Functions
void setup() {
  pinMode(Water_Pump, OUTPUT);
  digitalWrite(Water_Pump, LOW);
}

void loop() {
  int moisture = Calculate_Moisture();

  if (moisture < dryPercentage ){
    digitalWrite(Water_Pump, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5*1000);                  // Turns on for 5 second 
    
    digitalWrite(Water_Pump, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

int Calculate_Moisture() {
  /// Takes a mean average of moisture levels read from the sensor
  /// Takes 2 second to return value

  const int Check_times = 20;
  int Moisture_Value, Moisture_Percentage;

  for (int i = 0; i <= Check_times; i++) {
    Moisture_Value += analogRead(Moisture_Sensor);
    delay(100);
  }
  Moisture_Value /= Check_times;
  Moisture_Percentage = map(Moisture_Value, Air_Value, Water_Value, 0, 100);
  
  //Moisture_Percentage -= 10;      // cause of the power shortage from supplying lcd display
  
  return Moisture_Percentage;
}

