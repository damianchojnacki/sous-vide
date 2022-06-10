#ifndef CONFIG_H
    #include "./config.h"
#endif

#ifndef HELPERS_H
    #include "./helpers.h"
#endif

void initScreen()
{
    tft.init();
    tft.setRotation(0);

    uint16_t calData[5] = { 454, 3269, 454, 3269, 6 };
    tft.setTouch(calData);

    tft.setTextFont(4);
    tft.setTextSize(2);

    tft.fillScreen(TFT_WHITE);
    
    tft.setTextColor(TFT_BLACK);  // Adding a background colour erases previous text automatically
    tft.fillCircle(120, 120, 118, TFT_GREY);
    tft.fillCircle(120, 120, 114, TFT_BLACK);
}

void printTemp()
{
    tft.fillCircle(120, 120, innerCircleRadius, TFT_GREY);
    tft.fillCircle(120, 120, innerCircleRadius - 4, TFT_WHITE);

    String text = String(temp) + "`C";

    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK);
    tft.drawString(text, (240 - tft.textWidth(text)) / 2, 102);
}

void printTargetTemp()
{
    String text = String(targetTemp) + "`C";
    tft.setTextSize(1);
    tft.setTextColor(rgb(140, 140, 140), TFT_WHITE);
    tft.fillRect((240 - tft.textWidth(text)) / 2 - 10, 70, 60, 30, TFT_WHITE);
    tft.drawString(text, (240 - tft.textWidth(text)) / 2, 76);

    targetTempTextScreen = targetTemp;
}

void printTimeLeft()
{
    int hours = left / 60;
    int minutes = left - hours * 60;
    String zero = "";

    if(minutes < 10){
        zero = "0";
    }

    String text = String(hours) + ":" + zero + String(minutes);

    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString(text, (240 - tft.textWidth(text)) / 2, 262);
}

void fillCirclePart(float part, uint32_t color)
{
    int x = 122;
    int y = 122;
    int r1 = outerCircleRadius;
    int r2 = innerCircleRadius + 3;

    float degrees = map(part, 0, 100, 0, 360) + 90;

    int bx = r1 * cos(radians(degrees)) + x - 1;
    int by = r1 * sin(radians(degrees)) + y - 1;

    int ax = r2 * cos(radians(degrees)) + x - 2;
    int ay = r2 * sin(radians(degrees)) + y - 2;

    tft.drawWideLine(ax, ay, bx, by, 5, color);
}

void drawTargetTempCircle()
{
    float i = targetTempScreen;

    for(i; i <= targetTemp; i += 0.5){
        fillCirclePart(i, TFT_BLUE);
    }

    for(i; i > targetTemp; i -= 0.5){
        fillCirclePart(i, TFT_BLACK);
    }

    targetTempScreen = i;
}

void drawTempCircle()
{
    float i = tempScreen;

    for(i; i < temp; i += 0.1){ // 0.1
        int g = 200 - pow(i, 1.2);

        if(g > 255){
            g = 255;
        }

        if(g < 0){
            g = 0;
        }

        fillCirclePart(i, rgb(255, g, 0));
    }

    for(i; i > temp + 1; i -= 0.5){ // 0.5
        fillCirclePart(i, TFT_BLUE);
    }

    tempScreen = i;
}

void drawHeatingSymbol()
{    
    tft.setTextSize(1);

    if(heating){
        tft.setTextColor(TFT_RED, TFT_WHITE);
        tft.drawString("*", 10, 10);
    } else {
        tft.setTextColor(TFT_GREY, TFT_WHITE);
        tft.drawString("*", 10, 10);
    }

    tft.setTextColor(TFT_BLACK, TFT_WHITE);

    heatingScreen = heating;
}

void showPlusMinusButtons(){
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);

    tft.fillRoundRect(10, 260, 50, 50, 5, TFT_BLACK);
    tft.drawString("-", 27, 262);

    tft.fillRoundRect(180, 260, 50, 50, 5, TFT_BLACK);
    tft.drawString("+", 195, 262);

    tft.setTextColor(TFT_BLACK);
}

void hidePlusMinusButtons(){
    tft.fillRoundRect(10, 260, 50, 50, 5, TFT_WHITE);
    tft.fillRoundRect(180, 260, 50, 50, 5, TFT_WHITE);
}

void editDuration(){
    showPlusMinusButtons();

    tft.fillCircle(120, 120, innerCircleRadius, TFT_GREY);
    tft.fillCircle(120, 120, innerCircleRadius - 4, TFT_WHITE);

    tft.setTextSize(1);
    tft.setTextColor(TFT_BLACK);
    tft.drawString("START", (240 - tft.textWidth("START")) / 2, 112);

    editingDuration = true;
}

