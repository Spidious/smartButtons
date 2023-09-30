#include <Arduino.h>
#include <ESP8266WiFi.h>

// Network information
// TODO: CHANGE THIS TO PULL FROM FILE (or allow to be setup in access point, then switch over to router)
const char* SSID     = "TP-LINK_561A";
const char* password = "24426909"; 


// Set the pins to read and set the length of the length
// Unatached pins may result in false highs in that pin
const uint8_t inPins[] = {D0};
const uint8_t numIns = sizeof(inPins)/sizeof(inPins[0]);

// These are the digital output pins similarly to the in pins
// 0: network stat
const uint8_t outPins[] = {D6};
const uint8_t numOuts = sizeof(outPins)/sizeof(outPins[0]);

// Startup function
void setup() {
  // Set each pin to input
  for(int i = 0; i < numIns; i++){
    pinMode(inPins[i], INPUT);
  }
  pinMode(outPins[0], OUTPUT);
  // serial baudrate
  Serial.begin(115200);

  // Setup WIFI stuff
  WiFi.begin(SSID, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(SSID); Serial.println(" ...");
  // Print to serial until wifi connected
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    digitalWrite(outPins[0], HIGH);
    delay(500);
    Serial.print('.');
    digitalWrite(outPins[0], LOW);
    delay(500);
  }
  // Print to serial Wifi connection
  digitalWrite(outPins[0], LOW);
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}

// Looping function
void loop() {
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on (IDK why builtin led is LOW for on)

  // Loop through all the inputs
  for(int i = 0; i < numIns; i++){
    if(digitalRead(inPins[i])==HIGH){
      // This runs when the button is pressed

      digitalWrite(LED_BUILTIN, HIGH);

    }
  }
  delay(100); // Create minor delay
}