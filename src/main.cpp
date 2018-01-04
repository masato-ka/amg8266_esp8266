#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> // HTTPClient

#include <Adafruit_AMG88xx.h>
#include "SSD1306.h"

const int pixel_array_size = 8*8;
const float thd = 29.0;
float pixels[pixel_array_size];
Adafruit_AMG88xx amg;
SSD1306  display(0x3c, 4, 5);

String ssid = "";
String password = "";


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

//    WiFi.begin(ssid, password);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

}


void black_rect(int x, int y, int size){
    display.drawLine(x,y,x,y+size);
    display.drawLine(x,y,x+size,y);
    display.drawLine(x+size,y,x+size,y+size);
    display.drawLine(x,y+size,x+size,y+size);
    display.drawRect(x,y,size,size);
}

void circle(int x, int y, int radius){
    int r = radius/2;
    display.drawCircle(x+r, y+r, r);
}

void cross(int x, int y, int size){
    
    for(int i=0; i < size; ++i){
        display.setPixel(x+i, y+i);
        display.setPixel(x+size-i, y+i);
    }
}

void white_rect(int x,int y, int size){
    display.setColor(WHITE);
    display.fillRect(x,y,size,size);
}



void draw_display(float pixels[], float thd){
    int x=0;
    int y=0;
    int size = 7;
    for(int i =0; i < pixel_array_size; ++i){
        if(i%8==0){y+=size;x=0;}
        x = size * (i%8);
        if(pixels[i] > 24 && pixels[i] <= 29){
            cross(x,y,size);
        }else if(pixels[i] > 29 && pixels[i] <= 34){
            circle(x,y,size);
        
        }else if(pixels[i] > 34){
            white_rect(x,y,size);
        }else{
            black_rect(x,y,size);
            
        };

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
    String payload="[";
    for(int index=0; index < pixel_array_size; index++){
        payload += pixels[index];
        if(index != 63){
            payload += ",";
        }
    }
    payload += "]";
    Serial.println(payload);
    HTTPClient http;
    http.begin("http://localhost:8080/api/v1/thermography");
    http.addHeader("Content-Type","application/json");
    http.POST(payload);

}