void editTemperature(){
    tft.fillRect(0, 260, 240, 50, TFT_WHITE);

    showPlusMinusButtons();

    tft.fillRoundRect(80, 260, 80, 50, 5,  rgb(30, 150, 30));

    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, rgb(30, 150, 30));
    
    tft.drawString("OK", (240 - tft.textWidth("OK")) / 2, 275);

    editingTemperature = true;
    tempScreen = 0;
}

void setTemperature(){
    editingTemperature = false;

    tft.fillRect(0, 260, 240, 50, TFT_WHITE);

    leftScreen = -1;
}

void start(){
    editingDuration = false;

    tempTextScreen = -1;
    targetTempTextScreen = -1;

    hidePlusMinusButtons();

    started = true;

    startTime = millis();
}

void stop()
{
    tft.fillRoundRect(65, 260, 110, 50, 5,  rgb(150, 30, 30));

    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, rgb(150, 30, 30));
    
    tft.drawString("KONIEC", (240 - tft.textWidth("KONIEC")) / 2, 275);

    started = false;
    stopped = true;
}

bool touchedInnerCircle(uint16_t x, uint16_t y)
{
    return pow(x - 120, 2) + pow(y - 120, 2) < pow(innerCircleRadius, 2);
}

bool touchedCircle(uint16_t x, uint16_t y)
{
    return pow(x - 120, 2) + pow(y - 120, 2) < pow(outerCircleRadius, 2);
}

bool touchedOuterCircle(uint16_t x, uint16_t y)
{
    return touchedCircle(x, y) && !touchedInnerCircle(x, y);
}

bool touchedPlus(uint16_t x, uint16_t y)
{
    return x > 180 && x < 230 && y > 260 && y < 310;
}

bool touchedMinus(uint16_t x, uint16_t y)
{
    return x > 10 && x < 60 && y > 260 && y < 310;
}

bool touchedOk(uint16_t x, uint16_t y)
{
    return x > 70 && x < 170 && y > 260 && y < 310 && editingTemperature;
}

bool touchedStop(uint16_t x, uint16_t y)
{
    return x > 70 && x < 170 && y > 260 && y < 310 && stopped;
}

bool touchedDuration(uint16_t x, uint16_t y)
{
    return x > 70 && x < 170 && y > 260 && y < 310 && !editingTemperature;
}

void handleTouch(uint16_t x, uint16_t y){
    if(touchedInnerCircle(x, y) && editingDuration){
        start();
        delay(500);
    } 
    
    else if(touchedInnerCircle(x, y) && !editingTemperature){
        editTemperature();
        delay(500);
    }

    else if(touchedOuterCircle(x, y)){
        if(!editingTemperature){
            editTemperature();
        }

        int degrees = findAngle(x, y, 120, 120, outerCircleRadius);

        targetTemp = map(degrees, 0, 360, 0, 100);
    }

    else if(touchedOk(x, y)){
        setTemperature();
        delay(500);
    }

    else if(touchedStop(x, y)){
        stopped = false;

        tft.fillRect(0, 260, 240, 50, TFT_WHITE);

        leftScreen = -1;

        delay(500);
    }

    else if(touchedDuration(x, y) && !editingDuration){
        editDuration();
    }

    if(touchedPlus(x, y) && editingTemperature){
        targetTemp++;
        delay(50);
    }

    if(touchedMinus(x, y) && editingTemperature){
        targetTemp--;
        delay(50);
    }

    if(touchedPlus(x, y) && editingDuration){
        duration++;
        delay(50);
    }

    if(touchedMinus(x, y) && editingDuration){
        duration--;
        delay(50);
    }
}

void draw()
{
    uint16_t x = 0, y = 0; 

    if(tft.getTouch(&x, &y)){
        handleTouch(x, y);
    }

    if(temp != tempTextScreen && !editingDuration){
        printTemp();
    }

    if((targetTempTextScreen != targetTemp && temp != targetTemp || tempTextScreen != temp) && !editingDuration){
        printTargetTemp();
    }

    if(heating != heatingScreen){
        drawHeatingSymbol();
    }

    tempTextScreen = temp;

    int elapsed = started ? (millis() - startTime) / 60000 : 0;
    left = duration - elapsed; 

    if(left < 0){
        left = 0;
    }

    if(left != leftScreen && !editingTemperature && !stopped){
        printTimeLeft();
    }

    leftScreen = left;

    if(started && elapsed >= duration && duration != 0 && !stopped){
        stop();
    }

    if(targetTemp != targetTempScreen){
        drawTargetTempCircle();
    }

    if(!editingTemperature){
        drawTempCircle();
    }
}
