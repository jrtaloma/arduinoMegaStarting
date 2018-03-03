// https://www.logicaprogrammabile.it/webduino-recuperare-parametri-da-url/

#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"

int pinLed = 5;

static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

static uint8_t ip[] = { 192, 168, 1, 20 };

/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */
#define PREFIX ""
WebServer webserver(PREFIX, 80);

void startCmd(WebServer &server, WebServer::ConnectionType type, char *, bool) {
	/* this line sends the standard "200 OK" back to the browser */
	server.httpSuccess();

	if (type == WebServer::GET) {
		/* this defines some HTML text in read-only memory aka PROGMEM.
		* This is needed to avoid having the string copied to our limited
		* amount of RAM. */
		P(helloMsg) = "<h1>Let's play with your Arduino LED !</h1>";

		/* this is a special form of print that outputs from PROGMEM */
		server.printP(helloMsg);
	}
}

void turnLED(WebServer &server, WebServer::ConnectionType type, char* url_param, bool param_complete) {
	server.httpSuccess();
	
	if (type == WebServer::GET) {
		if (param_complete == true) {
			String param = url_param;
			server.print(param);
			if (param == "LED=ON") digitalWrite(pinLed, HIGH);
			else if (param == "LED=OFF") digitalWrite(pinLed, LOW);
		}
	}
}

void setup() {
	pinMode(pinLed, OUTPUT);
	digitalWrite(pinLed, LOW);
	
	Ethernet.begin(mac, ip);

	/* setup our default command that will be run when the user accesses
	* the root page on the server */
	webserver.setDefaultCommand(&startCmd);

	/* run the same command if you try to load /index.html, a common
	* default page name */
	webserver.addCommand("index.html", &startCmd);

	/* this will turn ON/OFF pinLed accordingly to LED paramater */
	webserver.addCommand("light", &turnLED);

	/* start the webserver */
	webserver.begin();
}

void loop() {
	/* process incoming connections forever */
	webserver.processConnection();
}
