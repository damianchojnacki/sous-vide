#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <WiFiManager.h>
#include <AsyncElegantOTA.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "Free_Fonts.h" 

#define CONFIG_H

AsyncWebServer server(81);

WiFiManager wm;

#define TFT_GREY 0x5AEB

#define SENSORS_BUS 27

#define COIL_PIN 33

OneWire oneWire(SENSORS_BUS);
DallasTemperature sensors(&oneWire);

TFT_eSPI tft = TFT_eSPI();

int targetTemp = 50;
int temp = 0;
bool heating = false;
bool heatingScreen = true;

float inertiaTemp = 0;
float inertiaFactor = 0.005;
unsigned long heatingStartedAt = 0;
unsigned long heatingTimeout = 0;

int targetTempScreen = 0;
int tempScreen = 0;
int tempTextScreen = -1;
int targetTempTextScreen = -1;

int editingTemperature = false;

int startTime = 0;
int duration = 0;
int left = 0;
int leftScreen = -1;

int editingDuration = false;

int started = false;
int stopped = false;

int outerCircleRadius = 112;
int innerCircleRadius = 68;