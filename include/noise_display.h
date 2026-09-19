// noise_display.h
#pragma once
#include <Adafruit_NeoPixel.h>
 
extern Adafruit_NeoPixel strip;

void setup_noise_display();
void show_noise_level(int pixels);  
