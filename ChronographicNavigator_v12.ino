//    Liam Ball Physical Computing
//    October 2017
//    
//
//


#include <Time.h>
#include <TimeLib.h>
#include <Keypad.h>
#include "TinyGPS++.h"
#include <SoftwareSerial.h>


//----------------------------------KEYPAD SETUP------------------
//Taken from https://playground.arduino.cc/Code/Keypad, the example  
//  for the Keypad library.
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

byte rowPins[ROWS] = {15, 16, 17, 18}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {3, 2, 14}; //connect to the column pinouts of the keypad

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//----------------------------------------------------------------------
//----------------GLOBAL STATE STUFF --------------

String globalState; //can be

bool indicatorState[8];
bool mainDisplay[8];

bool arrived = false;



//------------------------------------------------//

//---------------------------GPS STUFF -----------------------

TinyGPSPlus gps;
SoftwareSerial ss(8, 9);


bool gpsFound = false;

bool navStarted = false;

bool dStartSet = false;
boolean accTimeSet = false;

float latUnit;
float lonUnit;


float accuracyTimeStart;
float accuracyTime = 5000;

float dStart;

float artGalLat = -36.851411;
float artGalLon = 174.766228;

//currently at AUT 
float testCurrentLat = -36.853449;
float testCurrentLon = 174.765840;

//currently around surplustronics
float testStartLat = -36.857474;
float testStartLon = 174.761838;


//------------------ NAVIGATION STUFF --------------

float currentDistance;
float startDistance;


int timePercentDigits[2];
int distPercentDigits[2];

String navDisplayMode = "time"; //either "time" or "distance"



//----------------- TIME STUFF --------------------

time_t arrivalTime;
float arrivalHours;
float arrivalMinutes;

float startNavigateMillis;

float travelTime;
float timeToArrive;

float alertFlashUp = 100;
float alertFlashDown = 250;
int alertState = 1;
float lastAlertChange = 0;


// ----------------------------------SHIFT REGISTER SETUP-------------------
byte data;
byte binaryNumbers[10];

int latchPin = 12;
int clockPin = 11;
int dataPin = 13;
int dataPin2 = 7;

int navKey;
//----------------------------------------------------------------------------

//---------------------------------------------------------------------

//list of exponents for 10 integers into long/lat
float exponentsLon[] = {10e1, 10e0, 10e-1, 10e-2, 10e-3, 10e-4, 10e-5, 10e-6, 10e-7, 10e-8};
float exponentsLat[] = {10e0, 10e-1, 10e-2, 10e-3, 10e-4, 10e-5, 10e-6, 10e-7, 10e-8, 10e-9};





//-----------------------------------------------------------------------
//-----------------------------------SETUP()---------------------------
void setup() {

  Serial.begin(9600);
  Serial.println("Serial debug");
  ss.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(6, OUTPUT);

  initShiftVals(); //INIT BINARY DATA ARRAY

  globalState = "INPUT";
  //    global states:
  //      "INPUT" - entering long, lat, and time later, use inputMode for differencing
  //      "NAV"   - calculate current pos and distance and stuff and update main segment displays
  //
  //

  updateIndicatorRegister();
  pushDataToShifts();

}

//------------- INPUT STUFF ------------
int pressedKeys[10];    //array of keys entered with keypad
int stageCounter = 0;   //not used in current version
int inputCounter = 0;   //current digit position

float latDest;
float lonDest;
String inputMode = "lat"; //either "lat" or "lon" or "time" - add in later

int latDigits = 7; //actually 8 but zero indexed
int lonDigits = 7;
int timeDigits = 3; //again, acually 4

bool negativeInput = false;


//------------------------------------------

float brightness = 0;
float angle = 0;
float pulseIncrement = 0.1;

