

void updateMainDisplay() {
  //  mainDisplay Array Map:
  //--------- RINGS -------
  //  [0] last
  //  [2] lastThird ring
  //  [4] first third ring
  //  [6] inner ring
  //--------- UNIT POS --------- 

  //  [1] unit pos 1
  //  [3] unit pos 2
  //  [5] unit pos 3                                                                                                                                                                                                                                                                                                                                                                                                                                                                   bmmm
  //  [7] unit pos 4


  int timePercent = getTimePercent();
  int distPercent = getDistPercent(true);   //if argument is true, then uses test coords

  //update digit arrays for the LED segment displays
  timePercentDigits[0] = (timePercent / 10) % 10;
  timePercentDigits[1] = (timePercent / 1)  % 10;

//  Serial.print("Time percentage = ");
//  Serial.println(timePercent);


  int currentRing = (int)map(timePercent, 0, 100, 4, -1);
  currentRing = constrain(currentRing, 0, 3);

  int currentPos = (int) map(distPercent, 0, 100, 6, 2);
  currentPos = constrain(currentPos, 3, 6);

  //  Serial.println(currentRing);

  

  if (timePercent >= 100){
  mainDisplay[0] = false;
    mainDisplay[2] = false;
    mainDisplay[4] = false;
    mainDisplay[6] = false;
    arrived = true;
  } else {

    switch (currentRing) {
      case 0:
        mainDisplay[0] = true;
        mainDisplay[2] = false;
        mainDisplay[4] = false;
        mainDisplay[6] = false;
        break;
      case 1:
        mainDisplay[0] = false;
        mainDisplay[2] = true;
        mainDisplay[4] = false;
        mainDisplay[6] = false;
        break;
      case 2:
        mainDisplay[0] = false;
        mainDisplay[2] = false;
        mainDisplay[4] = true;
        mainDisplay[6] = false;
        break;
     case 3:
        mainDisplay[0] = false;
        mainDisplay[2] = false;
        mainDisplay[4] = false;
        mainDisplay[6] = true;
        break;

    }
  }

  //--------- UNIT POS --------- 
  //  [3] unit pos 1
  //  [4] unit pos 2
  //  [5] unit pos 3
  //  [6] unit pos 4                                                                                                                                                                                                                                                                                                                                                                                                                                                                   bmmm
  //  [7]

  if (distPercent >= 100){
  mainDisplay[1] = false;
    mainDisplay[3] = false;
    mainDisplay[5] = false;
    mainDisplay[7] = false;
    arrived = true;
  } else {

    switch (currentPos) {
      case 3:
        mainDisplay[1] = true;
        mainDisplay[3] = false;
        mainDisplay[5] = false;
        mainDisplay[7] = false;
        break;
      case 4:
        mainDisplay[1] = false;
        mainDisplay[3] = true;
        mainDisplay[5] = false;
        mainDisplay[7] = false;
        break;
      case 5:
        mainDisplay[1] = false;
        mainDisplay[3] = false;
        mainDisplay[5] = true;
        mainDisplay[7] = false;
        break;
      case 6:
        mainDisplay[1] = false;
        mainDisplay[3] = false;
        mainDisplay[5] = false;
        mainDisplay[7] = true;
        break;

    }
  }


}

// I added a test argument to this function because I had to test
// that it was working so much, and without using the acutal GPS
// module.

int getDistPercent(bool test) {
  float distP;
  if (!test) {
    distP = map(currentDistance, startDistance, 0, 0, 100);
  } else {
    float fullTestDistance = distanceBetween(testStartLat, testStartLon, latDest, lonDest);
    float currTestDistance = distanceBetween(testCurrentLat, testCurrentLon, latDest, lonDest);

    distP = map(currTestDistance, fullTestDistance, 0, 0, 100);

  }
  return (int) distP;

}

int getTimePercent() {
  float timePercentage = map(millis(), startNavigateMillis, timeToArrive, 0, 100);
  return (int)timePercentage;
}

float timeToMillis(int hours, int mins) {

  float  hMillis = (float)hours * 60 * 60 * 1000;

  float  mMillis = (float)mins * 60 * 1000;

  float totalMillis = hMillis + mMillis;

  return totalMillis;

}
