#include <stdio.h>
#include <stdlib.h>
#include <ArduinoJson.h>

#define BUF_SIZE 450

void setup() {
	Serial.begin(115200);
	while(!Serial);
	
	DynamicJsonBuffer jsonBuffer;

	// JSON input string
	// It's better to use a char[] as shown here.
	// If you use a const char* or a String, ArduinoJson will
	// have to make a copy of the input in the JsonBuffer.
	
	const char* original = "{\"coord\":{\"lon\":12.48,\"lat\":41.89},\"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"01n\"}],\"base\":\"stations\",\"main\":{\"temp\":9.99,\"pressure\":1005,\"humidity\":71,\"temp_min\":9,\"temp_max\":11},\"visibility\":10000,\"wind\":{\"speed\":3.1,\"deg\":140},\"clouds\":{\"all\":0},\"dt\":1517955600,\"sys\":{\"type\":1,\"id\":5848,\"message\":0.0036,\"country\":\"IT\",\"sunrise\":1517897779,\"sunset\":1517934758},\"id\":3169070,\"name\":\"Roma\",\"cod\":200}";
	char* json = (char*) malloc(sizeof(char)*BUF_SIZE);
	sprintf(json, "%s", original);
	Serial.println(json);
	
	// Root of the object tree
	// It's a reference to the JsonObject, the actual bytes are inside the
	// JsonBuffer with all the other nodes of the object tree.
	// Memory is freed when jsonBuffer goes out of scope.
	JsonObject& root = jsonBuffer.parseObject(json);

	// Test if parsing succeeds
	if (!root.success()) {
		Serial.println("parseObject() failed");
		return;
	}
  
	// Fetch values
	
	float coord_lon = root["coord"]["lon"];
	float coord_lat = root["coord"]["lat"];
	
	JsonObject& weather0 = root["weather"][0];
	int weather0_id = weather0["id"];
	const char* weather0_main = weather0["main"];
	const char* weather0_description = weather0["description"];
	const char* weather0_icon = weather0["icon"];
	
	const char* base = root["base"];
	
	JsonObject& main = root["main"];
	float main_temp = main["temp"];
	int main_pressure = main["pressure"];
	int main_humidity = main["humidity"];
	int main_temp_min = main["temp_min"];
	int main_temp_max = main["temp_max"];

	const char* name = root["name"];
	int cod = root["cod"];
	
	Serial.println(coord_lat);
	Serial.println(coord_lon);
	Serial.println(weather0_id);
	Serial.println(weather0_main);
	Serial.println(weather0_description);
	Serial.println(weather0_icon);
	Serial.println(base);
	Serial.println(main_temp);
	Serial.println(main_pressure);
	Serial.println(main_humidity);
	Serial.println(main_temp_min);
	Serial.println(main_temp_max);
	Serial.println(name);
	Serial.println(cod);
}

void loop() {}
