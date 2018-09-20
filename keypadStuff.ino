bool allFalse[8] = {false, false, false, false, false, false, false, false};
bool allTrue[8] = {true, true, true, true, true, true, true, true};



void displayDebug() {
  for (int i = 0; i < 10; i++) {
    Serial.print(pressedKeys[i]);
    Serial.print(",");
  }
  //  Serial.print("  ");
  //  Serial.print("Lat: ");
  //  Serial.print(lat, 6);
  //  Serial.print("  ");
  //
  //  Serial.print("Lon: ");
  //  Serial.print(lon, 6);
  //  Serial.print("  ");

  Serial.print("in: ");
  Serial.print(inputMode);
  Serial.print("  ");

  Serial.print("H: ");
  Serial.print(arrivalHours);
  Serial.print("  ");

  Serial.print("M: ");
  Serial.print(arrivalMinutes);
  Serial.print("  ");

  Serial.print("t: ");
  Serial.print(hour(arrivalTime));
  Serial.print(" ");

  Serial.println();

}

void pushDataToShifts() {
  digitalWrite(latchPin, 0);

  if (globalState == "INPUT") {
    sendit(dataPin, BoolArrayToByte(mainDisplay)); //change this to send all false
    //sendit(dataPin, BoolArrayToByte(allTrue));
    sendit(dataPin, BoolArrayToByte(indicatorState)); //send data to led pins
    sendit(dataPin, binaryNumbers[pressedKeys[inputCounter]]); // data to SEG 2
    sendit(dataPin, binaryNumbers[pressedKeys[inputCounter - 1]]); //data to SEG 1

  } else if (globalState == "NAV") {

    sendit(dataPin, BoolArrayToByte(mainDisplay));
    sendit(dataPin, BoolArrayToByte(indicatorState));

    //send time percentage
    if (!arrived) {
      if (navDisplayMode == "time") {
        sendit(dataPin, binaryNumbers[timePercentDigits[1]]);
        sendit(dataPin, binaryNumbers[timePercentDigits[0]]);
      } else if (navDisplayMode == "distance") {
        sendit(dataPin, binaryNumbers[distPercentDigits[1]]);
        sendit(dataPin, binaryNumbers[distPercentDigits[0]]);
      }
    } else {
      if (alertState == 1) {
        sendit(dataPin, BoolArrayToByte(allTrue));
        sendit(dataPin, BoolArrayToByte(allTrue));
      } else {
        sendit(dataPin, BoolArrayToByte(allFalse));
        sendit(dataPin, BoolArrayToByte(allFalse));
      }
    }

    //  sendit(dataPin, BoolArrayToByte(allTrue)); // push nothing through led segs
    //  sendit(dataPin, BoolArrayToByte(allTrue));



  }
  digitalWrite(latchPin, 1);
}

