/* This was modified from Fernando Koyang's Instructable visit  it here... https://www.instructables.com/id/ESP32-Bluetooth-Low-Energy/
 *  */

//Bluetooth headers
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
//Led headers
#include <TinyPICO.h>
#include <FastLED.h>
//Setup the Fast LED

#define NUM_LEDS 4
#define DATA_PIN 25
CRGB leds[NUM_LEDS];
float brightness;
String color;
String recieved;


int delayval = 500; // delay for half a second
TinyPICO tp = TinyPICO(); //The Tiny Pico library is necessary for the onboard dotstar

//Jump into Bluetooth setup and methods...
BLECharacteristic *characteristicTX; //através desse objeto iremos enviar dados para o client

bool deviceConnected = false; //controle de dispositivo conectado

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "4fafc201-1fb5-459e-8fcc-c5c9c331914b" // UART service UUID
#define CHARACTERISTIC_UUID_RX "4ac8a682-9736-4e5d-932b-e9b31405049c"
#define CHARACTERISTIC_UUID_TX "0972EF8C-7613-4075-AD52-756F33D4DA91"
#define CHARACTERISTIC_UUID_RX2 "59e3247e-8658-4fb4-a2d9-1c7a18339fa1"

//callback para receber os eventos de conexão de dispositivos
class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

//callback  para envendos das características
class CharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
      //retorna ponteiro para o registrador contendo o valor atual da caracteristica
      std::string rxValue = characteristic->getValue(); 
      //verifica se existe dados (tamanho maior que zero)
      color.clear();
      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");

        
        for (int i = 0; i < rxValue.length(); i++) {
          color +=rxValue[i];
          Serial.print(rxValue[i]);
        }

        

        Serial.println();

        Serial.println("*********" );
      }
    }
        
};


void setup() {
  Serial.begin(115200);
  //Setup leds
  TinyPICO tp = TinyPICO(); //The Tiny Pico library is necessary for the onboard dotstar
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

  // Create the BLE Device
  BLEDevice::init("ESP32-BLE"); // nome do dispositivo bluetooth
  // Create the BLE Server
  BLEServer *server = BLEDevice::createServer(); //cria um BLE server 
  server->setCallbacks(new ServerCallbacks()); //seta o callback do server
  // Create the BLE Service
  BLEService *service = server->createService(SERVICE_UUID);
  // Create a BLE Characteristic para envio de dados
  characteristicTX = service->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
 
                      
  characteristicTX->addDescriptor(new BLE2902());

  // Create a BLE Characteristic para recebimento de dados
  BLECharacteristic *characteristic = service->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  characteristic->setCallbacks(new CharacteristicCallbacks());
  
  // Start the service
  service->start();
  // Start advertising (descoberta do ESP32)
  server->getAdvertising()->start();  
  
  Serial.println("Waiting a client connection to notify...");
  brightness = 1;

}
//Setup the color arrays
String colorName[] = {"none", "red", "pink", "green", "blue", "cyan", "white", "warmwhite", "oldlace", "purple", "magenta", "yellow", "orange", "1999"}; // List of CheerLights color names
 
int colorRGB[][3] = {     0,  0,  0,  // "none"  // Map of RGB values for each of the Cheerlight color names
                          255,  0,  0,  // "red"
                          255, 119, 218, // "pink"
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


void loop() {
     //se existe algum dispositivo conectado
    if (deviceConnected) {
      
      
          // Let's convert the value to a char array:
          //char txString[10] = "hello"; // make sure this is big enuffz

          //characteristicTX->setValue(txString); //seta o valor que a caracteristica notificará (enviar)       
          //characteristicTX->notify(); // Envia o valor para o smartphone
          //String recievedString = txString;

          //Serial.print("*** Sent Value: ");
          //Serial.print(txString);
          Serial.println(" ***");
          setColor(color);
          //String color = txString;
          Serial.println("The Color is..." +color +"...");
          
    
    delay(1000);



    /*characteristicTX2->setValue("Hello!"); // Sending a test message
    characteristicTX2->notify(); // Send the value to the app!*/

  }
  delay(1000);

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
