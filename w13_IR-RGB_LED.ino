/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

// the setup routine runs once when you press reset:
void setup() {
  //initialize serial communication at 9600 bits per second:
  //Serial.begin(9600);
  pinMode(PA0,OUTPUT);
  pinMode(PA1,OUTPUT);
  pinMode (PA2,INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = digitalRead(PA2);
  // print out the value you read:
  //Serial.println(sensorValue);
  delay(10);        // delay in between reads for stability


  if(sensorValue ==1)
  {
   //LED(50);
  digitalWrite(PA1, LOW);
  digitalWrite(PA0,HIGH);
  }

  else
  {
    digitalWrite(PA1,HIGH);
    digitalWrite (PA0, LOW);
    
    }
  
  
}
