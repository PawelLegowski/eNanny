#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "eNanny";

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
	server.send(200, "text/html", "SSID:<br><input type=\"text\" name=\"SSID\"><br>PASSWORD:<br><input type=\"text\" name=\"PASSWORD\"><br><button href=\"/login\">OK</button>");
}

void handleLogin()
{
  String ssid = server.arg("ssid");
  Serial.print(ssid);
}

void setup() {
	delay(1000);
	Serial.begin(115200);
	Serial.println();
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);
  server.on("/login", handleLogin);
	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	server.handleClient();
}