void loop() {
  //------------------------GENERAL UPDATE -------------

  //pwm sin angle update
  angle += pulseIncrement;
  brightness = map(degrees(sin(angle)), -80, 80, 10, 250);
  analogWrite(6, brightness);
  //    analogWrite(6, map(degrees(sin(angle)), -80, 80, 0, 255));
  //  Serial.print(" ");
  //  Serial.println();


  if (globalState == "INPUT") {
    updateIndicatorRegister();
    //pushDataToShifts();
    //-----------------------------------------------KEYPAD CHAR GETTING-------------------
    char key = keypad.getKey();

    if (key != NO_KEY) {
      if (key == '#') {

        Serial.println("<<");

        if (inputCounter > 0) {

          inputCounter -= 1;
          pressedKeys[inputCounter] = 0;
          displayDebug();
          updateIndicatorRegister();
          pushDataToShifts();
        }


      } else if (key == '*') {
        //      Serial.println("*");

        if (negativeInput) {
          negativeInput = false;
        } else {
          negativeInput = true;
        }
        updateIndicatorRegister();
        pushDataToShifts();

        Serial.print("negativeInput = ");
        Serial.println(negativeInput);

      } else {
        String a = String(key);
        int num = a.toInt();


        //remove this to stop looping overwrite
        if (inputCounter > 9) {
          inputCounter = 0;
        }


        pressedKeys[inputCounter] = num;
//------------------------------------------------------------------------------------------------
/*
         for(int i = 0; i < 8; i ++){
//          Serial.print("i = ");
//          Serial.println(i);
          if(i == num){
            mainDisplay[i] = true;
//            Serial.println("True");
          }else{
            mainDisplay[i] = false;
//            Serial.println("False");
          }
        }
        */
//------------------------------------------------------------------------------------------------


        //      sendit(dataPin, 0b01010101);

        //        sendit(dataPin, BoolArrayToByte(indicatorState)); //send data to led pins
        //        sendit(dataPin, binaryNumbers[pressedKeys[inputCounter]]); // data to SEG 2
        //        sendit(dataPin, binaryNumbers[pressedKeys[inputCounter - 1]]); //data to SEG 1

        pushDataToShifts();

        inputCounter += 1;


        displayDebug();



        delay(50);

      }
    }
    //-------------------------------------------------------------------------------------


    //ADD IF STATEMENT TO CHECK CURRENT DEVICE STATE - USE THIS IN SETUP

    //get lat
    if (inputMode == "lat" && inputCounter > latDigits) {
      //        get and set latitude
      float latSum = 0;
      for (int i = 0; i < 10; i ++) {
        latSum += pressedKeys[i] * exponentsLat[i];
        pressedKeys[i] = 0;
      }
      if (negativeInput) {
        latSum *= -1;
      }

      latDest = latSum;

      //reset all counters and negative number bool
      inputMode = "lon";
      inputCounter = 0;
      negativeInput = false;

      updateIndicatorRegister();
      pushDataToShifts();

    } else if (inputMode == "lon" && inputCounter > lonDigits) {
      //      get and set longitude
      float lonSum = 0;
      for (int i = 0; i < 10; i++) {
        lonSum += pressedKeys[i] * exponentsLon[i];
        pressedKeys[i] = 0;
      }
      if (negativeInput) {
        lonSum *= -1;
      }
      lonDest = lonSum;



      //reset all counters and negative number bool
      inputMode = "time";
      inputCounter = 0;
      negativeInput = false;

      updateIndicatorRegister();
      pushDataToShifts();

    } else if (inputMode == "time" && inputCounter > timeDigits) {

      //      get and set time  - HH:MM - that you need to arrive in
      arrivalHours = (pressedKeys[0] * 10) + pressedKeys[1];
      arrivalMinutes = (pressedKeys[2] * 10) + pressedKeys[3];

      arrivalTime = (arrivalHours, arrivalMinutes);

      //travelTime is the time in milliseconds
      travelTime = timeToMillis(arrivalHours, arrivalMinutes);

      //the value millis() will be equal to when the unit should arrive
      timeToArrive =  travelTime + millis();




      updateIndicatorRegister();
      pushDataToShifts();
      displayDebug();


      startNavigateMillis = millis();
      globalState = "NAV";
    }
  } else if (globalState == "NAV") {
    //NAVIGATION LOOP
    updateMainDisplay();
    updateIndicatorRegister();
    pushDataToShifts();

    char key = keypad.getKey();

    if(arrived){

      float currentAlertTime;

        if(alertState == 1){
          currentAlertTime = alertFlashUp;
        }else{
          currentAlertTime = alertFlashDown;
        }
      if(millis() - lastAlertChange > currentAlertTime){
        if(alertState == 1){
          alertState = 0;
          
        }else{
          alertState = 1;
        }

        lastAlertChange = millis();
      }
    }

    if (key != NO_KEY) {
        if(key == '*'){
          Serial.print("Changed display mode to ");
          if(navDisplayMode == "time"){
            navDisplayMode = "distance";
            
          }else if(navDisplayMode == "distance"){
            navDisplayMode = "time";
          }
          Serial.println(navDisplayMode);
          
        }else if(key == '#'){
          
        }else{
        String a = String(key);
        int num = a.toInt();


//        navKey = num;
        }


    }
    

    //-------------------------------------------------------------------
    while (ss.available() > 0) {
      gps.encode(ss.read());  //'feed' gps data stream encoder
    }
    //-------------------------------------------------------------------


    if (gps.location.isUpdated()) {
      Serial.println("GPS found");
      latUnit = gps.location.lat();
      lonUnit = gps.location.lng();

      if (!gpsFound) {
        gpsFound  = true;
      }
      if(!navStarted){
        startDistance = distanceBetween(latUnit, lonUnit, latDest, lonDest);
        navStarted = true;
      }

      //update current distance
      currentDistance = distanceBetween(latUnit, lonUnit, latDest, lonDest);

      


    }



  }


}


