/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read new NUID from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to the read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the type, and the NUID if a new card has been detected. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include "RTClib.h"
#include <Wire.h>

#define  RST_PIN  9    // Configurable, see typical pin layout above
#define  SS_PIN   10    // Configurable, see typical pin layout above
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

RTC_DS1307 RTC;

char daysOfTheWeek[7][12] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

int ir1 = 0;
int ir2 = 1;

int buzzer = 7;  // speaker or buzzer on pin 8
int led_pos1 = 4; // green LED on pin 7
int led_neg1 = 5; // red LED on pin 6

int led_pos2 = 3; // green LED on pin 1
int led_neg2 = 8; // red LED on pin 2

String UID_tagA = "16025282126";  // UID of tag that we are using
String UID_tagB = "24094194128";  // UID of tag that we are using

short UIDs_No = 1;
short UID_No = 2;

// Init array that will store new NUID 
int nuidPICC[4];
String readTag = ""; 

// Declaration of the functions
int getID ();
boolean checkTag();  // check if tag is unknown
//void StoreData();  // store data to microSD
void irsensor(); // detect first tool is present or not
void timing();

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  Wire.begin();
  RTC.begin ();
   if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
   }
  
  pinMode (0, INPUT);
  pinMode(1, INPUT);
  
  pinMode(led_pos1, OUTPUT);
  pinMode(led_neg1, OUTPUT);

  pinMode(led_pos2, OUTPUT);
  pinMode(led_neg2, OUTPUT);
  pinMode(buzzer, OUTPUT); 
}
 
void loop() 
{
 int succesRead = getID(); // read RFID tag
 if(succesRead == 1) // if RFID read was succesful
  {
      timing ();
    
       if (checkTag()) // if tag is known, store data
       {
          Serial.println("Known Tag");
          Serial.println("");
          irsensor();    
       }
  
       else if (!checkTag())
       {
          Serial.println(" Unknown Tag");
       
           digitalWrite(7, HIGH);
           delay (10000);
           digitalWrite(7, LOW);
       }             
     }
  }
  

// FUNCTIONS
void irsensor()
{
  int sensorValue = digitalRead(0);
      int sensorValue1 = digitalRead(1);
      if(sensorValue ==1 && sensorValue1 ==1 )
        {
          // print out the value you read:
            Serial.println ("Tools Taken"); 
            timing ();   
            Serial.print("1st Tool - ");     
            Serial.print(sensorValue);
            Serial.print("   ");
            Serial.print("2nd Tool - ");
            Serial.print(sensorValue1);
            Serial.println("\n");
           // delay(10);        // delay in between reads for stability
            digitalWrite(4, HIGH);
            digitalWrite(3, HIGH);
            digitalWrite(5, LOW);
            digitalWrite(8, LOW);
            timing ();
        }
     else if (sensorValue != 1 && sensorValue1 == 1)
        {
            Serial.println ("Tools Taken "); 
            //timing ();        
            Serial.print("1st Tool - ");     
            Serial.print(sensorValue);
            Serial.print("   ");
            Serial.print("2nd Tool - "); 
            Serial.print(sensorValue1);
            Serial.println("\n");
                  
            digitalWrite(5, HIGH);
            digitalWrite(8, LOW);
             digitalWrite(4, LOW);
             digitalWrite(3, HIGH);
             timing();
    
        }
       else if (sensorValue == 1 && sensorValue1 != 1)
        {
            Serial.println ("Tools Taken"); 
            timing ();        
            Serial.print("1st Tool - ");     
            Serial.print(sensorValue);
            Serial.print("   ");
            Serial.print("2nd Tool - "); 
            Serial.print(sensorValue1);
            Serial.println("\n");
            
            digitalWrite(5, LOW);
            digitalWrite(8, HIGH);
             digitalWrite(4, HIGH);
             digitalWrite(3, LOW);
                timing();
    
        }
       else if (sensorValue == 0 && sensorValue1 == 0)
        {
            Serial.println ("Tools Taken"); 
            timing ();        
            Serial.print("1st Tool - ");     
            Serial.print(sensorValue);
            Serial.print("   ");
            Serial.print("2nd Tool - "); 
            Serial.print(sensorValue1);
            Serial.println("\n");
            
            digitalWrite(5, HIGH);
            digitalWrite(8, HIGH);
             digitalWrite(4, LOW);
             digitalWrite(3, LOW);
                timing();
    
        }
           
 }
 
boolean checkTag() // check if tag is unknown
 {
    if(readTag == UID_tagA){UIDs_No = 1; return true;}
    else if(readTag == UID_tagB){UIDs_No = 2; return true;}
    else {return false;}
 }

int getID()
{
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return 0;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return 0;
    
  readTag = "";
    // Store NUID into nuidPICC array
    for (int i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
       readTag = readTag+String(nuidPICC[i],DEC);
    }
     Serial.print("UID Tag NO : "); 
     Serial.print(readTag);
     Serial.println(" "); 

  // Halt PICC
  rfid.PICC_HaltA();
  return 1;

  /*// Stop encryption on PCD
  rfid.PCD_StopCrypto1();*/
  
}

void timing ()
{
  DateTime now = RTC.now();
  Serial.print(now.day(), DEC);
  Serial.print("-");
  Serial.print(now.month(), DEC);
  Serial.print("-");
  Serial.print(now.year(), DEC);
  Serial.print(" ");
  //Serial.print(daysOfTheWeek[now.daysOfTheWeek()]);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.println(now.second(), DEC);
  delay(1000);
  
}


