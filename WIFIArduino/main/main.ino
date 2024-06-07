#include <Grandeur.h> //service for HTTP REquests
#include "WiFi.h" //to connect to WiFi



// WiFi credentials --> my HotSPot
const char* ssid = "lucaarduino";
const char* passphrase = "42069420";

// Grandeur credentials --> removed so no one can see it
const char * apiKey = "";
const char* token = "";
const char* deviceId = "";

Grandeur::Project project;

unsigned long current = millis(); //to do it every 3 seconds
int highscore = 0;


void setup() {
    Serial.begin(9600);
    // This connects the device to WiFi.
    connectToWiFi(ssid, passphrase);
    // This connects the device to internet.
    project = grandeur.init(apiKey, token);
}

int preScore; //eliminate errors

void loop() {

  int value = Serial.read();

//same as previous
  if(value == preScore+1 || value == 1){
    highscore = value; 
    Serial.println(highscore);
    preScore = value;
  }


    //Serial.println(highscore);
    // This sends data to internet.
    if(project.isConnected() && millis() - current > 3000) {
        project.device(deviceId).data().set("millis", highscore);
        current = millis();
    }

    // This runs the SDK when the device WiFi is connected.
    if(WiFi.status() == WL_CONNECTED) project.loop();
}