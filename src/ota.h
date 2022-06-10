#ifndef CONFIG_H
    #include "./config.h"
#endif

void startOta()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", "Hi! This is ESP32 Sous Vide Web Server. Please visit <a href=\"http://" + WiFi.localIP().toString() + ":81/update\">/update</a> to update firmware.");
    });

    server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/text", "Temperature: " + String(sensors.getTempCByIndex(0)) + "°C");
    });

    AsyncElegantOTA.begin(&server);

    server.begin();

    Serial.println("ElegantOTA server started on port 81.");
}