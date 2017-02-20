#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>

/* Set these to your desired credentials. */
const char*   SETUP_SSID        = "eNanny";
String        ssid              = "";
String        password          = "";
bool          isSetupMode       = false;
bool          test              = HIGH;
ESP8266WebServer server(80);

//--- __      ___ ___ _  ---
//--- \ \    / (_) __(_) ---
//---  \ \/\/ /| | _|| | ---
//---   \_/\_/ |_|_| |_| ---               
void setSetupMode(bool bIsSetupMode)
{
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  if(bIsSetupMode)
  {
    WiFi.softAP(SETUP_SSID);  
  }
  else
  {    
    Serial.println("CONNECTING");
    Serial.println(ssid.c_str());
    Serial.println(password.c_str());
    WiFi.begin(ssid.c_str(), password.c_str());
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


//--- __      _____ ___   ___ ___ _____   _____ ___  ---
//--- \ \    / / __| _ ) / __| __| _ \ \ / / __| _ \ ---
//---  \ \/\/ /| _|| _ \ \__ \ _||   /\ V /| _||   / ---
//---   \_/\_/ |___|___/ |___/___|_|_\ \_/ |___|_|_\ ---
/* Go to http://192.168.4.1 in a web browser */ 
void handleRoot() {
  //sending HTML website with input form
  server.send(200, "text/html", "<form method=\"post\" action=\"login\">SSID:<br><input name=\"ssid\"><br>Password:<br><input name=\"password\"><br><input type=\"submit\" value=\"Save\"></form>");
}

void handleLogin()
{
  ssid = server.arg("ssid");  
  password = server.arg("password");
  
  Serial.println(ssid);
  Serial.println(password);

  EEPROM_writeSSID(ssid);
  EEPROM_writePassword(password);   
  EEPROM.commit();
  //sending HTML website with confirmation
  server.send(200, "text/html", "Data Saved!");
  setSetupMode(false);
}


//---  ___ ___ ___ ___  ___  __  __  ---
//--- | __| __| _ \ _ \/ _ \|  \/  | ---
//--- | _|| _||  _/   / (_) | |\/| | ---
//--- |___|___|_| |_|_\\___/|_|  |_| ---                        
String EEPROM_readSSID()
{
  char strln;
  char letter;  
  String ans = "";
  EEPROM.get(0, strln);
  for(char i = 0 ; i < strln; i++)
  {
    EEPROM.get(i+1, letter);
    ans += letter;
  }
  return ans;
}

void EEPROM_writeSSID(String aSSID)
{
  char strln = ssid.length();  
  EEPROM.put(0, strln);
  for(char i = 0; i < strln; i++)
  {
    EEPROM.put(i+1, ssid[i]);
  }
}

String EEPROM_readPassword()
{
  char strln;
  char letter;  
  String ans = "";
  EEPROM.get(64, strln);
  for(char i = 0 ; i < strln; i++)
  {
    EEPROM.get(i+65, letter);
    ans += letter;
  }
  return ans;
}

void EEPROM_writePassword(String aPassword)
{
  char strln = ssid.length();  
  EEPROM.put(64, strln);
  for(char i = 0; i < strln; i++)
  {
    EEPROM.put(i+65, ssid[i]);
  }
}


//---  __  __   _   ___ _  _   ___ _    _____      __ ---
//--- |  \/  | /_\ |_ _| \| | | __| |  / _ \ \    / / ---
//--- | |\/| |/ _ \ | || .` | | _|| |_| (_) \ \/\/ /  ---
//--- |_|  |_/_/ \_\___|_|\_| |_| |____\___/ \_/\_/   ---
void setup() {  
  delay(1000);
  Serial.begin(115200);
  
  EEPROM.begin(512);  
  ssid                = EEPROM_readSSID();
  password            = EEPROM_readPassword();

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

void loop() {
	server.handleClient();
}
