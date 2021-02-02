

#if !defined(UBRR1H)
#include <DFPlayerMini_Fast.h>
#include "SoftwareSerial.h"
SoftwareSerial mySerial(10, 11); //RX, TX
#endif

DFPlayerMini_Fast myMP3;

//Time for sensor calibration
int calibrationTime = 3;       
 
//low impulse time from sensor 
long unsigned int lowIn;        
 
//time until motion has ended 
long unsigned int pause = 1000; 
 
boolean lockLow = true;
boolean takeLowTime; 
 
int pirPin = 2;    //PIR Sensor 
int ledPin = 13;    //Led 
 
 
/////////////////////////////
//SETUP
void setup(){
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  #if !defined(UBRR1H)
  mySerial.begin(9600);
  myMP3.begin(mySerial);
#else
  Serial.begin(9600);
  myMP3.begin(Serial1);
#endif

 Serial.println("Volume set");
  myMP3.volume(30);
  delay(20);
 
  //Calibrating the sensor set to 30seconds 
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println("calibrated");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  }
 
////////////////////////////
//LOOP
void loop(){
 int randomInt = random(0,12);
 
     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led lights up when the sensor was activated 
       if(lockLow){ 

       
         //wait until sensor is LOW before continue/Change the delay to more if you want it to wait longer. 
         lockLow = false;           
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec");
         myMP3.play(randomInt);
         delay(3000);
         }        
         takeLowTime = true;
       }
 
     if(digitalRead(pirPin) == LOW){      
       digitalWrite(ledPin, LOW);  //led shows the PIR state 
 
       if(takeLowTime){
        lowIn = millis();          //saving time from PIR activation until end 
        takeLowTime = false;       //and that it starts from LOW 
        }
       //if the sensor stays LOW for more than the pause
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){ 
           //makes sure this block of code is only executed again after
           //a new motion sequence has been detected
           lockLow = true;                       
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           myMP3.pause();
           delay(50);
           }
       }
  }
