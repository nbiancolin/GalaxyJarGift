// * TODO: Delete wifi passwords
String SSID[] = {"JUPITER", "Mytrle"};
String PASS[] = {"Potatoes", "December2003"};

//#include <ESP8266WiFi.h> //need to learn abt these libraries
//#include <ESP8266WiFiMulti.h> 

//#include <SocketIoClient.h>

#include <CapacitiveSensor.h>
#include <FastLED.h>

#define D0  16
#define D1  5  // I2C Bus SCL (clock)
#define D2  4  // I2C Bus SDA (data)
#define D3  0
#define D4  2  // Same as "LED_BUILTIN", but inverted logic
#define D5  14 // SPI Bus SCK (clock)
#define D6  12 // SPI Bus MISO 
#define D7  13 // SPI Bus MOSI
#define D8  15 // SPI Bus SS (CS)
#define D9  3  // RX0 (Serial console)
#define D10 1  // TX0 (Serial console)

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
* CHECKTODO: Online - Setup socket.io and get web server squared away
* TODO: Online - set up arduino code to connect to wifi and server and use socketio functions
* CHECKTODO: Craft - dye polyester
* TODO: Misc - Solder circuit to protoboard / make both of the jars
* Try to tick off at least one of these per day? (can even break them up to be smaller to "accomplish" more)
*/



CapacitiveSensor Sensor = CapacitiveSensor(6, 8);
#define NUM_LEDS 20
#define DATA_PIN D3

CRGB leds[NUM_LEDS];

int buttonPin = D1;
int buttonState = 0;  // variable for reading the pushbutton status

long val;
int h = 1; //range is 1 to 8
int threshold = 5000;
bool onlineMode = true; //false == offline mode (flipped bc "toggle mode")
                   //true == online mode


int reds[] = {0, 255, 153, 133, 107, 72, 43, 20, 7}; //should update these but what the hell
int greens[] = {0, 153, 51, 89, 73, 52, 47, 24, 11};
int blues[] = {0, 0, 255, 186, 132, 117, 119, 82, 52};

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

  //for(int i = 0; i < 8; i++){
  //  pinMode(ledPins[i], OUTPUT);
  //}
  Serial.println("Code initialized");
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  pinMode(buttonPin, INPUT);
  delay(10);
  Serial.println("FastLED setup completed");
  //flash white once to signify turned on
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::White;
    FastLED.show();
    leds[i] = CRGB::Black;
    delay(30);
  }
  Serial.println("Start LED sequence completed");
  //if more stuff needs to be done

  toggleMode();
  Serial.println("Mode Toggled");
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

  if(onlineMode){
    //Do online stuff
  } else {
    oldInterpretTap();
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

void offlineInterpretTap(){
  val = Sensor.capacitiveSensor(30);
}

void oldInterpretTap(){  //should work?
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

/*
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
}*/

