#include <stdint.h>
#include <stdlib.h>
#include <Wire.h>

/* ATTENTION: I2C library has a 32-byte buffer limitation */
/* You need to split longer messages */

int slaveAddress = 8;

void setup(){
	Wire.begin();
}

void loop(){
	char* buffer = "Hello world";
	
	Wire.beginTransmission(slaveAddress);
	Wire.write((const uint8_t*)buffer, strlen(buffer));
	Wire.endTransmission();
	
	// add string terminator
	Wire.beginTransmission(slaveAddress);
	Wire.write(0);
	Wire.endTransmission();
	
	delay(500);
}
