#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include "SSD1306.h"

const int pixel_array_size = 8*8;
const float thd = 29.0;
float pixels[pixel_array_size];
Adafruit_AMG88xx amg;
SSD1306  display(0x3c, 4, 5);


void setup() {
    Serial.begin(115200);
    bool status = amg.begin(0x68);
    Serial.println("Init");
    Serial.println("-- Thermal Camera Test --");
    if(!status){
        Serial.println("AMG88xx is not available. Please connect the sensor.");
        while(1);
    }
  display.init();
  display.flipScreenVertically();
}


void black_rect(int x, int y, int size){
    display.drawLine(x,y,x,y+size);
    display.drawLine(x,y,x+size,y);
    display.drawLine(x+size,y,x+size,y+size);
    display.drawLine(x,y+size,x+size,y+size);
}

void white_rect(int x,int y, int size){
    display.setColor(WHITE);
    display.fillRect(x,y,size,size);
}



void draw_display(float pixels[], float thd){
    int x=0;
    int y=0;
    int size = 5;
    for(int i =0; i < pixel_array_size; ++i){
        if(i%8==0){y+=size;x=0;}
        x = size * (i%8);
        if(pixels[i] >= thd){
            Serial.print(y);
            Serial.print(",");
            white_rect(x,y,size);
        }else{
            Serial.print(y);
            Serial.print(",");
            black_rect(x,y,size);
        }
    }
    Serial.println("");

}

void loop() {
    amg.readPixels(pixels);
    Serial.println(" ");
    delay(1000);
    display.clear();
    draw_display(pixels, thd);
    display.display();
}