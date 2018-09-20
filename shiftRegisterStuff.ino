

void updateIndicatorRegister() {

  //  indicatorArray Map:
  //--------- INPUT -------
  //  [0] negative input
  //  [1] input mode lat
  //  [2] input mode lon
  //  [3] input mode time
  //--------- NAV ---------
  //  [4] gps signal pulse (searching for GPS)
  //  [5] gps signal solid (found GPS)
  //  [6] distance indicator                                                                                                                                                                                                                                                                                                                                                                                                                                                                          bmmm
  //  [7]


  indicatorState[3] = negativeInput; //is already a boolean so don't need if/else statement

  if (inputMode == "lat" && globalState == "INPUT") {
    indicatorState[0] = true;
  } else {
    indicatorState[0] = false;
  }

  if (inputMode == "lon" && globalState == "INPUT") {
    indicatorState[1] = true;
  } else {
    indicatorState[1] = false;
  }

  if (inputMode == "time" && globalState == "INPUT") {
    indicatorState[2] = true;
  } else {
    indicatorState[2] = false;
  }


  if (globalState == "NAV") {
    if(navDisplayMode == "time"){
      indicatorState[1] = true;
      indicatorState[2] = false;
      indicatorState[3] = false;
    }else if(navDisplayMode == "distance"){
      indicatorState[1] = false;
      indicatorState[2] = true;
      indicatorState[3] = false;
    }
    
    if (!gpsFound) {
      indicatorState[4] = true;
      indicatorState[5] = false;
    } else {
      indicatorState[4] = false;
      indicatorState[5] = true;
    }
  } else {
    indicatorState[4] = false;
    indicatorState[5] = false;
  }


}
// taken from stack overflow question :
//https://arduino.stackexchange.com/questions/28395/how-to-convert-bool-array-to-byte
// provided by user "Lemabata" on August 16, 2016


byte BoolArrayToByte(bool boolArray[8])
{
  byte result = 0;

  for (int i = 0; i < 8; i++)
  {
    if (boolArray[i])
    {
      result = result | (1 << i);
    }
  }

  return result;
}

//a useless function, but it's a part of my code now so oh well.
//removed the latch pin changning state becuase when pushing
//data through multiple shift registers it would cause the data currently 
//in the register to be outputted to the pins. This caused a flash of all 
//leds and displays connected.

void sendit(int dPin, byte data) {
  //  digitalWrite(latchPin, 0);

  shiftOut(dPin, clockPin, data);
  //  digitalWrite(latchPin, 1);
  //  delay(50);

}


//not taken from Sparkfun tutorial:
//  - my segments were layed out in reverse order to the ones used in the tutorial
//  - I have common annode as opposed to common cathode, so a 1 = off and a 0 = on
//    because the segments ground themselves through the shift register.
//    This probably isn't the best way, but it works.
//  But just clarifying that this isn't unreferenced code - it is original.

void initShiftVals() {

  binaryNumbers[0] = 0b01010000; //0 numeral
  binaryNumbers[1] = 0b01011111; // 1 numeral
  binaryNumbers[2] = 0b00110010; // 2 numeral
  binaryNumbers[3] = 0b00010110; // 3 numeral
  binaryNumbers[4] = 0b00011101; // 4 numeral
  binaryNumbers[5] = 0b10010100; // 5 numeral
  binaryNumbers[6] = 0b10010000; // 6 numeral
  binaryNumbers[7] = 0b01011110; // 7 numeral
  binaryNumbers[8] = 0b00010000; // 8 numeral
  binaryNumbers[9] = 0b00011100; // 9 numeral

}

//void flashDisplays(float pin, int displays

//Notice the 0.1"f capacitor on the latchPin,
//if you have some flicker when the latch pin puls
//es you can use a capacitor to even it out.


//shiftOut function taken from
//http://weworkweplay.com/play/practical-guide-to-shift-registers/
//Article containing code was Written by Thijs Bernolet

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i = 0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i = 7; i >= 0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1 << i) ) {
      pinState = 1;
    }
    else {
      pinState = 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}

