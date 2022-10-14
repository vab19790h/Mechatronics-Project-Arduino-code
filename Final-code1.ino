#include <HX711_ADC.h>

#include <DFRobot_HX711.h>
//pins:

#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//pins:
const int HX711_dout = 2; //mcu > HX711 dout pin
const int HX711_sck = 3; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

static boolean newDataReady = 0;
const int serialPrintInterval = 0; //increase value to slow down serial print activity

  
// Entire code above this row is for load cell

// Code bellow is for Stepper
const int dirPin = 12; const int stepPin = 13; const int stepsPerRevolution = 400; const int Max_steps = 1385;
int counter = 0; String yes_no; int step_Input = 0; 


void setup() {
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT); pinMode(dirPin, OUTPUT);
  Serial.begin(9600); delay(10); Serial.println(); Serial.println("Starting..."); delay(10);


   LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  
  calibrationValue = 696.0; // uncomment this if you want to set the calibration value in the sketch
  
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }

  Serial.println("IMPORTANT: make sure the stage is all the way up at the zero (0) location before test starts!! \n"); delay(3000); 
  
}








void loop() {

  if (Serial.available() > 0) {
    yes_no = Serial.readString();
    Serial.println(yes_no);
  }

  LoadCell.tareNoDelay();
  
  load_Cell_Code(); delay(1000);
   if (yes_no == "y\n")
  {
    run_test(); // Ask to initiate the test--stepper motor will be operated
  }

  

}









void run_test()
{
  
 
    digitalWrite(dirPin, HIGH); // This will move the actuator stage downward.
    
    for (int x = 0; x < Max_steps; x++)
    {
      load_Cell_Code();
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(2000);
      load_Cell_Code();
      digitalWrite(stepPin, LOW);
      delayMicroseconds(2000);
    }
    
     Serial.println("Reach at the lowest most position. \n"); delay(5000);
     Serial.println("Starting to go back to the Initial position in 5 seconds \n"); delay(5000);

     digitalWrite(dirPin, LOW); // This will move the actuator stage upward.

     for (int x = 0; x < Max_steps; x++)
    {
      load_Cell_Code();
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(2000);
      load_Cell_Code();
      digitalWrite(stepPin, LOW);
      delayMicroseconds(2000);
    }
    
     Serial.println("Reach Back to original location. \n"); delay(5000);  
     yes_no = "";
}




void load_Cell_Code()
{

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      Serial.print("DO you want to run the test? (y/n):    ");
      newDataReady = 0;
      t = millis();
    }
  }

  
}
