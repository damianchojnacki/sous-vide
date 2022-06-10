#ifndef CONFIG_H
    #include "./config.h"
#endif

bool shouldHeat()
{
    if(heating){
        heatingTimeout = millis() - heatingStartedAt;
        inertiaTemp = heatingStartedAt > 0 ? heatingTimeout / 1000 * inertiaFactor : 0;
    }

    tft.setTextSize(1);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString(String(inertiaTemp), 205, 10);

    return temp + round(inertiaTemp) < targetTemp && !stopped;
}

void resetHeatingTimeout()
{
    heatingStartedAt = 0;

    inertiaTemp -= inertiaFactor;
}

void heat()
{
    if(shouldHeat()){
        if(heatingStartedAt == 0){
            heatingStartedAt = millis();
        }

        heating = true;
        digitalWrite(COIL_PIN, HIGH);
    } else {
        heating = false;
        digitalWrite(COIL_PIN, LOW);

        resetHeatingTimeout();
    }
}