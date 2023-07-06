#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial dbgSerial(2, 3); // RX, TX

// * TODO: Delete wifi passwords
String SSID[] = {"JUPITER", "Mytrle"};
String PASS[] = {"Potatoes", "December2003"};

char myChar; //for the serial stuff

//#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>

//#include <SocketIoClient.h>

#include <CapacitiveSensor.h>
#include <FastLED.h>

//All that wifi shit I have to do (Im sure I can figure it out)
//Inspo for WIFI stuff: https://github.com/imjosh/espLedDimmer/tree/master  
/**
* Modify to try multiple wifi network names and passwords (have Christina add the network in italy?)
* Find a way to have two "inputs" - One to change colour, and one to blink yours and one to blink yours and your partner's
*   Maybe doubletap and hold changes the colour? Or some combination of # of taps and tap duration
*   Could always have some sort of second input, idk what though (discuss with nathan)
* When connecting to wifi, have it blink yellow when connecting, flash green when connected, and flash red when disconnected (do all the wifi shit later?)
*/


/** *TODO*
* CHECKTODO: Offline - get light colours set up, write new "writeLED" function CHECK
* TODO: Offline - Make new colour profiles (task for later later)
* TODO: Online - Setup socket.io and get web server squared away
* TODO: Online - set up arduino code to connect to wifi and server
* TODO: Online - set up arduino code to send and interpret messages from server
* CHECKTODO: Craft - dye polyester
* TODO: Misc - Solder circuit to protoboard / make both of the jars
* Try to tick off at least one of these per day? (can even break them up to be smaller to "accomplish" more)
*/



CapacitiveSensor Sensor = CapacitiveSensor(12, 8);
                //{b,g,r,b,r,g,b,y}  // Which pin does which colour
//int ledPins[] = {2,3,4,5,6,7,8,9};  OLD

              //{b,g,r,b, g, r,y,b}
//int ledPins[] = {3,5,6,9,10,11,2,4} DEPRECATED

#define NUM_LEDS 20
#define DATA_PIN 5

CRGB leds[NUM_LEDS];

int buttonPin = 7;
int buttonState = 0;  // variable for reading the pushbutton status

long val;
int h = 1; //range is 1 to 8
int threshold = 5000;
bool onlineMode = true; //false == offline mode (flipped bc "toggle mode")
                   //true == online mode


int reds[] = {0, 255, 153, 133, 107, 72, 43, 20, 7}; //deprecated
int greens[] = {0, 153, 51, 89, 73, 52, 47, 24, 11};
int blues[] = {0, 0, 255, 186, 132, 117, 119, 82, 52};
int blue[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};  //fill in the rest of these
int yellow[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};  //fill in the rest of these



//OFFLINE STUFF

/**
* Once offline mode is turned on, lights blink white twice to signify the change (this could be the "colour change" method that is used in online mode?)
* Single tap changes colour scheme
* Long press turns off LED
*/



//ONLINE STUFF
/** https://igorkonovalov.github.io/projects/2017/10/28/Arduino_Node_SocketIO.html -- Use this process for setting up sending stuff to web server (and then find a way to make it recieve)
* When online mode is turned on, lights flash white once, then pulse yellow when signing into wifi, going red if there's an error (and then back to offline mode) and doing one flash of green if connected and then resuming normal operation
* Socket stuff that nathan said for connections and shit
* * Basic principle is that when the top is tapped, the other person's lights up
* Some method of changing colour in this mode?
* * Maybe some combination of taps enables some special mode for playing special things on both of the jars (maybe some music?)
* * Maybe long press (>5 seconds?) allows you to enter colour change mode, where tapping changes your colour scheme (long-ish press to confirm colour)
* * * And if you tap 3 times (or some other pattern), the lights flash a few times and it changes the colour of the other jar!
*/

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(5000);

  //dbgSerial.begin(19200); //can't be faster than 19200 for softserial
  dbgSerial.begin(38400);  //38400 works fine for me

  dbgSerial.println("ESP8266 Demo");
  delay(100);


  //for(int i = 0; i < 8; i++){
  //  pinMode(ledPins[i], OUTPUT);
  //}
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  pinMode(buttonPin, INPUT);
  delay(10);
  //flash white once to signify turned on
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::White;
    FastLED.show();
    leds[i] = CRGB::Black;
    delay(30);
  }
  //if more stuff needs to be done

  toggleMode();
  /*
  for(int j = 0; j < 2; j++){
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    delay(250);
    FastLED.clear();
    FastLED.show();
    delay(250);
  }*/
  
}

