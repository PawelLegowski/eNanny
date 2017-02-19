#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>

/* Set these to your desired credentials. */
const char*   SETUP_SSID        = "eNanny";
const char*   ssid              = "";
const char*   password          = "";
bool          isSetupMode       = false;
bool          test              = HIGH;
ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
  //sending HTML website with input form
  server.send(200, "text/html", "<form method=\"post\" action=\"login\">SSID:<br><input name=\"ssid\"><br>Password:<br><input name=\"password\"><br><input type=\"submit\" value=\"Save\"></form>");
}

void handleLogin()
{
  ssid = server.arg("ssid").c_str();  
  password = server.arg("password").c_str();
  
  Serial.println(ssid);
  Serial.println(password);
  
  EEPROM.put(0, ssid);
  EEPROM.put(64, password);
  EEPROM.commit();
  //sending HTML website with confirmation
  server.send(200, "text/html", "Data Saved!");
  setSetupMode(false);
}

void setup() {  
	delay(1000);
  Serial.begin(115200);
  
  EEPROM.begin(512);
  EEPROM.get(0, ssid);
  EEPROM.get(64, password);

  Serial.println("");
  Serial.println("siemandero");
  Serial.println(ssid);
  Serial.println(password);

  setSetupMode(true);
	/* You can remove the password parameter if you want the AP to be open. */
	//IPAddress myIP = WiFi.softAPIP();
	//Serial.print("AP IP address: ");
	//Serial.println(myIP);
	server.on("/", handleRoot);
  server.on("/login", handleLogin);
	server.begin();
	//Serial.println("HTTP server started");
}

void setSetupMode(bool bIsSetupMode)
{
  WiFi.disconnect();
  if(bIsSetupMode)
  {
    WiFi.softAP(SETUP_SSID);  
  }
  else
  {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
	server.handleClient();
}
