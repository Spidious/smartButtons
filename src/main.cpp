#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESP8266WebServer.h>

// Web Function Headers
bool loadFromSpiffs(String path);
void handleRoot();
void handleWebRequests();
bool attemptConnect();
// HTML main file
const char * HTMLFile = "test.html";

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
const uint8_t outPins[] = {D5};
const uint8_t numOuts = sizeof(outPins)/sizeof(outPins[0]);

//Webserver (IDK what the 80 does)
ESP8266WebServer server(80);

// Startup function
void setup() {
  // Spiffs initialization
  SPIFFS.begin();
  Serial.println("File System Initialized");

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

  // Initialize Webserer
  server.on("/", handleRoot);
  server.onNotFound(handleWebRequests);
  server.begin();
  Serial.println();
}

// Looping function
void loop() {
  if(WiFi.status() != WL_CONNECTED){
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on (IDK why builtin led is LOW for on)
  }

  // Loop through all the inputs
  for(int i = 0; i < numIns; i++){
    if(digitalRead(inPins[i])==HIGH){
      // This runs when the button is pressed

      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Hit");

    }
  }
  server.handleClient();
}

void handleRoot(){
    server.sendHeader("Location", "/" + String(HTMLFile), true);
    server.send(302, "text/plain", "");
}

void handleWebRequests(){
    if(loadFromSpiffs(server.uri())) return;
    else server.send(404, "text/plain", "File not detected :(");
}

bool loadFromSpiffs(String path){
    String dataType = "text/plain";

    // Edit path
    if(path.endsWith("/")) path += String(HTMLFile);

    // Find correct dataType
    if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
    else if(path.endsWith(".html")) dataType = "text/html";
    else if(path.endsWith(".htm")) dataType = "text/html";
    else if(path.endsWith(".css")) dataType = "text/css";
    else if(path.endsWith(".js")) dataType = "application/javascript";
    else if(path.endsWith(".png")) dataType = "image/png";
    else if(path.endsWith(".gif")) dataType = "image/gif";
    else if(path.endsWith(".jpg")) dataType = "image/jpeg";
    else if(path.endsWith(".ico")) dataType = "image/x-icon";
    else if(path.endsWith(".xml")) dataType = "text/xml";
    else if(path.endsWith(".pdf")) dataType = "application/pdf";
    else if(path.endsWith(".zip")) dataType = "application/zip";

    // Read and send data to server
    File dataFile = SPIFFS.open(path.c_str(), "r");
    if(!dataFile){
        Serial.print("ERROR: cannot find dataFile -> ");
        Serial.println(path);
        return false;
    }
    if(server.hasArg("download")) dataType = "application/octet-stream";
    if(server.streamFile(dataFile, dataType) != dataFile.size()){}
    dataFile.close();
    return true;
}

// bool attemptConnect(){
//   WiFi.
// }