void loop() {
  //h = 1;
  if(onlineMode){
    //Do online stuff
  } else {
    interpretTap();
  }
  delay(30);

//just add a some delay after everything is done so it doesn't check the button too often
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH){
    toggleMode();
    delay(1000);
  }

  //delay(30);

  //check if button is pressed
  //buttonState = digitalRead(buttonPin);
  //if(buttonState == HIGH){
    //on button press, call toggleMode();
  //  toggleMode();
  //}
  //delay(10);
}
void writeLED(int num){
  fill_solid(leds, NUM_LEDS, CRGB(reds[h], greens[h], blues[h]));
  FastLED.show();
}
void killLED(){
  FastLED.clear();
  FastLED.show();
}
void writeGradient(){
  return;
}
/*
void oldWriteLED(int num){
  analogWrite(ledPins[2], reds[num]);
  analogWrite(ledPins[1], greens[num]);
  analogWrite(ledPins[0], blues[num]);
  analogWrite(ledPins[5], reds[num]);
  analogWrite(ledPins[4], greens[num]);
  analogWrite(ledPins[3], blues[num]);
  analogWrite(ledPins[6], blue[num]);
  analogWrite(ledPins[7], yellow[num]);
}

void oldKillLED(){
  analogWrite(ledPins[2], reds[0]);
  analogWrite(ledPins[1], greens[0]);
  analogWrite(ledPins[0], blues[0]);
  analogWrite(ledPins[5], reds[0]);
  analogWrite(ledPins[4], greens[0]);
  analogWrite(ledPins[3], blues[0]);
  analogWrite(ledPins[6], blue[0]);
  analogWrite(ledPins[7], yellow[0]);
} */

void interpretTap(){  //should work?
  val = Sensor.capacitiveSensor(30);
  //Serial.println(val);
  if(val <= threshold) {
    return; //means that there is no input, 
  } else {
    h++;
    //Serial.println(h);
    if(h == 9){
      h = 1;
    }
    writeLED(h);
    delay(250); // quarter second delay so that theres only one tap
    val = Sensor.capacitiveSensor(30);
    int counter = 0;
    while(val >= threshold && counter < 30){ //toggle the mode after 3 seconds
      counter++;
      delay(10);
      val = Sensor.capacitiveSensor(30);
      //Serial.println(val);
      //Serial.println("COunting Seconds");
    }
    if(counter == 30){ // TODO: this isn't working for some reason
      killLED();
      h-= 2;
      while(val >=threshold){
        delay(50);
        //Serial.println(val);
        //Serial.println("Waiting");
        val = Sensor.capacitiveSensor(30);
      }
    }
    counter = 0;
  }
}
void toggleMode(){
  onlineMode ^= true; //cool little xor boolean switcher
  if(onlineMode){
    for(int j = 0; j < 2; j++){ //signals online mode
      fill_solid(leds, NUM_LEDS, CRGB::Blue);
      FastLED.show();
      delay(250);
      FastLED.clear();
      FastLED.show();
      delay(250);
    }

    //test if the module is ready
    Serial.println("AT+RST");
    if (Serial.find("ready")){
      dbgSerial.println("Module is ready");
      delay(1000);
      //connect to the wifi
      boolean connected = false;
      for (int i = 0; i < 5; i++){
        if (connectWiFi()) {
          connected = true;
          break;
        }
      }
      if (!connected) {
        errorHandler(0);
      }

      delay(5000);
      //set the single connection mode
      Serial.println("AT+CIPMUX=0");
    } else {
      dbgSerial.println("Module didn't respond.");
      delay(100);

      //serial loop mode for diag
      while (1) {
        while (dbgSerial.available()) {
          myChar = dbgSerial.read();
          Serial.print(myChar);
          digitalWrite(13, HIGH);
          delay(50);
          digitalWrite(13, LOW);
          delay(50);
        }

        while (Serial.available()) {
          myChar = Serial.read();
          delay(25);
          dbgSerial.print(myChar);
        }
      }
    }

  } else{
    for(int j = 0; j < 2; j++){ //signlas offline mode
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      FastLED.show();
      delay(250);
      FastLED.clear();
      FastLED.show();
      delay(250);
    }
    writeLED(h);
  }
}

void tapOther(){
  //Do web stuff
  //lowkey scared for this shit, and I gotta write the webapp
}

void errorHandler(int val){
  switch(val){
    case 0:
      while(true){
         for(int i = 0; i < NUM_LEDS; i++){
          leds[i] = CRGB::Orange;
          FastLED.show();
          leds[i] = CRGB::Black;
          delay(70);
        }
      }
      break;
    default:
      return;
  }
}

boolean connectWiFi(){
  fill_solid(leds, NUM_LEDS, CRGB::Orange);
  FastLED.show();
  for(int i = 0; i < 2; i++){
    Serial.println("AT+CWMODE=1");
    String cmd = "AT+CWJAP=\"";
    cmd += SSID[i];
    cmd += "\",\"";
    cmd += PASS[i];
    cmd += "\"";
    dbgSerial.println(cmd);
    Serial.println(cmd);
    delay(2000);
    if (Serial.find("OK")) {
      dbgSerial.println("OK, Connected to WiFi.");
      return true;
    }
  }
  dbgSerial.println("Can not connect to the WiFi.");
  return false;
}

