# ChronographicNavigator

The Chronographic Navigator is a handheld device that allows the user to experience time as a
physical area. The user inputs a location and the time they want to arrive, and the Navigator will
display a shrinking circular area around their destination they must be inside of to arrive on time.
This is an idea I had a couple years ago while I was walking to school and checking the time. I
wondered where I would have to be in order to make it on time
# INPUT MODE:
First you enter the coordinates of the target location.
The leftmost column of indicator squares shows what value you are entering:
Working from the bottom upwards (and in order), they represent: LATITUDE (green), LONGITUDE
(green), and TIME (blue).

The two 7 Segment displays shows visual feedback of the numbers entered. The first digit entered
will appear on the right-hand display. When the next digit is entered, the first digit is pushed to the
left display and the new number appears on the right display. This allows for easy copying of long
numbers because the displays provide reference of the last two digits entered.
While entering numbers, the button on the bottom left marker (-1) will toggle NEGATIVE – whether
the coordinate you are entering is a negative number. This can be changed as you enter the number
and will take effect once all 8 digits are entered. Whether NEGATIVE is active or not is indicated by
the NEGATIVE indicator, an (unfortunately dim) orange LED to the left of the keypad.
If you make a mistake entering the numbers, you can delete the last digit entered by pressing the
button on the bottom right of the keypad marked (<-). This steps back one digit, and displays a zero
on the right-hand display that the next entered digit will set, and the digit before that on the lefthand
display.

Enter 8 digits of latitude – The first two digits are 10s and 1s (as latitude is between 90 and -90), and
the remaining 6 are decimal digits.
After the 8th digit is entered, the mode will advance to Longitude. This is indicated by the input mode
indicator changing, both segment displays resetting to zero, and if NEGATIVE is on it will be set to
off. Because Longitude is measured between -180 and 180, the first three digits are 100s, 10, and 1s,
and the following 5 digits are decimal places. The 6th decimal place digit represents 0.11m of
accuracy, so losing it in Longitude mode is non-critical.
After the 8th digit of longitude is entered, the input mode will advance to TIME.
The time value represents how many hours and minutes from the current time you want to arrive at
the previously entered destination. Time takes four digits – two digits for hours, and two for
minutes. For examples, to set 1 hour and 33 minutes, you would enter the digits 01 33.
After the fourth digit of time has been entered, the device changes from INPUT mode to
NAVIGATION mode.

# NAVIGATION MODE:
When navigation mode starts, the timer begins and the unit starts looking for a GPS signal.
The GPS connection status is shown by the GPS purple indicator light; if the indicator is pulsing then
the unit is still looking for a GPS connection. When the Navigator finds a GPS signal the light will
change to solid colour.

The main display of the Chronographic Navigator is the panel sitting at an angle, attached above the
input area. The display consists of four vertical lines (purple) interspaced with four small circles.
On the face of the display, the left-hand side represents your destination and the right-hand side
your starting position. Your current position is shown by the green dots, each one representing a
quarter of the overall distance. The position of the closing time-based radius is shown by the purple
lines. This display shows whether the unit’s position is inside of the radius or not (if the green dot is
closer – further left – than the glowing purple line).

The two 7 segment displays, by default, show the percentage of time elapsed. This can be changed
to show the percentage of the total distance covered by the unit by pressing the (-1) key on the
bottom left of the keypad. The display mode is indicated by the column of input mode indicators.
In time percentage mode, the middle green light is lit. Distance mode is shown by the top blue light.
If either the time or distance percentages reach 100% the displays will flash all segments. 
