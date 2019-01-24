#include <M5Stack.h>

uint8_t PinGPIO = 39;

void setup(){
    M5.begin();
    M5.Lcd.setTextSize(2);
    M5.Lcd.print("Pin GPIO value: ");
    pinMode(PinGPIO, INPUT);
}

void loop(){
    M5.Lcd.setCursor(0, 16);
    if(digitalRead(PinGPIO)){
        M5.Lcd.print("ON ");
    } else{
        M5.Lcd.print("OFF");
    }
    delay(500);
    M5.update();
}
