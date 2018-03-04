#include <Wire.h>

/* ATTENTION: I2C library has a 32-byte buffer limitation */
/* You need to split longer messages */

#define BUF_SIZE 32

int slaveAddress = 8;
char buffer[BUF_SIZE];
volatile int size = 0;

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany){
	while (Wire.available()){
		char c = Wire.read();
		buffer[size] = c;
		if (c == '\n' || c == 0){
			Serial.println(buffer);
			size = 0;
			return;
		}
		++size;
		buffer[size] = 0;
		if(size == BUF_SIZE-1){  // end of buffer
			while(c != 0) c = Wire.read(); // read all incoming chars without storing in buffer: they are lost
			return;
		}
	}
}

void setup(){
	buffer[0] = 0;
	Serial.begin(115200);
	Wire.begin(slaveAddress);
	Wire.onReceive(receiveEvent);
}

void loop(){

}
