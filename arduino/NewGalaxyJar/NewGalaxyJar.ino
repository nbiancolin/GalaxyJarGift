#include <SocketIOclient.h>
#include <WebSocketsClient.h>

#include <CapacitiveSensor.h>
#include <FastLED.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <PolledTimeout.h>
#include <algorithm>  // std::min

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ArduinoJson.h>

#include <WebSocketsClient.h>
#include <SocketIOclient.h>

#include <Hash.h>

//clean these up later


/**
* === GALAXY JAR ARDUINO CODE ===
* TODO: Add LED colouring during background process
*/


//Wifi Passwords (should delete these later but oh well)
const char * SSID[] = {"JUPITER", "Mytrle"};
const char * PASS[] = {"Potatoes", "December2003"};



//setup values and things
CapacitiveSensor Sensor = CapacitiveSensor(12, 15); //pins 6 and 8 are actually gpio 12 and 15 (if this doesn't work, try 6 and 16)

#define NUM_LEDS 20
#define DATA_PIN 2 //pin 4 is gpio 2 (alt 17)
CRGB leds[NUM_LEDS];
#define BUTTON_PIN 5 //pin 1 is GPIO 5 (alt 20)

long val;
int h = 1; //range is 1 to 4 (for now)
int threshold = 45; //tinker with this so that it works with the correct voltage

bool state = false;

constexpr int port = 23;

//WiFiServer server(port);
//WiFiClient client;
ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

#define USE_SERIAL Serial


// * TODO: 

/**
* #Process:
* 1. Run through all LEDs to ensure they are functioning properly
* 2. Connect to WiFi (try both networks and passwords) (pulse yellow while connecting, flash green when connected and hold red if unable to connect) (try 3 times to connect to network then pulse red to signify error)
* 3. Connect to socket server (pulse orange while connecting, flash green twice once connected and blink red if unable to connect)
* 4. REGULAR OPERATION
*   a. tap turns on both lights
*   b. hold down for ~3 seconds to enter colour change mode
*     i. tap to cycle through colour patterns, hold down for ~1.5 secs to confirm (colour blinks twice then turns off)
*   c. Pushing button switches to offline mode (develop later)
*/


void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case sIOtype_DISCONNECT:
            USE_SERIAL.printf("[IOc] Disconnected!\n");
            fill_solid(leds, NUM_LEDS, CRGB::Red);
            FastLED.show();
            break;
        case sIOtype_CONNECT:
            USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);

            // join default namespace (no auto join in Socket.IO V3)
            socketIO.send(sIOtype_CONNECT, "/");
            break;
        case sIOtype_EVENT:
            USE_SERIAL.printf("[IOc] get event: %s\n", payload);
            
            state = !state;
            break;
        case sIOtype_ACK:
            USE_SERIAL.printf("[IOc] get ack: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_ERROR:
            USE_SERIAL.printf("[IOc] get error: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_EVENT:
            USE_SERIAL.printf("[IOc] get binary: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_ACK:
            USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
            hexdump(payload, length);
            break;
    }
}

void setup() {
  Serial.begin(115200);
  Serial.println(ESP.getFullVersion());
  Serial.println("Ready to start");

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  delay(10);
  Serial.println("FastLED setup completed");
  //flash white once to signify turned on
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::White;
    FastLED.show();
    leds[i] = CRGB::Black;
    delay(30);
  }
  if(WiFiSetup()){ ///sub-setup functions
    Serial.println("Mode Sucessfully Toggled (Setup successfully completed)");
    state = false;
  } else {
    Serial.println("An Error Occured in startup. Try restarting!");
  }
  pinMode(BUTTON_PIN, INPUT);
}

unsigned long messageTimestamp = 0;
void loop() {
  socketIO.loop();
  //if state == true -> IGNORE ANY TAPS UNTIL state IS SET TO false (by calling)

  if(state){
    writeLED();
    changeColour();
    return;
  } else{
    killLED();
    val = Sensor.capacitiveSensor(30);
    Serial.println(val);
    if(val <= threshold) {
      changeColour();
      return; //means that there is no input, 
    } else {
      //socketIO.loop(); //incase somehow the other button was pressed at *exactly* the same time
      sendBeep();
      socketIO.loop();
      writeLED();
      delay(100);
      val = Sensor.capacitiveSensor(30);
      while(val >= threshold){
        delay(50);
        Serial.println("waiting for release");
        val = Sensor.capacitiveSensor(30);
        socketIO.loop();
      }
      Serial.println("Released");
      sendBeep();
      killLED();
      socketIO.loop();
      return;
    }
  }

    

    //ping server to check for state changes
    //if led should be on, do nothing, keep checking for changes
    //if led should be off, check for touch
    //if not touched, return to checking for changes

  //check for button (doesn't work, maybe delete this entirely?)
}

