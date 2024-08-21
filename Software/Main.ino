// Arduino Code

//LED MATRIX Display 
#include <Wire.h>
const uint8_t addr = 0x70; // HT16K33 default address
uint16_t displayBuffer[8];
void setup() {
	Wire.begin();
	Wire.beginTransmission(addr);
	Wire.write(0x20 | 1); // turn on oscillator
	Wire.endTransmission();
	setBrightness(15);
	blink(0);
}
void loop() {
	const int dTime = 50;
	// Loop through all segments
	for(int i = 0; i < 8; i++){
		for(int k = 0; k < 16; k++){
		  displayBuffer[i] = 1 << k;
		  show();
		  delay(dTime);
		}
		clear();
	}
	// Turn on all segments, one at a time
	for(int i = 0; i < 8; i++){
		for(int k = 0; k < 16; k++){
			displayBuffer[i] |= 1 << k;
			show();
			delay(dTime);
		}
	}
	// Test blinking
	for(int i = 3 ; i > 0; i--){
		blink(i);
		delay(2000);
	}
	blink(0); // Turn blinking off
	// Test blanking
	for(int i = 0; i < 10; i++){
		blank();
		delay(dTime * 2);
	}
	// Test dimming
	for(int i = 15; i >= 0; i--){
		setBrightness(i);
		delay(dTime * 2);
	}
	clear();
	setBrightness(15);
}
void show(){
	Wire.beginTransmission(addr);
	Wire.write(0x00); // start at address 0x0
	for (int i = 0; i < 8; i++) {
		Wire.write(displayBuffer[i] & 0xFF);    
		Wire.write(displayBuffer[i] >> 8);    
	}
	Wire.endTransmission();  
}
void clear(){
	for(int i = 0; i < 8; i++){
		displayBuffer[i] = 0;
	}
}
void setBrightness(uint8_t b){
	if(b > 15) return;
	Wire.beginTransmission(addr);
	Wire.write(0xE0 | b); // Dimming command
	Wire.endTransmission();
}
void blank(){
	static boolean blankOn;  
	Wire.beginTransmission(addr);
	Wire.write(0x80 | blankOn); // Blanking / blinking command
	Wire.endTransmission();
	blankOn = !blankOn;
}
void blink(uint8_t b){
	if(b > 3) return;
	Wire.beginTransmission(addr);
	Wire.write(0x80 | b << 1 | 1); // Blinking / blanking command
	Wire.endTransmission();
}