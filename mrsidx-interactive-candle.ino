
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 15

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define AIO_SERVER      "raspkerapi.local:18800"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "candlelightmqtt"
#define AIO_KEY         "romanticmqtt"

/* Set these to your desired credentials. */
const char *ssid = "Tatooine";
const char *password = "vaderhater1a";
//const char *ssid = "sidenet";
//const char *password = "entersid";



//const char *ssid = "lightproj";
//const char *password = "thereisnospoon";
char *state = "rainbow";
const char *stat = "";
int stroberate = 1000;
int rainbowspeed = 35;
ESP8266WebServer server(80);
//Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);


const char *pixelArrayType = "feather4x8";

 char *ui = "<!DOCTYPE HTML><style>a {background-color: red;color: white;padding: 1em 1.5em;text-decoration: none;text-transform: uppercase;}</style><html><h3>states:</h3><br><p><a href=\"http://lightproj1.local/led/off\">OFF</a><p><br><a href=\"http://lightproj1.local/state/fire\">Fire Effect</a><p><br><a href=\"http://lightproj1.local/state/police\">Police (Red Blue) Effect</a><p><br><a href=\"http://lightproj1.local/state/hazzard\">Hazzard (Yellow) Effect</a><p><br><a href=\"http://lightproj1.local/state/rainbow\">Rainbow Effect</a><p><br><a href=\"http://lightproj1.local/state/strobe\">Strobe</a><p><br><a href=\"http://lightproj1.local/green\">Solid Green</a><p><br><a href=\"http://lightproj1.local/red\">Solid Red</a><p><br><a href=\"http://lightproj1.local/blue\">Solid Blue</a><p><br><a href=\"http://lightproj1.local/state/singleLightningStrike\">Single Lightning Strike</a><p><br>  </html>";

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */






///////////SETUP//////////////////
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP("CandleLight", "romantic");
  //WiFi.softAPdisconnect (true);
  WiFi.begin(ssid, password);


  //IPAddress myIP = WiFi.softAPIP();
  IPAddress myIP = WiFi.localIP();

  Serial.print("AP IP address: ");
  
  
  Serial.println(myIP);
  Serial.println("lightproj1.local");
  server.on("/", handleRoot);
  server.on("/led/on", handleOn);
  server.on("/led/off", handleOff);
  server.on("/state/fire", handleStateFire);
  server.on("/state/police", handlePolice);
  server.on("/state/hazzard", handleHazzard);
  server.on("/state/redhazzard", handleRedHazzard);
  server.on("/state/rainbow", handleRainbow);
  server.on("/green", handleGreen);
  server.on("/red", handleRed);
  server.on("/blue", handleBlue);
  server.on("/blue", handleWhite);
  server.on("/brightness/up", setBrightnessUp);
  server.on("/brightness/down", setBrightnessDown);
  server.on("/state/strobe", handleStrobe);
  server.on("/state/singleLightningStrike", handleSingleLightningStrike);
  server.on("/state/autoLightningStrike", handleAutoLightningStrike);
  server.on("/setcolor", handleSetColor);
  server.on("/stats", handleStats);

  if ( MDNS.begin ( "lightproj1" ) ) {
    Serial.println ( "MDNS responder started" );
  }
  
  server.begin();
  Serial.println("HTTP server started");
  
   // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  strip.begin();
  colorWipe(strip.Color(0,0,0),0);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

