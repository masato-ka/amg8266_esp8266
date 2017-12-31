#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include "SSD1306.h"

const int pixel_array_size = 8*8;

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

void drawRect(){
    for (int i = 0; i < 10; i++) {
      display.setColor(WHITE);
      display.setPixel(i, i);
      display.setPixel(10 - i, i);
    }
//    display.drawRect(12,12,20,20);
    display.setColor(BLACK);
    display.fillRect(12,12,20,20);
}

void black_rect(int x, int y, int size){
    display.setColor(BLACK);
    display.fillRect(x,y,x+size,y+size);
}

void white_rect(int x,int y, int size){
    display.setColor(WHITE);
    display.fillRect(x,y,x+size,y+size);
}



void draw_display(float pixels[]){
    int x=0;
    int y=0;
    int size = 10;
    for(int i =0; i < pixel_array_size; ++i){
        if(i%8==0){y+=size;x=0;}
        
        x += 10;
        if(pixels[i] >= 30.0){
            white_rect(x,y,size);
        }else{
            black_rect(x,y,size);
        }
    }

}

void loop() {
    amg.readPixels(pixels);
    Serial.println(" ");
    for(int i=0; i < pixel_array_size; ++i){
        if(i%8==0){Serial.print("\n");}
        Serial.print(pixels[i]);
        Serial.print(",");
    }
    delay(1000);
    display.clear();
    draw_display(pixels);
    display.display();
}