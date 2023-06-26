//All that wifi shit I have to do (Im sure I can figure it out)
//Inspo for WIFI stuff: https://github.com/imjosh/espLedDimmer/tree/master  
/**
* Modify to try multiple wifi network names and passwords (have Christina add the network in italy?)
* Find a way to have two "inputs" - One to change colour, and one to blink yours and one to blink yours and your partner's
*   Maybe doubletap and hold changes the colour? Or some combination of # of taps and tap duration
*   Could always have some sort of second input, idk what though (discuss with nathan)
* When connecting to wifi, have it blink yellow when connecting, flash green when connected, and flash red when disconnected (do all the wifi shit later?)
*/

#include <CapacitiveSensor.h>

CapacitiveSensor Sensor = CapacitiveSensor(12, 10);
              //{b,g,r,b,r,g,b,y}  // Which pin does which colour
int ledPins[] = {2,3,4,5,6,7,8,9}; 

long val;

int reds[] = {0, 255, 153, 133, 107, 72, 43, 20, 7};
int greens[] = {0, 153, 51, 89, 73, 52, 47, 24, 11};
int blues[] = {0, 0, 255, 186, 132, 117, 119, 82, 52};
int blue[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};  //fill in the rest of these
int yellow[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};  //fill in the rest of these

void setup() {
  Serial.begin(9600); //remove serial stuff for when disconnecting arduino
  for(int i = 0; i < 8; i++){
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  val = Sensor.capacitiveSensor(30);
  Serial.println(val);
  if (val >= 1000){
    i++;
    if(i == 9){
      i = 0;
    }
    writeLED(i);
    delay(200);
  }

  delay(10);
}

void writeLED(int num){
  analogWrite(ledPins[2], reds[num]);
  analogWrite(ledPins[1], greens[num]);
  analogWrite(ledPins[0], blues[num]);
  analogWrite(ledPins[5], reds[num]);
  analogWrite(ledPins[4], greens[num]);
  analogWrite(ledPins[3], blues[num]);
  analogWrite(ledPins[6], blue[num]);
  analogWrite(ledPins[7], yellow[num]);
}



