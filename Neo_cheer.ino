
/* This code is adapted from the adafruit Feather Huzzah tutorial found here.  https://learn.adafruit.com/cheerlights/video
It lights up the dotstar as well as Neopixels using the FastLED library.  You do need to import the FastLED library to build this.


#include "WiFi.h"
#include <TinyPICO.h>
#include <FastLED.h>
#include "ThingSpeak.h" // Include the ThingSpeak library
#define NUM_LEDS 4
#define DATA_PIN 25
CRGB leds[NUM_LEDS];

int delayval = 500; // delay for half a second
 
WiFiClient  client;
TinyPICO tp = TinyPICO(); //The Tiny Pico library is necessary for the onboard dotstar

const char* ssid = "Buttons";
const char* password =  "7FF68D064F";

const char* host = "api.thingspeak.com";
unsigned long cheerLightsChannelNumber = 1417; // The channel of Cheerlights
 
void setup() {
 
  Serial.begin(115200);
  delay(100);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  static unsigned long LastColor = 0;

  ThingSpeak.begin(client);
  //Neopixel Setup
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // brightness = 75;
  FastLED.setBrightness(255);
}

String colorName[] = {"none", "red", "pink", "green", "blue", "cyan", "white", "warmwhite", "oldlace", "purple", "magenta", "yellow", "orange", "1999"}; // List of CheerLights color names
 
int colorRGB[][3] = {     0,  0,  0,  // "none"  // Map of RGB values for each of the Cheerlight color names
                          255,  0,  0,  // "red"
                          255, 192, 203, // "pink"
                          0, 255,  0, // "green"
                          0,  0, 255, // "blue"
                          0, 255, 255, // "cyan",
                          255, 255, 255, // "white",
                          255, 223, 223, // "warmwhite",
                          255, 223, 223, // "oldlace",
                          128,  0, 128, // "purple",
                          255,  0, 255, // "magenta",
                          255, 255,  0, // "yellow",
                          255,165,  0, // "orange"
                          128,  0, 128}; // "1999 Prince purple"
float brightness = 1;

void loop() {

  Serial.print("connecting to ");
  Serial.println(host);
 
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 
  // We now create a URI for the request
  String url = "/channels/1417/field/1/last.json";
  Serial.print("Requesting URL: ");
  Serial.println(url);
 
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(500); // Just a quick pause to let everything settle
  String color = ThingSpeak.readStringField(cheerLightsChannelNumber, 1); //Read the Cheerlight color and print to the serial monitor
  Serial.println(color);
  setColor(color);
  delay(18000);
}

void setColor(String color)
{
  // Look through the list of colors to find the one that was requested
  for (int iColor = 0; iColor <= 13; iColor++)
  {
    if (color == colorName[iColor])
    {
      Serial.println("Setting the LED" +color);
      tp.DotStar_SetPixelColor(colorRGB[iColor][0]* brightness, colorRGB[iColor][1] * brightness, colorRGB[iColor][2] * brightness); // set pixel 1 color (kept them seperate for easy hacking & understanding)
      for(int dot = 0; dot < NUM_LEDS; dot++) { 
            leds[dot].setRGB(colorRGB[iColor][0]* brightness, colorRGB[iColor][1] * brightness, colorRGB[iColor][2] * brightness);
            FastLED.show();
            // clear this led for the next time around the loop
            //leds[dot] = CRGB::Black;
            //delay(3000);
        } 
      delay(delayval); // Delay for a period of time (in milliseconds).
 
    }
  }
}
