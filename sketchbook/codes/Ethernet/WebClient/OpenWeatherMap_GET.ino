#include <Ethernet.h>
#include <SPI.h>
#include <stdlib.h>
#include "ArduinoJson.h"
#include "RestClient.h"

#define BUF_SIZE 450

//const char* APIKEY = "b011edc116685d89dfdc35517d9ce205";
//const char* CityID = "3169070";

RestClient client = RestClient("api.openweathermap.org");

void setup() {
	Serial.begin(115200);
	Serial.println("connect to network");
	
	byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
	byte ip[] = { 192, 168, 1, 20 };
	Ethernet.begin(mac,ip);
	
	Serial.println("Setup!");

	char* response = (char*) malloc(sizeof(char)*BUF_SIZE);
	if (response == NULL) {
		Serial.println("response allocation on heap FAIL");
		return;
	}
	response[0] = 0;
	
	const char* path = "/data/2.5/weather?id=3169070&units=metric&APPID=b011edc116685d89dfdc35517d9ce205";
	int statusCode = client.get(path, &response);
	Serial.print("Status code from server: ");
	Serial.println(statusCode);
	Serial.print("Response body from server: ");
	Serial.println(response);
	
	delay(10);
	
	DynamicJsonBuffer jsonBuffer(1024);
	JsonObject& root = jsonBuffer.parseObject(response);

	if (!root.success()) {
		Serial.println("parseObject() failed");
	}

	const char* location = root["name"];
	const char* country = root["sys"]["country"];
	float temperature = root["main"]["temp"];
	float humidity = root["main"]["humidity"];
	const char* weather = root["weather"]["main"];
	const char* description = root["weather"]["description"];
	float pressure = root["main"]["pressure"];
	
	Serial.print("Location: "); 
	Serial.println(location);
	Serial.print("Country: "); 
	Serial.println(country);
	Serial.print("Temperature: "); 
	Serial.print(temperature);
	Serial.println(" °C");
	Serial.print("Humidity: "); 
	Serial.print(humidity);
	Serial.println("%");
	Serial.print("Pressure: "); 
	Serial.print(pressure);
	Serial.println(" hPa");
	Serial.print("Weather: "); 
	Serial.println(weather);
	Serial.print("Description: "); 
	Serial.println(description);	

	free(response);

}

void loop() {}