void changeColour(){
  //check for button press
  Serial.println("Reading for button");
  Serial.println(digitalRead(BUTTON_PIN));
  if(digitalRead(BUTTON_PIN) == 1){
    return;//if not pressed, return
  } else{
    //if pressed, increase h, then wait for button release
    h++;
    Serial.println("changing colour");
    if(h >=5){
      h = 0;
    }
    writeLED();
    delay(500);
    killLED();
  }
  return;
  
}

void sendBeep(){
  uint64_t now = millis();

    if(now - messageTimestamp > 50) {
        messageTimestamp = now;

        // creat JSON message for Socket.IO (event)
        DynamicJsonDocument doc(1024);
        JsonArray array = doc.to<JsonArray>();

        // add evnet name
        // Hint: socket.on('event_name', ....
        array.add("beep");

        // add payload (parameters) for the event
        JsonObject param1 = array.createNestedObject();
        param1["beep"] = (uint32_t) now;

        // JSON to String (serializion)
        String output;
        serializeJson(doc, output);

        // Send event
        socketIO.sendEVENT(output);

        // Print JSON for debugging
        USE_SERIAL.println(output);
    }
}
void writeLED(){
  switch(h){
    case 0:
      break;
    case 1:
      fill_gradient_RGB(leds, NUM_LEDS, CRGB::Blue, CRGB::Purple);
      break;
    case 2:
      fill_gradient_RGB(leds, NUM_LEDS, CRGB::Orange, CRGB::Yellow);
      break;
    case 3:
      fill_gradient_RGB(leds, NUM_LEDS, CRGB::Purple, CRGB::Blue);
      break;
    case 4:
      fill_gradient_RGB(leds, NUM_LEDS, CRGB::Red, CRGB::Yellow);
      break;
    default:
      fill_gradient_RGB(leds, NUM_LEDS, CRGB::Blue, CRGB::Purple);
  }
  FastLED.show();
}
void killLED(){
  FastLED.clear();
  FastLED.show();
}

bool WiFiSetup(){
  for(int j = 0; j < 2; j++){ //signals online mode
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    FastLED.show();
    delay(250);
    FastLED.clear();
    FastLED.show();
    delay(250);
  }

  Serial.println("Connecting to Wifi Network");

  if(connectWiFi()){
    fill_gradient_RGB(leds, NUM_LEDS, CRGB::Orange, CRGB::Yellow);
    FastLED.show();
    Serial.println("Connecting to Socket");
    //connect to socket
    socketIO.begin("35.193.67.238", 80, "/socket.io/?EIO=4");
    socketIO.onEvent(socketIOEvent);
    Serial.println("WAIING");
    for(int i = 0; i < 10000; i++){
      socketIO.loop();
      //delay(5);
    }
    for(int j = 0; j < 2; j++){ 
          fill_solid(leds, NUM_LEDS, CRGB::Green);
          FastLED.show();
          delay(250);
          FastLED.clear();
          FastLED.show();
          delay(250);
        }
    socketIO.loop();
    state = false;
    
    return true;
  }
  return false;
}

bool connectWiFi(){
  fill_solid(leds, NUM_LEDS, CRGB::Orange);
  FastLED.show();
  for(int i = 0; i < 2; i++){
    int counter = 0;
    WiFiMulti.addAP(SSID[i], PASS[i]);
    while (WiFiMulti.run() != WL_CONNECTED) {
        Serial.print('.');
        delay(500);
        counter++;
        if(counter >= 15){
          break;
        }
      }
      if(WiFiMulti.run() == WL_CONNECTED){
        Serial.println("Connected");
        for(int j = 0; j < 2; j++){ 
          fill_solid(leds, NUM_LEDS, CRGB::Green);
          FastLED.show();
          delay(250);
          FastLED.clear();
          FastLED.show();
          delay(250);
        }
        return true;
      }
      for(int j = 0; j < 2; j++){ 
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        FastLED.show();
        delay(250);
        FastLED.clear();
        FastLED.show();
        delay(250);
      }
  }
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  return false;
}
