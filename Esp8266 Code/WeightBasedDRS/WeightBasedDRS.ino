#include <ESP8266WiFi.h> 
#include <ArduinoJson.h>

//needed for wifi manager library
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>                    //https://github.com/tzapu/WiFiManager

//needed for eeprom access
#include <EEPROM.h>
#include <Arduino.h>
#include "EEPROMAnything.h"

#include "HX711.h"

HX711 scale(12, 14);    // parameter "gain" is ommited; the default value 128 is used by the library

float calibration_factor = 461.00; //460 worked for my 200gm scale s

//AMAZON DRS parameters
const char* device_model = "OmegaJar"; //Product model ID of your device 
const char* device_identifier = "OmegaJarByESP8266"; // Serial No of device any thing you wish at production end
const char* client_id = "amzn1.application-oa2-client.a94c17d6686bfd692f8ea"; // your Amazon developer account client ID
const char* client_secret = "744e088c93d539062a5d6bca77fb7476edbcc"; // Your Amazon developer account secret code 
const char* slot_id = "be3e0bdf-6-46a0-b1a3bad1"; //Sloat ID for your product
const char* redirect_uri = "https%3A%2F%2Falpha-amol.github.io"; //Encoded Return URL should be encoded and same as used at time of device creation

String authorization_grant_code = "ANXLDeHRZEPokem"; //ex: ANQEgiAOjkQWjKvhNWIN
String refresh_Token = "Atzr|IwEFaVys9_s4Jsc-dRKgC2CPmrSNNRhAFuwoe70mY-WWouoEotZUFj3XoaOXNHsYhD5Hczc148HylHRASgWizpMd8RIEYU_ZlGoTbMNTWSH1lJ1BV7XzV_hGcwZ25QtY7RQSQPXXNJ8NRvn54X2yC4jZNw-EA3LpcCpUc8Ux0r2Qus3UL67gj2q0MfJ1BgLNaDWTKD7UF3W1zMCWvfVzBEvYN-Obv5U4IThHedcRePb4g2NkFOIH-wXJIGl6hmdV5_KugiC4GzKzKnbC1i4raGapi084rJZuGfPyYCgi2kW_PEInHbKRmphQN5P1kCJqwJqP6kP0G-rWbb9h0GXzi6inQLPswKr8aDp9ZofKBe4caxl1euz7OTaXtWTjdIa2Yyl5cIporgSi1-fXHc3JEHlwU2XFn_0";
String access_Token = "";

#define AP_PIN    D0 //Active LOW--2 ESP8266 as Access Point

int threshold = 100; // Threshold in grams after which replenishment

unsigned long previousMillisTemp = 0;
const long intervalTemp = 30000;

void wifi_connect() {
  Serial.println(F(""));
  Serial.println(F("******************************WIFI Connecting*******************************************"));

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(F("."));
    delay(1000);
  }

  Serial.println(F(""));
  Serial.println(F("Wi-Fi Connected!!"));
  Serial.println(F("***************************************************************************************"));
}

void config_ap_auth(void) 
{
  char auth_temp[30]="";
  String temp1="",temp2="";
  byte val;
  eepromReadString(0, 30, auth_temp);
  temp1 = auth_temp;
  //WiFiManager
  WiFiManagerParameter custom_auth_code("Authorization", "authorization code", auth_temp, 25);
  WiFiManagerParameter custom_text("<br/>DRS Authorization Code");
  
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  wifiManager.addParameter(&custom_text);
  wifiManager.addParameter(&custom_auth_code);
  if (!wifiManager.startConfigPortal("OmegaJar")) 
  {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }
  strcpy(auth_temp, custom_auth_code.getValue());
  Serial.println(auth_temp);
  temp2 = auth_temp;
  
  if(auth_temp[0] != 0x00)
  {
    eepromWriteString(0,21, auth_temp);
    EEPROM.commit();
    Serial.println("Auth code received");
    Serial.println(auth_temp);

    if (temp1.equals(temp2)) 
    {
      val = 0;
      Serial.println("Equals");
    }
    else
    {
      val = 1;
      Serial.println("Not Equals");
    }
    EEPROM.write(40, val);
    EEPROM.commit();
  }
  else
  {
    Serial.println("Auth code not Supplied");
  }
  //if you get here you have connected to the WiFi
  Serial.println("AP and Auth config done :)");
  
}

