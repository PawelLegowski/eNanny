#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>

/* Set these to your desired credentials. */
const char*   SETUP_SSID        = "eNanny";
String        ssid              = "";
String        password          = "";
bool          isSetupMode       = false;
ESP8266WebServer server(80);
IPAddress eNannyStaticIP(192,168,0,111);
IPAddress gateway(192,168,0,1);
IPAddress mask(255,255,255,0);

void setSetupMode(bool bIsSetupMode);

//---  ___ ___ ___ ___  ___  __  __  ---
//--- | __| __| _ \ _ \/ _ \|  \/  | ---
//--- | _|| _||  _/   / (_) | |\/| | ---
//--- |___|___|_| |_|_\\___/|_|  |_| ---   
bool EEPROM_isInitialised()
{
  return EEPROM.read(200) == 1;
}

void EEPROM_setInitialised()
{
  EEPROM.write(200,1);
}


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
  char strln = aSSID.length();  
  EEPROM.put(0, strln);
  for(char i = 0; i < strln; i++)
  {
    EEPROM.put(i+1, aSSID[i]);
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
  char strln = aPassword.length();  
  EEPROM.put(64, strln);
  for(char i = 0; i < strln; i++)
  {
    EEPROM.put(i+65, aPassword[i]);
  }
}


//--- __      _____ ___   ___ ___ _____   _____ ___  ---
//--- \ \    / / __| _ ) / __| __| _ \ \ / / __| _ \ ---
//---  \ \/\/ /| _|| _ \ \__ \ _||   /\ V /| _||   / ---
//---   \_/\_/ |___|___/ |___/___|_|_\ \_/ |___|_|_\ ---
/* Go to http://192.168.4.1 in a web browser */ 
void handleRoot() {
  //sending HTML website with input form

 //--look for avalible networks - hubert 20-02-17

  // WiFi.scanNetworks will return the number of networks found
  String box = "<select name=\"ssid\"><option selected=\"selected\" value=\"\">Select Network</option>";
String option = "";
String endbox = "</select>"; 

  int n = WiFi.scanNetworks();
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
    option = option + "<option value=\"" + WiFi.SSID(i) + "\">"+ WiFi.SSID(i) + "</option>";
    
      delay(10);
    }
  }

  
  //--end
 
 String networkList =box+option+endbox;
  String NetworkSelectForm = "<form method=\"post\" action=\"login\">SSID:" + EEPROM_readSSID() + "<br>" + networkList + "<br>Password:<br><input name=\"password\"><br><input type=\"submit\" value=\"Save\"></form><br>";
  server.send(200, "text/html", NetworkSelectForm);
}

void handleLogin()
{
   
  ssid = server.arg("ssid");  
  password = server.arg("password");
  
  Serial.println(ssid);
  Serial.println(password);

  EEPROM_writeSSID(ssid);
  EEPROM_writePassword(password);
  EEPROM_setInitialised();
  EEPROM.commit();
  //sending HTML website with confirmation
  String DataRecieved = "Saved network:" + EEPROM_readSSID() + " pass:" +  EEPROM_readPassword();
  server.send(200, "text/html", DataRecieved);
  
  setSetupMode(false);
}

void setupServer()
{  
  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.begin();
}

//--- __      ___ ___ _  ---
//--- \ \    / (_) __(_) ---
//---  \ \/\/ /| | _|| | ---
//---   \_/\_/ |_|_| |_| ---               
void setSetupMode(bool bIsSetupMode)
{
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  if(bIsSetupMode)
  {
    WiFi.softAPConfig(eNannyStaticIP, gateway, mask);   
    WiFi.softAP(SETUP_SSID);  
  }
  else
  {    
      

    Serial.println("CONNECTING");
    Serial.println(ssid.c_str());
    Serial.println(password.c_str()); 
    WiFi.config(eNannyStaticIP, gateway, mask);   
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

//---  __  __   _   ___ _  _   ___ _    _____      __ ---
//--- |  \/  | /_\ |_ _| \| | | __| |  / _ \ \    / / ---
//--- | |\/| |/ _ \ | || .` | | _|| |_| (_) \ \/\/ /  ---
//--- |_|  |_/_/ \_\___|_|\_| |_| |____\___/ \_/\_/   ---


const int buttonPin = 5;     // the number of the pushbutton pin
const int ledPin =  4;      // the number of the LED pin

// variables will change:
int buttonState = 0;


void setup() {  

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);


  
  delay(1000);
  
  Serial.begin(115200);
  Serial.println("siemano");

//--look for avalible networks - hubert 20-02-17
// Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  Serial.println("Setup done");

//--end
  
  EEPROM.begin(512);  
  ssid                = EEPROM_readSSID();
  password            = EEPROM_readPassword();

  setSetupMode(EEPROM_isInitialised() == false);
  setupServer();
}
void loop() {
      server.handleClient();
}
