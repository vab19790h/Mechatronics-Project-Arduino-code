// Define pin connections & motor's steps per revolution
const int dirPin = 12;
const int stepPin = 13;
const int stepsPerRevolution = 400;
int counter = 0;
String dir_Input; 
int step_Input = 0;



void setup()
{
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.begin(9600); delay(10);
  Serial.println();
  Serial.println("Starting..."); delay(10);
  change_Dir(); // Ask for direction Input
}


void loop()
{
  Serial.println("Enter steps (Max 200): ");
  while (Serial.available() == 0) {  
  }
  step_Input = Serial.parseInt();
  
  Serial.print("You have entered: ");
  Serial.println(step_Input);
  
  if(step_Input == 0)
  {
    step_Input = 1;
  }
  
  for (int x = 0; x < step_Input; x++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(2000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(2000);
    }
    Serial.print("steps Complete:");
    Serial.println(step_Input);
    int dummy_reader = Serial.parseInt();

    String change_Dir_again;
    Serial.print("Do you want to change direction (y/n): \n");
    while (Serial.available() == 0) {  
    }
    change_Dir_again = Serial.readString();
   
    if(change_Dir_again == "y\n")
    {
      change_Dir();
    }
    else
    {
      Serial.print("The direction is still " + dir_Input);
    }

    dummy_reader = Serial.parseInt();

}


void full_Displacement()
{
  // Set motor direction clockwise
  digitalWrite(dirPin, HIGH);

  // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    // printing on Serial Monitor
    Serial.print("x: ");
    Serial.println(x);
    Serial.println(counter);
    // printing on Serial Monitor
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  delay(5000); // Wait a second


  // Set motor direction clockwise
  digitalWrite(dirPin, LOW);

  // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    // printing on Serial Monitor
    Serial.print("x: ");
    Serial.println(x);
    Serial.println(counter);
    // printing on Serial Monitor
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  delay(5000); // Wait a second
}

void change_Dir()
{
  int good = 0;
  while(good == 0) 
  {
    Serial.println("Enter Direction (up/down): ");
    while (Serial.available() == 0) {  
    }
    dir_Input = Serial.readString();
    Serial.print("You have Entered: " + dir_Input);

    
    if(dir_Input == "up\n")
    {
      digitalWrite(dirPin, LOW);
      good = 1;
    }
    else if(dir_Input == "down\n")
    {
      digitalWrite(dirPin, HIGH);
      good = 1;
    }
    else
    {
      Serial.print("Try Again   ");
    }
  }
    
}