WiFiClientSecure client;
 
void setup() 
{
  Serial.begin(9600); //  Initlize UART for Serial debug
  EEPROM.begin(2048);   // Initlize EEPROM with 2K Bytes
   
  Serial.println(F("\n OmegaJar- Poweredby Amazon DRS"));

  pinMode(AP_PIN,   INPUT_PULLUP);
  
  scale.set_scale(0);         // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  
  Serial.println(F("Press CONFIG"));
  for(int cnt=0; cnt<=500; cnt++)
  {
    if ( digitalRead(AP_PIN) == HIGH ) 
    {
      Serial.println("SSID: OmegaJar");
      Serial.println("URL: 192.168.4.1");
      config_ap_auth();
    }
    delay(25);
  }
  
  Serial.println(F(""));
  Serial.println(F("Connecting to WiFi"));

  wifi_connect();
  
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
}

void loop()
{
  byte c = EEPROM.read(40);
  Serial.println(c);
  
  if (c)
  {
  Serial.println(F(""));
  Serial.println(".............Access and Refresh Token from Auth Code.............");
  unsigned int x = obtain_access_and_refresh_token();
  Serial.println(x);
  Serial.println("Connected to AP......and DRS Enabled...");    
  c = 0;
  delay(4000);
  }

   unsigned long currentMillisTemp = millis();
  if(currentMillisTemp - previousMillisTemp >= intervalTemp) 
  {
    float weight = 0.0, temp = 0.0;
    for (int i = 0; i < 5; i++)
    {
      temp = scale.getGram();
      if (temp < 0)
      {
        temp = 0;
      }
      weight = weight + temp;
    }
    weight = weight/5;    
    
    Serial.print("Reading:- ");
    Serial.print(weight);
    Serial.println(" Gram");
    
    if (weight < threshold)
    {
      Serial.println(F(""));
      Serial.println(".............Access from Refresh Token.............");
      unsigned int y = obtain_access_token();

      delay(3000); //wait a moment for the authorization code grant to complete the exchange
      
      Serial.println(F(""));
      Serial.println(".............REPLENISHING Jar..............");
      unsigned int z=end_point_request_replenishment(slot_id);
    }
  }

}

String assembleAuthCodeBody(String authCode, String clientId, String clientSecret, String redirectUri)
{
   String tempTokenBody = "";
    
   tempTokenBody += "grant_type=authorization_code";
   tempTokenBody += "&";
   tempTokenBody += "code=";
   tempTokenBody += authorization_grant_code;
   tempTokenBody += "&";
   tempTokenBody += "client_id=";
   tempTokenBody += client_id;
   tempTokenBody += "&";
   tempTokenBody += "client_secret=";
   tempTokenBody += client_secret;
   tempTokenBody += "&";
   tempTokenBody += "redirect_uri=";
   tempTokenBody += redirect_uri;

   return tempTokenBody;
}

