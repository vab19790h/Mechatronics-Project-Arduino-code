
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

  
//   ///  ////   /////  ///////   Entire code above this row is for load cell   //   ///  ////   /////  ///////

int variable = 0;
int sensorPin = A0;

//   ///  ////   /////  ///////   Code bellow is for Stepper    //   ///  ////   /////  ///////

const int dirPin = 12; const int stepPin = 13; const int stepsPerRevolution = 400; const int Max_steps = 1385; String yes_no; 


void setup() {
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT); pinMode(dirPin, OUTPUT);

  
   LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue = 696.0; // calibration value (see example file "Calibration.ino")

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);

  Serial.begin(9600); delay(100); Serial.println("Connect your device to the Bluetooth"); 
  
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }

  
  Serial.println("IMPORTANT: Stage must be at zero (0) location before test starts!!"); 

  
}


void loop() {
  
  variable = analogRead(sensorPin);
  if(variable > 50)
  {
    
    Serial.print("Run the test? (y/n):,");
    LoadCell.tareNoDelay();
    load_Cell_Code();  delay(500);
    
    if (Serial.available() > 0) 
    {
      yes_no = Serial.readString();
      Serial.println(yes_no);
    }

    delay (3000);
    
     if (yes_no == "y\n")
    {
      run_test(); // Ask to initiate the test--stepper motor will be operated
    }
  }

  
}


void run_test()
{
  
    digitalWrite(dirPin, HIGH); // This will move the actuator stage downward.
    
    for (int x = 0; x < Max_steps; x++)
    {
      load_Cell_Code_2(); 
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(5000);
      load_Cell_Code_2(); 
      digitalWrite(stepPin, LOW);
      delayMicroseconds(5000);
    }
    
     Serial.println("Reach at the lowest most position. \n"); load_Cell_Code_2(); 
     Serial.println("Starting to go back to the Initial position in 5 seconds \n"); load_Cell_Code_2(); delay(4000);
    
     digitalWrite(dirPin, LOW); // This will move the actuator stage upward.
     load_Cell_Code_2(); delay(150);
     load_Cell_Code_2(); delay(150);
     load_Cell_Code_2(); delay(150);
     load_Cell_Code_2(); delay(150);
     load_Cell_Code_2(); delay(150);
     load_Cell_Code_2(); delay(150);
     load_Cell_Code_2(); delay(145);     
     for (int x = 0; x < Max_steps; x++)
    {
      load_Cell_Code_2(); 
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(5000);
      load_Cell_Code_2(); 
      digitalWrite(stepPin, LOW);
      delayMicroseconds(5000);
    }
    
     Serial.println("Returned Back to the Initial Position. \n"); delay(5000);  
     yes_no = "";
}




void load_Cell_Code_2()
{

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Test is running,");
      Serial.print("Force:,");
      Serial.print(i);
      Serial.print(",");
      Serial.println();
      newDataReady = 0;
      t = millis();
    }
  } 
}
void load_Cell_Code()
{

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Force:,");
      Serial.print(i);
      Serial.print(",");
      Serial.println();
      newDataReady = 0;
      t = millis();
    }
  } 
}
