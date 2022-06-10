#include "./helpers.h"
#include "./config.h"
#include "./ota.h"
#include "./screen.h"
#include "./heater.h"

void setup(void) {
    Serial.begin(9600);
    
    pinMode(COIL_PIN, OUTPUT);
    sensors.begin();   

    initScreen();

    WiFi.mode(WIFI_STA); 

    wm.setConfigPortalBlocking(false);
    wm.setConfigPortalTimeout(60);
   
    if(wm.autoConnect("ESP32 Sous Vide")){
        startOta();
    } else {
        Serial.println("Configportal running");
    }
}

int lastReading = 0;

void loop() {
    wm.process();

    draw();

    if(millis() - lastReading > 1000){
        sensors.requestTemperatures();

        temp = round(sensors.getTempCByIndex(0));

        if(temp < 0){
            temp = 0;
        }
    
        heat();

        lastReading = millis();
    }
}