unsigned int obtain_access_and_refresh_token() 
{
  char _tmp_token[500];
  unsigned int _len_str;
  Serial.println();
  Serial.println("OBTAIN ACCESS AND REFRESH TOKEN FROM AUTH CODE:\r\nConnecting to Host: api.amazon.com");
  _len_str = eepromReadString(0, 30, _tmp_token);
  Serial.println(_tmp_token);

  authorization_grant_code = _tmp_token;
  
  //assemble the body
  String authCodeBody = assembleAuthCodeBody(authorization_grant_code, client_id, client_secret, redirect_uri);
  // Assemble POST request:
  //----------------------------------------------------------------
  //Request for Bearer and Refresh Tokens
  //----------------------------------------------------------------

  // Use WiFiClientSecure class to create TCP connections
  if (!client.connect("api.amazon.com", 443)) {
    Serial.println("Error 5 --> Connection failed");
    return 5;
  }
  client.flush();
  Serial.println("Connected\r\nSending Request for obtain Tokens");
  // This will send request for refresh of Access token to the server
  client.println("POST /auth/o2/token HTTP/1.1");
  client.println("Host: api.amazon.com"); 
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println("Cache-Control: no-cache");
  client.print("Content-Length: ");
  client.println(authCodeBody.length());
  client.println();
  client.println(authCodeBody);
  Serial.println("Request Send");
  
  unsigned long timeout = millis();
  
  while (client.available() == 0) 
  {
    if (millis() - timeout > 30000) 
    {
      Serial.println("Error 4 --> Connection Timeout !");
      client.stop();
      return 4;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  String _line = "temp";
  unsigned int _line_no = 1;
  
  while(client.available())
  {
    _line = client.readStringUntil('\r');
    //Serial.print(_line);
    if ((_line_no == 1)&& (_line != "HTTP/1.1 200 OK"))
    {
      Serial.println("Error 3 --> Invalid Response");
      client.stop();
      return 3;
    }
    _line_no++;
  }
  
  DynamicJsonBuffer jsonBuffer;
  // Responce last line have access token and referesh token json oblect 
  JsonObject& root = jsonBuffer.parseObject(_line);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("Error 2 --> Json parseObject() failed");
    return 2;
  }  
  Serial.println();
  
  const char* access_token = root["access_token"];
  Serial.print("Access token: ");
  Serial.println(access_token);
  access_Token = access_token;
  Serial.println(access_Token);
  
  const char* _refresh_token = root["refresh_token"];
  Serial.print("Refresh token: ");
  Serial.println(_refresh_token);
  refresh_Token = _refresh_token;
  Serial.println(refresh_Token);

  int len_str = eepromWriteString1(100, 500, _refresh_token);
  EEPROM.commit();
  //Serial.print("Length: ");
  //Serial.println(len_str);

  return 1;
}


String assembleRefreshTokenBody(String refresh_token, String clientId, String clientSecret, String redirectUri)
{
   String tempTokenBody = "";
    
   tempTokenBody += "grant_type=refresh_token";
   tempTokenBody += "&";
   tempTokenBody += "refresh_token=";
   tempTokenBody += refresh_token;
   tempTokenBody += "&";
   tempTokenBody += "client_id=";
   tempTokenBody += client_id;
   tempTokenBody += "&";
   tempTokenBody += "client_secret=";
   tempTokenBody += client_secret;
   tempTokenBody += "&";
   tempTokenBody += "redirect_uri=";
   tempTokenBody += redirect_uri;

   return tempTokenBody;
}

unsigned int obtain_access_token() 
{
  char _tmp_token[500];
  unsigned int _len_str;
  Serial.println();
  Serial.println("REQUEST NEW ACCESS TOKEN BY PROVIDING REFRESH TOKEN:\r\nConnecting to Host: api.amazon.com");
  _len_str = eepromReadString(100, 500, _tmp_token);
  Serial.println(_tmp_token);

  refresh_Token = _tmp_token;
  
  //assemble the body
  String authCodeBody = assembleRefreshTokenBody(refresh_Token, client_id, client_secret, redirect_uri);
  // Assemble POST request:
  //----------------------------------------------------------------
  //Request for Bearer and Refresh Tokens
  //----------------------------------------------------------------

  // Use WiFiClientSecure class to create TCP connections
  if (!client.connect("api.amazon.com", 443)) {
    Serial.println("Error 5 --> Connection failed");
    return 5;
  }
  client.flush();
  Serial.println("Connected\r\nSending Request for obtain Tokens");
  // This will send request for refresh of Access token to the server
  client.println("POST /auth/o2/token HTTP/1.1");
  client.println("Host: api.amazon.com"); 
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println("Cache-Control: no-cache");
  client.print("Content-Length: ");
  client.println(authCodeBody.length());
  client.println();
  client.println(authCodeBody);
  Serial.println("Request Send");
  
  unsigned long timeout = millis();
  
  while (client.available() == 0) 
  {
    if (millis() - timeout > 30000) 
    {
      Serial.println("Error 4 --> Connection Timeout !");
      client.stop();
      return 4;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  String _line = "temp";
  unsigned int _line_no = 1;
  
  while(client.available())
  {
    _line = client.readStringUntil('\r');
    //Serial.print(_line);
    if ((_line_no == 1)&& (_line != "HTTP/1.1 200 OK"))
    {
      Serial.println("Error 3 --> Invalid Response");
      client.stop();
      return 3;
    }
    _line_no++;
  }
  
  DynamicJsonBuffer jsonBuffer;
  // Responce last line have access token and referesh token json oblect 
  JsonObject& root = jsonBuffer.parseObject(_line);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("Error 2 --> Json parseObject() failed");
    return 2;
  }  
  
  const char* access_token = root["access_token"];
  Serial.print("Access token: ");
  access_Token = access_token;
  Serial.println(access_Token);
  Serial.println("SUCCESS");
  
  int len_str = eepromWriteString1(600, 500, access_token);
  EEPROM.commit();
  //Serial.print("Length: ");
  //Serial.println(len_str);
  
  return 1;
}

unsigned int end_point_request_replenishment(const char* _slot_id)
{
  char _tmp_token[500];
  unsigned int _len_str;
  Serial.println();
  Serial.println("REQUEST FOR REPLENISHMENT:\r\nConnecting to Host: dash-replenishment-service-na.amazon.com");
  _len_str = eepromReadString(600,500, _tmp_token);
  Serial.println(_tmp_token);

  access_Token = _tmp_token;
  
  Serial.print("Requesting Replenishment for slotId ");
  Serial.println(_slot_id);
  // Assemble POST request:
  //----------------------------------------------------------------
  //Request replenishment for supplied slotId
  //----------------------------------------------------------------
  
    if(!client.connect("dash-replenishment-service-na.amazon.com", 443)) 
  {
    Serial.println("Error 5 --> Connection failed");
    return 5;
  }
  client.flush();
  Serial.println("Connected\r\nSending Request for Replenishment");
  //This will send the replenishment request to the Amazon DRS server
  client.print("POST /replenish/");
  client.print(_slot_id); 
  client.println(" HTTP/1.1");
  client.println("Host: dash-replenishment-service-na.amazon.com");
  client.print("Authorization: Bearer ");
  client.println(access_Token);
  client.println("x-amzn-accept-type: com.amazon.dash.replenishment.DrsReplenishResult@1.0");
  client.println("x-amzn-type-version: com.amazon.dash.replenishment.DrsReplenishInput@1.0");
  client.println("Cache-Control: no-cache");
  client.println();
  Serial.println("Request Send");
  
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 20000) {
      Serial.println("Error 4 --> Connection Timeout !");
      client.stop();
      return 4;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  String _line = "temp";
  unsigned int _line_no = 1;
  while(client.available())
  {
    _line = client.readStringUntil('\r');
    //Serial.print(_line);
    if ((_line_no == 1)&& (_line != "HTTP/1.1 200 OK"))
    {
      Serial.println();
      Serial.println("Error 3 --> Invalid Response");
      client.stop();
      return 3;
    }
    _line_no++;
  }
  DynamicJsonBuffer jsonBuffer;
  // Responce last line have access token and referesh token json oblect 
  JsonObject& root = jsonBuffer.parseObject(_line);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("Error 2 --> Json parseObject() failed");
    return 2;
  }
  const char* _detailCode = root["detailCode"];
  // Print values
  Serial.println();
  Serial.print("detailCode: ");
  Serial.println(_detailCode);

  if(strcmp(_detailCode,"STANDARD_ORDER_PLACED")==0)
  {
    Serial.println("SUCCESS 11 --> Standard Order Placed");
    return 11;
  }
  else if(strcmp(_detailCode,"TEST_ORDER_PLACED")==0)
  {
    Serial.println("SUCCESS 12 --> Test Order Placed");
    return 12;
  }
  else if(strcmp(_detailCode,"ORDER_INPROGRESS")==0)
  {
    Serial.println("SUCCESS 13 --> Order In Progress");
    return 13;
  }
  else
  {
    return 0;
  }
}
