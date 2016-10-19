#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;
RTC_Millis rtc;

// constants won't change. Used here to set a pin number :
const int ledPin =  6;      // the number of the LED pin

// Variables will change :
int ledState = LOW;             // ledState used to set the LED

unsigned long previousMillis = 0;        // will store last time LED was updated

 // constants won't change :
int interval = 0;           // interval at which to blink (milliseconds)

String dataString;
String changingrate;
double SumTotal = 0;
int Percentage = 0;

void setup() {
  Serial.begin(9600);
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  pinMode(A0, INPUT);

 rtc.begin(DateTime(F(__DATE__), F(__TIME__)));

 // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
     

 
  }
while (millis() < 3000) {
   emon1.current(A0, 60);             // Current: input pin, calibration.
double  Irms0 = emon1.calcIrms(1480);  // Calculate First Irms only

   }

}

void loop() {
  
  String dataString = "";

emon1.current(A0, 60);             // Current: input pin A0, calibration.
double  Irms1 = emon1.calcIrms(1480);  // Calculate First Irms only
  

//  ********Calculation****************
  
  emon1.current(A0, 60);             // Current: input pin A0, calibration.
double  Irms2 = emon1.calcIrms(1480);  // Calculate Second Irms only
 
  
  if (Irms2 < 0.17)
{
interval = 1000;  //T=1/f 1Hz 
changingrate = "Led 1Hz";
LEDrate ();   //blink led
Irms2 = 0;     //calibrate
Serial.println(changingrate);


}
else if (Irms2 > 0.17)
{
interval = 330;   //Time=1/3Hz =0.33 * 1000 = 330
changingrate = "Led 3Hz";
Serial.println(changingrate);
LEDrate ();   //blink led

}
  /*
   Calculating percentage: 
   Sum = Value 2 – Value 1 ……………(a)
   Percentage change = (Sum / Value 1) * 100 …………….(b)
   */
 
 Serial.print(Irms2);          // Irms
  //Serial.println(" A : second value");
  Serial.println(" A");
   
  //*********RTC*****************
  DateTime now = rtc.now();
    int nako1 = now.day();
    int nako2 = now.month();
    int nako3 = now.year();
    int nako4 = now.hour();
    int nako5 = now.minute();
    int nako6 = now.second();
    String FTime = String(nako1) + String("/") + String(nako2) + String("/") + String(nako3) + String(" ") + String(nako4) + String(":") + String(nako5) + String(":") + String(nako6);
    Serial.println (FTime);
    Serial.println(",");
    
 if (Irms1>Irms2){
  SumTotal = (Irms1 - Irms2);
  Percentage = (SumTotal/Irms2)*100;  
  }
  else if (Irms1<Irms2){
    SumTotal = (Irms2 - Irms1);
  Percentage = (SumTotal/Irms1)*100;
  } 
  else if (Irms1=Irms2){
    Percentage = 0;
  }

if (Percentage == 5) {
    //Serial.println ("Below 5% and I2<I1 therefore it dropped"); 
    
    File dataFile = SD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    
    
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.csv");
     LEDrate ();   //blink led
  }
  

  
 }
   
 //**********SD card************************
if ((Irms2>Irms1)||(Percentage == 5)) {  // upload only Irms2 if Percentagechange is above 5%
//if (Percentage == 5)//&&(Irms2>Irms1)) 
   {  
dataString += String(Irms2);
dataString += " A, Time: ";
dataString += FTime;
dataString += " , Led rate: ";
dataString += changingrate;

 File dataFile = SD.open("datalog.csv", FILE_WRITE);
// dataFile.println("Current: A  Time: ");
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
   
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.csv");
     LEDrate ();   //blink led
  }
   }  
if ((Irms2<Irms1)&&(Percentage == 5)) {
    //Serial.println ("Below 5% and I2<I1 therefore it dropped"); 
    
    File dataFile = SD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    
    
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.csv");
     LEDrate ();   //blink led
  }
  

  
 }

}


}

void LEDrate()
{
 
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      //Serial.println("Changing state");
      ledState = HIGH;
      
    } else {
      ledState = LOW;
     
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
  
}