//  MQTT_connect();
//
//  // this is our 'wait for incoming subscription packets' busy subloop
//  // try to spend your time here
//
//  Adafruit_MQTT_Subscribe *subscription;
//  while ((subscription = mqtt.readSubscription(5000))) {
//    // Check if its the onoff button feed
//    if (subscription == &onoffbutton) {
//      Serial.print(F("On-Off button: "));
//      Serial.println((char *)onoffbutton.lastread);
//      
//      if (strcmp((char *)onoffbutton.lastread, "ON") == 0) {
//        digitalWrite(LED, LOW); 
//      }
//      if (strcmp((char *)onoffbutton.lastread, "OFF") == 0) {
//        digitalWrite(LED, HIGH); 
//      }
//    }
//    
//    // check if its the slider feed
//    if (subscription == &slider) {
//      Serial.print(F("Slider: "));
//      Serial.println((char *)slider.lastread);
//      uint16_t sliderval = atoi((char *)slider.lastread);  // convert to a number
//      analogWrite(PWMOUT, sliderval);
//    }
//  }
//
//  // ping the server to keep the mqtt connection alive
//  if(! mqtt.ping()) {
//    mqtt.disconnect();
//  }

  

  server.handleClient();
  if (state == "fire") {
    delay(2);
    doFire(strip.Color(random(200),random(45),random(0)),60);
  }
  if (state == "hazzard") {
    delay(2);
    doHazzard(strip.Color(250, 250,0),500);
  }
  if (state == "redhazzard") {
    delay(2);
    doHazzard(strip.Color(200, 0,0),500);
  }
  if (state == "police") {
    delay(2);
    doPolice(4);
  }
  if (state == "rainbow") {
    delay(2);
    doRainbow();
  }
  if (state == "strobe") {
    delay(2);
    doStrobe(strip.Color(200,200,200),stroberate);
  }

  if (state == "autolightning") {
    delay(2);
    doAutoLightning();
  }
  

  server.handleClient();
  delay(2);
    

}

void doAutoLightning(){
    #define BETWEEN 2579
    #define DURATION 43 
    #define TIMES 7
    unsigned long lastTime = 0;
    int waitTime = 0;

    for (int i=0; i< random(TIMES); i++)
    {
      Serial.println(millis());
      //digitalWrite(LEDPIN, HIGH);
      colorWipe(strip.Color(255,255,255),0);
      delay(20 + random(DURATION));
      colorWipe(strip.Color(0,0,0),0);
      //digitalWrite(LEDPIN, LOW);
      delay(10);
    } 
}


void doRainbow(){
    delay(1);
    rainbow(rainbowspeed);  
}

void doPolice(int wait) {
  delay(10);
  colorWipe(strip.Color(200,0,0),wait); //Red
  colorWipe(strip.Color(0,0,0),wait); //Blank
  colorWipe(strip.Color(0,0,200),wait); //Blue
  colorWipe(strip.Color(0,0,0),wait); //Blank
  delay(10);
  colorWipe(strip.Color(200,0,0),wait); //Red
  colorWipe(strip.Color(0,0,0),wait); //Blank
  colorWipe(strip.Color(0,0,200),wait); //Blue
  colorWipe(strip.Color(0,0,0),wait); //Blank
  delay(10);
  colorWipe(strip.Color(200,0,0),wait); //Red
  colorWipe(strip.Color(0,0,0),wait); //Blank
  colorWipe(strip.Color(0,0,200),wait); //Blue
  colorWipe(strip.Color(0,0,0),wait); //Blank
}



void doHazzard (uint32_t c, uint8_t wait) {
  static bool stat = true ; 

  if(stat == true) {
    stat = false;
    delay(1);
    colorWipe(c,0);
  } else {
    stat = true;
    c= strip.Color(0,0,0);
    delay(1);
    colorWipe(c,0);
  }

  delay(wait);
}

void doFire(uint32_t c, uint8_t wait) {
    strip.setPixelColor(random(strip.numPixels()), c);
    c = strip.Color(0,0,0);
    delay(1);
    strip.show();
    
    strip.setPixelColor(random(strip.numPixels()), c);
    delay(1);
    strip.show();
    delay(wait);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  delay(1);
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


uint32_t Wheel(byte WheelPos) {
  delay(1);
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow(uint8_t wait) {
  delay(1);
  static uint16_t i, j;
    if(j > 256) {j = 0;}
    j++;
    if (i > strip.numPixels()){ i =0;}
      strip.setPixelColor(i, Wheel((i+j) & 255));
      i++;
    strip.show();
    delay(wait);
}

void doStrobe(uint32_t c, uint8_t wait){
  delay(1);
  colorWipe(strip.Color(0,0,0),0);
  strip.show();
  delay(wait);
  
  colorWipe(c,0);
  strip.show();
  delay(wait);
}

